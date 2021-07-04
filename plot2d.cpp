#include <math.h>
#include <X11/keysym.h>
#include <stdio.h>
#include <stdlib.h>

#include "plot2d.hpp"

#define NAME        "plot2d"
#define ERR_MSG     NAME ":"

using namespace Plot2D;

const int Error::codes[] = {
    1,
    2,
    3,
    4,
    5,
    6,
    7,
    8
};

const char *Error::msgs[] = {
    ERR_MSG " color out of range",
    ERR_MSG " out of range indexing",
    ERR_MSG " X server connection failed",
    ERR_MSG " GLX configuration failed",
    ERR_MSG " GLX context creation failed",
    ERR_MSG " GLX context attachment failed",
    ERR_MSG " OpenGL shader compilation failed",
    ERR_MSG " OpenGL shader linkage failed",
    ERR_MSG " OpenGL shader configuration failed"
};

const char * const GL::vertex_shader_code =
    "#version 330 core"                                 "\n"
                                                        "\n"
    "in vec2 vertex;"                                   "\n"
                                                        "\n"
    "uniform mat3 transform;"                           "\n"
    "uniform float ratio;"                              "\n"
                                                        "\n"
    "void main(void) {"                                 "\n"
    "    vec3 v = transform * vec3(vertex, 1);"         "\n"
    "    if(ratio > 1)"                                 "\n"
    "        v.x *= 1.0 / ratio;"                       "\n"
    "    else"                                          "\n"
    "        v.y *= ratio;"                             "\n"
    "    gl_Position = vec4(v.x, v.y, 0, 1);"           "\n"
    "}"                                                 "\n";

const char * const GL::fragment_shader_code = 
    "#version 330 core"                                 "\n"
                                                        "\n"
    "out vec4 c;"                                       "\n"
                                                        "\n"
    "uniform vec4 color;"                               "\n"
                                                        "\n"
    "void main(void) {"                                 "\n"
    "    c = color;"                                    "\n"
    "}"                                                 "\n";

void Error::terminate(const char *heading) const {
    fprintf(stderr, log ? "%s%s\n%s\n" : "%s%s\n", heading, msg, log);
    exit(code);
}

Color::Color(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
    : r(red), g(green), b(blue), a(alpha)
{
    if(
        r < 0 || r > 1 ||
        g < 0 || g > 1 ||
        b < 0 || b > 1 ||
        a < 0 || a > 1
    )
        throw(Error(Error::COLOR));
}

Func *UsualFunc::copy() const {
    UsualFunc *p = new UsualFunc(fp, color);
    return p;
}

void Line::line(const Viewport& vp, const Func& f) {
    clean();
    const unsigned res = vp.get_resolution();
    pts = new Point[res];
    size = sizeof(Point) * res;
    color = f.get_color();
    unsigned i;
    const GLfloat x1 = vp.get_left_bottom().get_x();
    const GLfloat x2 = vp.get_right_top().get_x();
    GLfloat d;
    for(i = 0, d = (x2 - x1) / (res - 1); i < res; i++)
        pts[i] = Point(x1 + i * d, f(x1 + i * d));
    mode = GL_POINTS;
    load();
}

void Line::straight(const Point& a, const Point& b) {
    clean();
    pts = new Point[2];
    size = sizeof(Point) * 2;
    color = Color();
    pts[0] = a;
    pts[1] = b;
    mode = GL_LINE_STRIP;
    load();
}

void Line::draw(const GL& gl) const {
    glBindBuffer(GL_ARRAY_BUFFER, buf);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    gl.hand_color(color.get_all());
    glDrawArrays(mode, 0, size / sizeof(Point));
}

void Line::load() {
    glGenBuffers(1, &buf);
    glBindBuffer(GL_ARRAY_BUFFER, buf);
    glBufferData(GL_ARRAY_BUFFER, size, pts, GL_STATIC_DRAW);
}

void Line::clean() {
    if(pts) {
        glDeleteBuffers(1, &buf);
        delete [] pts;
    }
}

Matrix::Matrix(GLfloat s) {
    const GLfloat m[3 * 3] = {
        s, 0, 0,
        0, s, 0,
        0, 0, 1
   };
   fill(m);
}

Matrix::Matrix(GLfloat x, GLfloat y) {
    const GLfloat m[3 * 3] = {
        1, 0, x,
        0, 1, y,
        0, 0, 1
   };
   fill(m);
}

Matrix& Matrix::operator*=(const Matrix& m) {
    unsigned i, j, k;
    GLfloat t[3 * 3];
    for(i = 0; i < 3; i++)
        for(j = 0; j < 3; j++)
            for(k = 0, t[3 * i + j] = 0; k < 3; k++)
                t[3 * i + j] += m(i, k) * (*this)(k, j);
    fill(t);
    return *this;
}

GLfloat Matrix::operator()(unsigned i, unsigned j) const {
    if(i >= 3 || j >= 3)
        throw(Error(Error::INDEXING));
    return m[3 * i + j];
}

void Matrix::fill(const GLfloat data[3 * 3]) {
   unsigned i, j;
   for(i = 0; i < 3; i++)
       for(j = 0; j < 3; j++)
           m[3 * i + j] = data[3 * i + j];
}

Func& FuncSet::operator[](unsigned i) const {
    if(i >= amt)
        throw Error(Error::INDEXING);
    return *(set[i]);
}

FuncSet& FuncSet::operator=(const FuncSet& fs) {
    clean();
    vp = fs.vp;
    set = new Func*[size = fs.size];
    unsigned i;
    for(i = 0, amt = fs.amt; i < amt; i++)
        set[i] = fs.set[i]->copy();
    return *this;
}

void FuncSet::add(const Func& f) {
    if(amt == size)
        extend();
    set[amt++] = f.copy();
}

void FuncSet::extend() {
    Func* *t = new Func*[size ? size *= 2 : size = 1];
    unsigned i;
    for(i = 0; i < amt; i++)
        t[i] = set[i];
    if(set)
        delete[] set;
    set = t;
}

void FuncSet::clean() {
    unsigned i;
    for(i = 0; i < amt; i++)
        delete set[i];
    if(set)
        delete[] set;
}

const FuncSet& FuncPack::operator[](unsigned i) const {
    if(i >= amt)
        throw Error(Error::INDEXING);
    return pack[i];
}

void FuncPack::add(const FuncSet& fs) {
    if(amt == size)
        extend();
    pack[amt++] = fs;
}

void FuncPack::extend() {
    FuncSet *t = new FuncSet[size ? size *= 2 : size = 1];
    unsigned i;
    for(i = 0; i < amt; i++)
        t[i] = pack[i];
    if(pack)
        delete[] pack;
    pack = t;
}

void View::create(const FuncSet& fs) {
    clean();
    amt = fs.get_amount();
    vp = fs.get_viewport();
    const GLfloat x1 = vp. get_left_bottom().get_x();
    const GLfloat x2 = vp.   get_right_top().get_x();
    const GLfloat y1 = vp. get_left_bottom().get_y();
    const GLfloat y2 = vp.   get_right_top().get_y();
    amt += floor(x2 - x1) + (ceil(x1) == x1 ? 1 : 0) +
           floor(y2 - y1) + (ceil(y1) == y1 ? 1 : 0);
    lines = new Line[amt];
    unsigned i;
    GLfloat t;
    for(i = 0, t = ceil(x1); t <= x2; i++, t++)
        lines[i].straight(Point(t, y1), Point(t, y2));
    for(t = ceil(y1); t <= y2; i++, t++)
        lines[i].straight(Point(x1, t), Point(x2, t));
    unsigned j;
    for(j = 0; j < fs.get_amount(); j++, i++)
        lines[i].line(vp, fs[j]);
    const GLfloat x = -(x1 + x2) / 2.0;
    const GLfloat y = -(y1 + y2) / 2.0;
    const GLfloat s = x2 - x1 > y2 - y1 ? 2.0 / (x2 - x1) : 2.0 / (y2 - y1);
    m = Matrix(x, y) *= Matrix(s);
}

void View::draw(const GL& gl) const {
    unsigned i;
    for(i = 0; i < amt; i++)
        lines[i].draw(gl);
}

const GLfloat *View::matrix() const {
    return m.get_data();
}

void View::clean() {
    if(lines)
        delete[] lines;
}

Scope::Scope(const FuncPack& fp)
    : amt(fp.get_amount())
{
    views = amt ? new View[amt] : NULL;
    unsigned i;
    for(i = 0; i < amt; i++)
        views[i].create(fp[i]);
}

View *Scope::operator[](unsigned i) {
    if(i >= amt)
        throw(Error(Error::INDEXING));
    return views + i;
}

GLX::GLX(const char *name) {
    if( NULL == (dpy = XOpenDisplay(NULL)) )
        throw Error(Error::X_CONNECT);
    const int fb_attr[] = {
        GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
        GLX_RENDER_TYPE,   GLX_RGBA_BIT,
        GLX_DOUBLEBUFFER,  True,
        GLX_RED_SIZE,      1,
        GLX_GREEN_SIZE,    1, 
        GLX_BLUE_SIZE,     1,
        None
    };
    int i;
    if(NULL == (conf = glXChooseFBConfig(dpy, DefaultScreen(dpy), fb_attr, &i)))
       throw Error(Error::GLX_CONFIG);
    inf = glXGetVisualFromFBConfig(dpy, conf[0]);
    XSetWindowAttributes xwin_attr;
    xwin_attr.border_pixel = 0;
    xwin_attr.event_mask = KeyReleaseMask | StructureNotifyMask | ExposureMask;
    xwin_attr.colormap = XCreateColormap(
                                          dpy, RootWindow(dpy, inf->screen),
                                          inf->visual, AllocNone
                                        );
    unsigned long xwin_attr_mask;
    xwin_attr_mask = CWBorderPixel | CWEventMask | CWColormap;
    xwin = XCreateWindow(
                          dpy, RootWindow(dpy, inf->screen),
                          0, 0, 640, 480, 0,
                          inf->depth, InputOutput, inf->visual,
                          xwin_attr_mask, &xwin_attr
                        );
    XStoreName(dpy, xwin, name);
    if(
        NULL ==
        (ctx = glXCreateNewContext(dpy, conf[0], GLX_RGBA_TYPE, NULL, True))
      )
        throw Error(Error::GLX_CONTEXT_CREATE);
    glxwin = glXCreateWindow(dpy, conf[0], xwin, NULL);
    XMapWindow(dpy, xwin);
    if(!glXMakeContextCurrent(dpy, glxwin, glxwin, ctx))
        throw Error(Error::GLX_CONTEXT_ATTACH);
}

GLX::~GLX() {
    XFree(inf);
    XFree(conf);
    XCloseDisplay(dpy);
}

GL::GL() {
    vertex = compile_shader(GL_VERTEX_SHADER, vertex_shader_code);
    fragment = compile_shader(GL_FRAGMENT_SHADER, fragment_shader_code);
    init_program();
    if(
           ( -1 == (transform = glGetUniformLocation(program, "transform")) )
        || ( -1 == (ratio = glGetUniformLocation(program, "ratio")) )
        || ( -1 == (color = glGetUniformLocation(program, "color")) )
      )
        throw Error(Error::GL_SHADER_CONFIG);
    glClearColor(1, 1, 1, 1);
    glPointSize(1.5);
}

void GL::hand_transform(const GLfloat *d) const {
    glUniformMatrix3fv(transform, 1, GL_TRUE, d);
}

void GL::hand_ratio(GLfloat d) const {
    glUniform1f(ratio, d);
}

void GL::hand_color(const GLfloat *d) const {
    glUniform4fv(color, 1, d);
}

GLuint GL::compile_shader(GLenum type, const char *code) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &code, NULL);
    glCompileShader(shader);
    GLint compiled;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if(compiled == GL_FALSE) {
        GLint length;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
        char *log = new char[length];
        glGetShaderInfoLog(shader, length, &length, log);
        throw Error(Error::GL_SHADER_COMPILE, log);
    }
    return shader;
}

void GL::init_program() {
    program = glCreateProgram();
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);
    GLint linked;
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if(linked == GL_FALSE) {
        GLint length;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
        char *log = new char[length];
        glGetProgramInfoLog(program, length, &length, log);
        throw Error(Error::GL_SHADER_LINK, log);
    }
    glDetachShader(program, vertex);
    glDetachShader(program, fragment);
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    glUseProgram(program);
}

void Plotter::loop() {
    redraw = 1;
    stop = 0;
    while(!stop) {
        XNextEvent(glx.get_display(), &ev);
        switch(ev.type) {
            case Expose:
                redraw = 1;
                break;
            case ConfigureNotify:
                adjust();
                break;
            case KeyRelease:
                keyctl();
                break;
            default:
                ;
        }
        if(redraw) {
            draw();
            redraw = 0;
        }
    }
}

void Plotter::draw() {
    glClear(GL_COLOR_BUFFER_BIT);
    viewp->draw(gl);
    glXSwapBuffers(glx.get_display(), glx.get_glxwin());
}

void Plotter::adjust() {
    static unsigned w = 0;
    static unsigned h = 0;
    int resized = 0;
    if(w != ev.xconfigure.width) {
        w = ev.xconfigure.width;
        resized = 1;
    }
    if(h != ev.xconfigure.height) {
        h = ev.xconfigure.height;
        resized = 1;
    }
    if(resized) {
        glViewport(0, 0, w, h);
        gl.hand_ratio((GLfloat)w / (GLfloat)h);
        redraw = 1;
    }
}

void Plotter::keyctl() {
    const KeySym k = XLookupKeysym(&ev.xkey, 0);
    if(k >= XK_F1 && k <= XK_F12) {
        if(k - XK_F1 + 1 > scp.get_amount()) {
            redraw = 0;
            return;
        }
        shift(k - XK_F1);
        redraw = 1;
        return;
    }
    if(k == XK_q || k == XK_Escape) {
        stop = 1;
        return;
    }
    switch(k) {
        case XK_Up: case XK_k:
            viewp->transform(Matrix(0, -0.1));
            break;
        case XK_Down: case XK_j:
            viewp->transform(Matrix(0, 0.1));
            break;
        case XK_Right: case XK_l:
            viewp->transform(Matrix(-0.1, 0));
            break;
        case XK_Left: case XK_h:
            viewp->transform(Matrix(0.1, 0));
            break;
        case XK_plus: case XK_K: case XK_equal:
            viewp->transform(Matrix(1.1));
            break;
        case XK_minus: case XK_J:
            viewp->transform(Matrix(0.9));
            break;
        default:
            return;
    }
    gl.hand_transform(viewp->matrix());
    redraw = 1;
}

#include <string>

#include "../include/gl.hpp"
#include "../include/errors.hpp"

using namespace Plot2D;

const char * const GL::vertex_shader_code =
    "#version 330 core\n"
    "\n"
    "in vec2 vertex;\n"
    "\n"
    "uniform mat3 transform;\n"
    "uniform float ratio;\n"
    "\n"
    "void main(void) {\n"
    "    vec3 v = transform * vec3(vertex, 1);\n"
    "    if(ratio > 1)\n"
    "        v.x *= 1.0 / ratio;\n"
    "    else\n"
    "        v.y *= ratio;\n"
    "    gl_Position = vec4(v.x, v.y, 0, 1);\n"
    "}\n";

const char * const GL::fragment_shader_code = 
    "#version 330 core\n"
    "\n"
    "out vec4 c;\n"
    "\n"
    "uniform vec4 color;\n"
    "\n"
    "void main(void) {\n"
    "    c = color;\n"
    "}\n";

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
        std::string log(length, '*');
        glGetShaderInfoLog(shader, length, &length, log.data());
        throw Error(Error::GL_SHADER_COMPILE, std::move(log));
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
        std::string log(length, '*');
        glGetProgramInfoLog(program, length, &length, log.data());
        throw Error(Error::GL_SHADER_LINK, std::move(log));
    }
    glDetachShader(program, vertex);
    glDetachShader(program, fragment);
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    glUseProgram(program);
}

#ifndef __PLOT2D_HPP__
#define __PLOT2D_HPP__

#define GL_GLEXT_PROTOTYPES
#include <X11/Xlib.h>
#include <GL/gl.h>
#include <GL/glx.h>

namespace Plot2D {

class Error {
public:
    enum Type {
        COLOR = 0,
        INDEXING,
        X_CONNECT,
        GLX_CONFIG,
        GLX_CONTEXT_CREATE,
        GLX_CONTEXT_ATTACH,
        GL_SHADER_COMPILE,
        GL_SHADER_LINK,
        GL_SHADER_CONFIG,
        LAST // needed for counting
    };
    Error(Type t, const char *log = NULL)
        : code(codes[t]), msg(msgs[t]), log(log) {}
    int get_code() const { return code; }
    const char *get_msg() const { return msg; }
    const char *get_log() const { return log; }
    void terminate(const char *heading) const;
private:
    static const char *msgs[Type::LAST];
    static const int codes[Type::LAST];
    int code;
    const char *msg;
    const char *log;
};

class Color {
public:
    Color(
        GLfloat red = 0, GLfloat green = 0, GLfloat blue = 0,
        GLfloat alpha = 1
    );
    const GLfloat *get_all() const { return &r; }
    GLfloat get_red() const { return r; }
    GLfloat get_green() const { return g; }
    GLfloat get_blue() const { return b; }
    GLfloat get_alpha() const { return a; }
private:
    GLfloat r;
    GLfloat g;
    GLfloat b;
    GLfloat a;
};

class Point {
public:
    Point(GLfloat x = 0, GLfloat y = 0) : x(x), y(y) {}
    GLfloat get_x() const { return x; }
    GLfloat get_y() const { return y; }
private:
    GLfloat x;
    GLfloat y;
};

class Viewport {
public:
    Viewport(
        const Point& left_bottom = Point(),
        const Point&   right_top = Point(), unsigned resolution = 0
    ) : lb(left_bottom), rt(right_top), res(resolution) {}
    const Point& get_left_bottom() const { return lb; }
    const Point& get_right_top() const { return rt; }
    unsigned get_resolution() const { return res; }
private:
    Point lb;
    Point rt;
    unsigned res;
};

struct Func {
    Func(const Color& c = Color()) : color(c) {}
    virtual ~Func() {}
    const Color& get_color() const { return color; }
    virtual Func *copy() const = 0;
    virtual double function(double) const = 0;
    double operator()(double x) const { return function(x); }
    Color color;
};

class UsualFunc : public Func {
public:
    typedef double (*FP)(double);
    UsualFunc(FP fp, const Color& c = Color()) : Func(c), fp(fp) {}
    virtual double function(double x) const { return (*fp)(x); }
    virtual Func *copy() const;
private:
    FP fp;
};

class GL;

class Line {
public:
    Line() : pts(NULL) {}
    ~Line() { clean(); }
    void line(const Viewport& vp, const Func& f);
    void straight(const Point& a, const Point& b);
    void draw(const GL& gl) const;
private:
    Point *pts;
    unsigned size;
    GLuint buf;
    GLenum mode;
    Color color;
    void load();
    void clean();
};

class Matrix {
public:
    Matrix(GLfloat s = 1);
    Matrix(GLfloat x, GLfloat y);
    Matrix& operator*=(const Matrix& m);
    const GLfloat *get_data() const { return m; }
private:
    GLfloat m[3 * 3];
    GLfloat operator()(unsigned i, unsigned j) const;
    void fill(const GLfloat data[3 * 3]);
};

class FuncSet {
public:
    FuncSet(const Viewport& vp = Viewport(), unsigned asize = 8)
        : vp(vp), set(asize ? new Func*[asize] : NULL), size(asize), amt(0) {}
    ~FuncSet() { clean(); }
    Func& operator[](unsigned i) const;
    FuncSet& operator=(const FuncSet& fs);
    unsigned get_amount() const { return amt; }
    const Viewport& get_viewport() const { return vp; }
    void add(const Func& f);
private:
    Viewport vp;
    Func* *set;
    unsigned size;
    unsigned amt;
    void extend();
    void clean();
};

class FuncPack {
public:
    FuncPack(unsigned asize = 8)
        : pack(asize ? new FuncSet[asize] : NULL), size(asize), amt(0) {}
    ~FuncPack() { delete[] pack; }
    const FuncSet& operator[](unsigned i) const;
    unsigned get_amount() const { return amt; }
    void add(const FuncSet& fs);
private:
    FuncSet *pack;
    unsigned size;
    unsigned amt;
    void extend();
};

class View {
public:
    View() : lines(NULL) {}
    ~View() { clean(); }
    void create(const FuncSet& fs);
    void transform(const Matrix& m) { this->m *= m; }
    void draw(const GL& gl) const;
    const GLfloat *matrix() const;
private:
    Line *lines;
    unsigned amt;
    Viewport vp;
    Matrix m;
    void clean();
};

class Scope {
public:
    Scope(const FuncPack& fp);
    ~Scope() { delete[] views; }
    View *operator[](unsigned i);
    unsigned get_amount() const { return amt; }
private:
    View *views;
    unsigned amt;
};

class GLX {
public:
    GLX(const char *name);
    ~GLX();
    Display *get_display() { return dpy; }
    const GLXWindow& get_glxwin() const { return glxwin; }
private:
    Display *dpy;
    Window xwin;
    XVisualInfo *inf;
    GLXWindow glxwin;
    GLXFBConfig *conf;
    GLXContext ctx;
};

class GL {
public:
    GL();
    void hand_transform(const GLfloat *d) const;
    void hand_ratio(GLfloat d) const;
    void hand_color(const GLfloat *d) const;
private:
    static const char * const vertex_shader_code;
    static const char * const fragment_shader_code;
    GLuint vertex;
    GLuint fragment;
    GLuint program;
    GLint transform;
    GLint ratio;
    GLint color;
    GLuint compile_shader(GLenum type, const char *code);
    void init_program();
};

class Plotter {
public:
    Plotter(const char *name, const FuncPack& fp)
        : glx(GLX(name)), scp(Scope(fp)) { shift(0); }
    void loop();
private:
    GLX glx;
    GL gl;
    Scope scp;
    View *viewp;
    XEvent ev;
    int redraw;
    int stop;
    void shift(unsigned i) { gl.hand_transform( (viewp = scp[i])->matrix() ); }
    void draw();
    void adjust();
    void keyctl();
};

} // namespace Plot2D {

#endif // #ifndef __PLOT2D_HPP__

#ifndef __PLOT2D_GL_HPP__
#define __PLOT2D_GL_HPP__

#include <X11/Xlib.h>
#define GL_GLEXT_PROTOTYPES
#include <GL/glx.h>
#include <GL/gl.h>

namespace Plot2D {

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

} // namespace Plot2D {

#endif // #ifndef __PLOT2D_GL_HPP__

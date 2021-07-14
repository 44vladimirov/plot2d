#ifndef __PLOT2D_GEOMETRY_HPP__
#define __PLOT2D_GEOMETRY_HPP__

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>

namespace Plot2D {

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

} // namespace Plot2D {

#endif // #ifndef __PLOT2D_GEOMETRY_HPP__

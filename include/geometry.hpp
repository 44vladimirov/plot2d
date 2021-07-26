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
    const GLfloat* data() const { return &r; }
    GLfloat   red() const { return r; }
    GLfloat green() const { return g; }
    GLfloat  blue() const { return b; }
    GLfloat alpha() const { return a; }
private:
    GLfloat r;
    GLfloat g;
    GLfloat b;
    GLfloat a;
};

class Point {
public:
    Point(GLfloat x = 0, GLfloat y = 0) : px(x), py(y) {}
    GLfloat x() const { return px; }
    GLfloat y() const { return py; }
private:
    GLfloat px;
    GLfloat py;
};

class Viewport {
public:
    Viewport(
        const Point& left_bottom = Point(),
        const Point&   right_top = Point(), unsigned resolution = 0
    ) : lb(left_bottom), rt(right_top), res(resolution) {}
    const Point& left_bottom() const { return lb; }
    const Point&   right_top() const { return rt; }
    unsigned resolution() const { return res; }
private:
    Point lb;
    Point rt;
    unsigned res;
};

} // namespace Plot2D {

#endif // #ifndef __PLOT2D_GEOMETRY_HPP__

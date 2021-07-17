#ifndef __PLOT2D_LINES_HPP__
#define __PLOT2D_LINES_HPP__

#include "functions.hpp"
#include "gl.hpp"

namespace Plot2D {

class Line {
public:
    Line() : pts(NULL) {}
    Line(unsigned amt, const Color& c = Color());
    virtual ~Line();
    Line& operator=(Line&& l);
    Point& operator[](unsigned i);
    void set_mode(GLenum m) { mode = m; }
    void draw(const GL& gl);
private:
    Point *pts;
    unsigned size;
    unsigned amt;
    GLuint buf;
    GLenum mode;
    Color color;
    Line(const Line&) {}
    Line& operator=(const Line&) { return *this; }
};

class Straight : public Line {
public:
    Straight(const Point& a, const Point& b);
};

class Curve : public Line {
public:
    Curve(const Viewport& vp, const Func& f);
};

} // namespace Plot2D {

#endif // #ifndef __PLOT2D_LINES_HPP__

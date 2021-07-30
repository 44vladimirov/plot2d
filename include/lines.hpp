#ifndef __PLOT2D_LINES_HPP__
#define __PLOT2D_LINES_HPP__

#include <vector>

#include "functions.hpp"
#include "gl.hpp"

namespace Plot2D {

class Line {
public:
    Line(const Color& c = Color()) : c_(c) {}
    virtual ~Line() {}
    void add(const Point& p) { pts.push_back(p); }
    void draw(const GL& gl);
private:
    std::vector<Point> pts;
    Color c_;
protected:
    GLenum mode;
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

#ifndef __PLOT2D_LINES_HPP__
#define __PLOT2D_LINES_HPP__

#include "functions.hpp"
#include "gl.hpp"

namespace Plot2D {

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

} // namespace Plot2D {

#endif // #ifndef __PLOT2D_LINES_HPP__

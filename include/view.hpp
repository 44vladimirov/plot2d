#ifndef __PLOT2D_VIEW_HPP__
#define __PLOT2D_VIEW_HPP__

#include <vector>

#include "lines.hpp"

namespace Plot2D {

class Matrix {
public:
    Matrix(GLfloat s = 1);
    Matrix(GLfloat x, GLfloat y);
    Matrix& operator*=(const Matrix& m);
    const GLfloat* data() const { return m; }
private:
    GLfloat m[3*3];
    GLfloat operator()(unsigned i, unsigned j) const;
    void fill(const GLfloat data[3*3]);
};

class View {
public:
    View(const FuncSet& fs);
    void transform(const Matrix& m) { this->m *= m; }
    void draw(const GL& gl) const;
    const GLfloat* matrix() const { return m.data(); }
private:
    Viewport vp;
    std::vector<Line> lines;
    Matrix m;
};

} // namespace Plot2D {

#endif // #ifndef __PLOT2D_VIEW_HPP__

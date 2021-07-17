#ifndef __PLOT2D_SCOPE_HPP__
#define __PLOT2D_SCOPE_HPP__

#include "lines.hpp"

namespace Plot2D {

class Matrix {
public:
    Matrix(GLfloat s = 1);
    Matrix(GLfloat x, GLfloat y);
    Matrix& operator*=(const Matrix& m);
    const GLfloat* get_data() const { return m; }
private:
    GLfloat m[3 * 3];
    GLfloat operator()(unsigned i, unsigned j) const;
    void fill(const GLfloat data[3 * 3]);
};

class View {
public:
    View() : lines(NULL) {}
    View(const FuncSet& fs);
    ~View();
    View& operator=(View&& v);
    void transform(const Matrix& m) { this->m *= m; }
    void draw(const GL& gl) const;
    const GLfloat* matrix() const { return m.get_data(); }
private:
    Line *lines;
    unsigned amt;
    Viewport vp;
    Matrix m;
    View(const View&) {}
    View& operator=(const View&) { return *this; }
};

class Scope {
public:
    Scope(const FuncPack& fp);
    ~Scope() { delete[] views; }
    View* operator[](unsigned i);
    unsigned get_amount() const { return amt; }
private:
    View *views;
    unsigned amt;
    Scope& operator=(const Scope&) { return *this; }
};

} // namespace Plot2D {

#endif // #ifndef __PLOT2D_SCOPE_HPP__

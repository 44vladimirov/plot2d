#include <math.h>

#include "../include/scope.hpp"
#include "../include/errors.hpp"

using namespace Plot2D;

Matrix::Matrix(GLfloat s) {
    const GLfloat m[3 * 3] = {
        s, 0, 0,
        0, s, 0,
        0, 0, 1
   };
   fill(m);
}

Matrix::Matrix(GLfloat x, GLfloat y) {
    const GLfloat m[3 * 3] = {
        1, 0, x,
        0, 1, y,
        0, 0, 1
   };
   fill(m);
}

Matrix& Matrix::operator*=(const Matrix& m) {
    unsigned i, j, k;
    GLfloat t[3 * 3];
    for(i = 0; i < 3; i++)
        for(j = 0; j < 3; j++)
            for(k = 0, t[3 * i + j] = 0; k < 3; k++)
                t[3 * i + j] += m(i, k) * (*this)(k, j);
    fill(t);
    return *this;
}

GLfloat Matrix::operator()(unsigned i, unsigned j) const {
    if(i >= 3 || j >= 3)
        throw(Error(Error::INDEXING));
    return m[3 * i + j];
}

void Matrix::fill(const GLfloat data[3 * 3]) {
   unsigned i, j;
   for(i = 0; i < 3; i++)
       for(j = 0; j < 3; j++)
           m[3 * i + j] = data[3 * i + j];
}

View::View(const FuncSet& fs) : amt(fs.get_amount()), vp(fs.get_viewport()) {
    const GLfloat x1 = vp. get_left_bottom().get_x();
    const GLfloat x2 = vp.   get_right_top().get_x();
    const GLfloat y1 = vp. get_left_bottom().get_y();
    const GLfloat y2 = vp.   get_right_top().get_y();
    amt += floor(x2 - x1) + (ceil(x1) == x1 ? 1 : 0) +
           floor(y2 - y1) + (ceil(y1) == y1 ? 1 : 0);
    lines = new Line[amt];
    unsigned i;
    GLfloat t;
    for(i = 0, t = ceil(x1); t <= x2; i++, t++)
        lines[i] = Straight(Point(t, y1), Point(t, y2));
    for(t = ceil(y1); t <= y2; i++, t++)
        lines[i] = Straight(Point(x1, t), Point(x2, t));
    unsigned j;
    for(j = 0; j < fs.get_amount(); j++, i++)
        lines[i] = Curve(vp, fs[j]);
    const GLfloat x = -(x1 + x2) / 2.0;
    const GLfloat y = -(y1 + y2) / 2.0;
    const GLfloat s = x2 - x1 > y2 - y1 ? 2.0 / (x2 - x1) : 2.0 / (y2 - y1);
    m = Matrix(x, y) *= Matrix(s);
}

View::~View() {
    if(lines !=  NULL)
        delete[] lines;
}

View& View::operator=(View&& v) {
    lines = v.lines;
    amt = v.amt;
    vp = v.vp;
    m = v.m;
    v.lines = NULL;
    return *this;
}

void View::draw(const GL& gl) const {
    unsigned i;
    for(i = 0; i < amt; i++)
        lines[i].draw(gl);
}

Scope::Scope(const FuncPack& fp) : amt(fp.get_amount()) {
    views = amt ? new View[amt] : NULL;
    unsigned i;
    for(i = 0; i < amt; i++)
        views[i] = View(fp[i]);
}

View* Scope::operator[](unsigned i) {
    if(i >= amt)
        throw(Error(Error::INDEXING));
    return views + i;
}

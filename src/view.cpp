#include <cmath>

#include "../include/view.hpp"
#include "../include/errors.hpp"

using namespace Plot2D;

Matrix::Matrix(GLfloat s) {
    const GLfloat m[3*3] = {
        s, 0, 0,
        0, s, 0,
        0, 0, 1
    };
    fill(m);
}

Matrix::Matrix(GLfloat x, GLfloat y) {
    const GLfloat m[3*3] = {
        1, 0, x,
        0, 1, y,
        0, 0, 1
    };
    fill(m);
}

Matrix& Matrix::operator*=(const Matrix& m) {
    GLfloat t[3*3];
    for(unsigned i = 0, j, k; i < 3; i++)
        for(j = 0; j < 3; j++)
            for(k = 0, t[3*i + j] = 0; k < 3; k++)
                t[3*i + j] += m(i, k) * (*this)(k, j);
    fill(t);
    return *this;
}

GLfloat Matrix::operator()(unsigned i, unsigned j) const {
    if(i >= 3 || j >= 3)
        throw(Error(Error::INDEXING));
    return m[3*i + j];
}

void Matrix::fill(const GLfloat data[3*3]) {
   for(unsigned i = 0, j; i < 3; i++)
       for(j = 0; j < 3; j++)
           m[3*i + j] = data[3*i + j];
}

View::View(const FuncSet& fs) : vp(fs.viewport()) {
    const GLfloat x1 = vp.left_bottom().x();
    const GLfloat x2 = vp.  right_top().x();
    const GLfloat y1 = vp.left_bottom().y();
    const GLfloat y2 = vp.  right_top().y();
    lines.resize(
        fs.size() +
        std::floor(x2 - x1) + (std::ceil(x1) == x1 ? 1 : 0) +
        std::floor(y2 - y1) + (std::ceil(y1) == y1 ? 1 : 0)
    );
    size_t i;
    GLfloat t;
    for(i = 0, t = std::ceil(x1); t <= x2; i++, t++)
        lines[i] = Straight(Point(t, y1), Point(t, y2));
    for(t = std::ceil(y1); t <= y2; i++, t++)
        lines[i] = Straight(Point(x1, t), Point(x2, t));
    for(size_t j = 0; j < fs.size(); j++, i++)
        lines[i] = Curve(vp, fs[j]);
    const GLfloat x = -(x1 + x2) / 2.0;
    const GLfloat y = -(y1 + y2) / 2.0;
    const GLfloat s = x2 - x1 > y2 - y1 ? 2.0 / (x2 - x1) : 2.0 / (y2 - y1);
    m = Matrix(x, y) *= Matrix(s);
}

void View::draw(const GL& gl) const {
    for(Line l : lines)
        l.draw(gl);
}

#include "../include/lines.hpp"
#include "../include/errors.hpp"

using namespace Plot2D;

Line::Line(unsigned amt, const Color& c) : amt(amt), color(c) {
    pts = new Point[amt];
    size = sizeof(*pts) * amt;
}

Line::~Line() {
    if(pts) {
        glDeleteBuffers(1, &buf);
        delete [] pts;
    }
}

Line& Line::operator=(Line&& l) {
    pts = l.pts;
    size = l.size;
    amt = l.amt;
    buf = l.buf;
    mode = l.mode;
    color = l.color;
    l.pts = NULL;
    l.size = l.amt = 0;
    return *this;
}

Point& Line::operator[](unsigned i) {
    if(i >= amt)
        throw(Error(Error::INDEXING));
    return pts[i];
}

void Line::draw(const GL& gl) {
    glGenBuffers(1, &buf);
    glBindBuffer(GL_ARRAY_BUFFER, buf);
    glBufferData(GL_ARRAY_BUFFER, size, pts, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, buf);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    gl.hand_color(color.get_all());
    glDrawArrays(mode, 0, amt);
}

Straight::Straight(const Point& a, const Point& b) : Line(2) {
    (*this)[0] = a;
    (*this)[1] = b;
    set_mode(GL_LINE_STRIP);
}

Curve::Curve(const Viewport& vp, const Func& f)
    : Line(vp.get_resolution(), f.get_color())
{
    unsigned i;
    const unsigned res = vp.get_resolution();
    const GLfloat x1 = vp.get_left_bottom().get_x();
    const GLfloat x2 = vp.get_right_top().get_x();
    GLfloat d;
    for(i = 0, d = (x2 - x1) / (res - 1); i < res; i++)
        (*this)[i] = Point(x1 + i * d, f(x1 + i * d));
    set_mode(GL_POINTS);
}

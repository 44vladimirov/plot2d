#include "../include/lines.hpp"
#include "../include/errors.hpp"

using namespace Plot2D;

void Line::draw(const GL& gl) {
    GLuint buf;
    glGenBuffers(1, &buf);
    glBindBuffer(GL_ARRAY_BUFFER, buf);
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(pts[0]) * pts.size(), pts.data(),
        GL_STATIC_DRAW
    );
    glBindBuffer(GL_ARRAY_BUFFER, buf);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    gl.hand_color(c.data());
    glDrawArrays(mode, 0, pts.size());
}

Straight::Straight(const Point& a, const Point& b) {
    add(a);
    add(b);
    mode = GL_LINE_STRIP;
}

Curve::Curve(const Viewport& vp, const Func& f) : Line(f.color()) {
    const unsigned res = vp.resolution();
    const GLfloat x1 = vp.left_bottom().x();
    const GLfloat x2 = vp.  right_top().x();
    unsigned i;
    GLfloat d;
    for(i = 0, d = (x2 - x1) / (res - 1); i < res; i++)
        add( Point(x1 + i*d, f(x1 + i*d)) );
    mode = GL_POINTS;
}

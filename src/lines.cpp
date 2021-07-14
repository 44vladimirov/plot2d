#include "../include/lines.hpp"

using namespace Plot2D;

void Line::line(const Viewport& vp, const Func& f) {
    clean();
    const unsigned res = vp.get_resolution();
    pts = new Point[res];
    size = sizeof(Point) * res;
    color = f.get_color();
    unsigned i;
    const GLfloat x1 = vp.get_left_bottom().get_x();
    const GLfloat x2 = vp.get_right_top().get_x();
    GLfloat d;
    for(i = 0, d = (x2 - x1) / (res - 1); i < res; i++)
        pts[i] = Point(x1 + i * d, f(x1 + i * d));
    mode = GL_POINTS;
    load();
}

void Line::straight(const Point& a, const Point& b) {
    clean();
    pts = new Point[2];
    size = sizeof(Point) * 2;
    color = Color();
    pts[0] = a;
    pts[1] = b;
    mode = GL_LINE_STRIP;
    load();
}

void Line::draw(const GL& gl) const {
    glBindBuffer(GL_ARRAY_BUFFER, buf);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    gl.hand_color(color.get_all());
    glDrawArrays(mode, 0, size / sizeof(Point));
}

void Line::load() {
    glGenBuffers(1, &buf);
    glBindBuffer(GL_ARRAY_BUFFER, buf);
    glBufferData(GL_ARRAY_BUFFER, size, pts, GL_STATIC_DRAW);
}

void Line::clean() {
    if(pts) {
        glDeleteBuffers(1, &buf);
        delete [] pts;
    }
}

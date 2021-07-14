#ifndef __PLOT2D_PLOTTER_HPP__
#define __PLOT2D_PLOTTER_HPP__

#include "scope.hpp"
#include "gl.hpp"

namespace Plot2D {

class Plotter {
public:
    Plotter(const char *name, const FuncPack& fp)
        : glx(GLX(name)), scp(Scope(fp)) { shift(0); }
    void loop();
private:
    GLX glx;
    GL gl;
    Scope scp;
    View *viewp;
    XEvent ev;
    int redraw;
    int stop;
    void shift(unsigned i) { gl.hand_transform( (viewp = scp[i])->matrix() ); }
    void draw();
    void adjust();
    void keyctl();
};

} // namespace Plot2D {

#endif // #ifndef __PLOT2D_PLOTTER_HPP__

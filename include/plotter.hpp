#ifndef __PLOT2D_PLOTTER_HPP__
#define __PLOT2D_PLOTTER_HPP__

#include <vector>

#include "view.hpp"
#include "gl.hpp"

namespace Plot2D {

class Plotter {
public:
    Plotter(const char *name, const FuncPack& fp);
    void loop();
private:
    GLX glx;
    GL gl;
    std::vector<View> views;
    View *active;
    bool redraw;
    bool stop;
    void shift(size_t i);
    void draw();
    void adjust(const XEvent& ev);
    void keyctl(XEvent& ev);
};

} // namespace Plot2D {

#endif // #ifndef __PLOT2D_PLOTTER_HPP__

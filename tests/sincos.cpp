#include "../plot2d.hpp"
#include <math.h>

int main() {
    using namespace Plot2D;
    try {
        const Color  blue(0,    0, 0.75);
        const Color green(0, 0.75,    0);
        const Viewport vp(Point(0, -2), Point(2 * M_PI, 2), 100);
        FuncSet fs(vp);
        FuncPack fp;
        fs.add(UsualFunc(&sin, blue));
        fp.add(fs);
        fs.add(UsualFunc(&cos, green));
        fp.add(fs);
        Plotter p("Plot2D test: sin cos", fp);
        p.loop();
    }
    catch(const Error& err) {
        err.terminate("");
    }
    return 0;
}

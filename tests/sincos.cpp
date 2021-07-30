#include "../plot2d.hpp"
#include <cmath>

int main() {
    try {
        const Plot2D::Color  blue(0,    0, 0.75);
        const Plot2D::Color green(0, 0.75,    0);
        const Plot2D::Viewport vp(
            Plot2D::Point(0, -2), Plot2D::Point(2 * M_PI, 2), 100
        );

        Plot2D::FuncSet fs(vp);
        fs.add(Plot2D::UsualFunc(&sin, blue));
        Plot2D::FuncPack fp;
        fp.add(fs);
        fs.add(Plot2D::UsualFunc(&cos, green));
        fp.add(fs);

        Plot2D::Plotter p("Plot2D test: sin cos", fp);
        p.loop();
    }
    catch(const Plot2D::Error& err) {
        err.terminate("");
    }
    return 0;
}

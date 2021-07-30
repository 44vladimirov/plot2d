#include "../include/plotter.hpp"
#include "../include/errors.hpp"

using namespace Plot2D;

Plotter::Plotter(const char *name, const FuncPack& fp) : glx(GLX(name)) {
    views.reserve(fp.size());
    for(size_t i = 0; i < fp.size(); i++)
        views.push_back(View(fp[i]));
    if(views.empty())
        throw Error(Error::INIT);
    shift(0);
}

void Plotter::loop() {
    redraw = true;
    stop = false;
    while(!stop) {
        XEvent ev;
        XNextEvent(glx.display(), &ev);
        switch(ev.type) {
        case Expose:
            redraw = true;
            break;
        case ConfigureNotify:
            adjust(ev);
            break;
        case KeyRelease:
            keyctl(ev);
            break;
        default:
            ;
        }
        if(redraw) {
            draw();
            redraw = false;
        }
    }
}

void Plotter::shift(size_t i) {
    gl.hand_transform( (active = views.data() + i)->matrix() );
}

void Plotter::draw() {
    glClear(GL_COLOR_BUFFER_BIT);
    active->draw(gl);
    glXSwapBuffers(glx.display(), glx.window());
}

void Plotter::adjust(const XEvent& ev) {
    static unsigned w = 0;
    static unsigned h = 0;
    bool resized = false;
    if(w != ev.xconfigure.width) {
        w = ev.xconfigure.width;
        resized = true;
    }
    if(h != ev.xconfigure.height) {
        h = ev.xconfigure.height;
        resized = true;
    }
    if(resized) {
        glViewport(0, 0, w, h);
        gl.hand_ratio((GLfloat)w / (GLfloat)h);
        redraw = true;
    }
}

void Plotter::keyctl(XEvent& ev) {
    const KeySym k = XLookupKeysym(&ev.xkey, 0);
    if(k >= XK_F1 && k <= XK_F12) {
        if(k - XK_F1 + 1 > views.size()) {
            redraw = false;
            return;
        }
        shift(k - XK_F1);
        redraw = true;
        return;
    }
    if(k == XK_q || k == XK_Escape) {
        stop = true;
        return;
    }
    switch(k) {
    case XK_Up: case XK_k:
        active->transform(Matrix(0, -0.1));
        break;
    case XK_Down: case XK_j:
        active->transform(Matrix(0, 0.1));
        break;
    case XK_Right: case XK_l:
        active->transform(Matrix(-0.1, 0));
        break;
    case XK_Left: case XK_h:
        active->transform(Matrix(0.1, 0));
        break;
    case XK_plus: case XK_K: case XK_equal:
        active->transform(Matrix(1.1));
        break;
    case XK_minus: case XK_J:
        active->transform(Matrix(1.0 / 1.1));
        break;
    default:
        return;
    }
    gl.hand_transform(active->matrix());
    redraw = true;
}

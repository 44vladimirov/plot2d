#include "../include/plotter.hpp"
#include "../include/errors.hpp"

using namespace Plot2D;

void Plotter::loop() {
    redraw = 1;
    stop = 0;
    while(!stop) {
        XNextEvent(glx.get_display(), &ev);
        switch(ev.type) {
            case Expose:
                redraw = 1;
                break;
            case ConfigureNotify:
                adjust();
                break;
            case KeyRelease:
                keyctl();
                break;
            default:
                ;
        }
        if(redraw) {
            draw();
            redraw = 0;
        }
    }
}

void Plotter::draw() {
    glClear(GL_COLOR_BUFFER_BIT);
    viewp->draw(gl);
    glXSwapBuffers(glx.get_display(), glx.get_glxwin());
}

void Plotter::adjust() {
    static unsigned w = 0;
    static unsigned h = 0;
    int resized = 0;
    if(w != ev.xconfigure.width) {
        w = ev.xconfigure.width;
        resized = 1;
    }
    if(h != ev.xconfigure.height) {
        h = ev.xconfigure.height;
        resized = 1;
    }
    if(resized) {
        glViewport(0, 0, w, h);
        gl.hand_ratio((GLfloat)w / (GLfloat)h);
        redraw = 1;
    }
}

void Plotter::keyctl() {
    const KeySym k = XLookupKeysym(&ev.xkey, 0);
    if(k >= XK_F1 && k <= XK_F12) {
        if(k - XK_F1 + 1 > scp.get_amount()) {
            redraw = 0;
            return;
        }
        shift(k - XK_F1);
        redraw = 1;
        return;
    }
    if(k == XK_q || k == XK_Escape) {
        stop = 1;
        return;
    }
    switch(k) {
        case XK_Up: case XK_k:
            viewp->transform(Matrix(0, -0.1));
            break;
        case XK_Down: case XK_j:
            viewp->transform(Matrix(0, 0.1));
            break;
        case XK_Right: case XK_l:
            viewp->transform(Matrix(-0.1, 0));
            break;
        case XK_Left: case XK_h:
            viewp->transform(Matrix(0.1, 0));
            break;
        case XK_plus: case XK_K: case XK_equal:
            viewp->transform(Matrix(1.1));
            break;
        case XK_minus: case XK_J:
            viewp->transform(Matrix(0.9));
            break;
        default:
            return;
    }
    gl.hand_transform(viewp->matrix());
    redraw = 1;
}

#include "../include/functions.hpp"

using namespace Plot2D;

Func *UsualFunc::copy() const {
    UsualFunc *p = new UsualFunc(fp, color);
    return p;
}

#include "../include/geometry.hpp"
#include "../include/errors.hpp"

using namespace Plot2D;

Color::Color(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
    : r(red), g(green), b(blue), a(alpha)
{
    if(
        r < 0 || r > 1 ||
        g < 0 || g > 1 ||
        b < 0 || b > 1 ||
        a < 0 || a > 1
    )
        throw(Error(Error::COLOR));
}

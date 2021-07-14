#include <stdio.h>
#include <stdlib.h>

#include "../include/errors.hpp"

#define NAME    "plot2d"
#define ERR_MSG NAME ":"

using namespace Plot2D;

const int Error::codes[] = {
    1,
    2,
    3,
    4,
    5,
    6,
    7,
    8
};

const char *Error::msgs[] = {
    ERR_MSG " color out of range",
    ERR_MSG " out of range indexing",
    ERR_MSG " X server connection failed",
    ERR_MSG " GLX configuration failed",
    ERR_MSG " GLX context creation failed",
    ERR_MSG " GLX context attachment failed",
    ERR_MSG " OpenGL shader compilation failed",
    ERR_MSG " OpenGL shader linkage failed",
    ERR_MSG " OpenGL shader configuration failed"
};

void Error::terminate(const char *heading) const {
    fprintf(stderr, log ? "%s%s\n%s\n" : "%s%s\n", heading, msg, log);
    exit(code);
}

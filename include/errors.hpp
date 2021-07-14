#ifndef __PLOT2D_ERRORS_HPP__
#define __PLOT2D_ERRORS_HPP__

#include <stddef.h>

namespace Plot2D {

class Error {
public:
    enum Type {
        COLOR = 0,
        INDEXING,
        X_CONNECT,
        GLX_CONFIG,
        GLX_CONTEXT_CREATE,
        GLX_CONTEXT_ATTACH,
        GL_SHADER_COMPILE,
        GL_SHADER_LINK,
        GL_SHADER_CONFIG,
        LAST // needed for counting
    };
    Error(Type t, const char *log = NULL)
        : code(codes[t]), msg(msgs[t]), log(log) {}
    int get_code() const { return code; }
    const char *get_msg() const { return msg; }
    const char *get_log() const { return log; }
    void terminate(const char *heading) const;
private:
    static const char *msgs[Type::LAST];
    static const int codes[Type::LAST];
    int code;
    const char *msg;
    const char *log;
};

} // namespace Plot2D {

#endif // #ifndef __PLOT2D_ERRORS_HPP__

#ifndef __PLOT2D_ERRORS_HPP__
#define __PLOT2D_ERRORS_HPP__

#include <string>

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
    Error(Type t, const std::string& log = std::string()) : tp(t), lg(log) {}
    int code() const { return codes[tp]; }
    const char* msg() const { return msgs[tp]; }
    const char* log() const { return lg.data(); }
    void terminate(const char *heading) const;
private:
    static const char *msgs[Type::LAST];
    static const int codes[Type::LAST];
    Type tp;
    std::string lg;
};

} // namespace Plot2D {

#endif // #ifndef __PLOT2D_ERRORS_HPP__

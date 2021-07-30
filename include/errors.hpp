#ifndef __PLOT2D_ERRORS_HPP__
#define __PLOT2D_ERRORS_HPP__

#include <exception>
#include <string>

namespace Plot2D {

class Error : public std::exception {
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
        INIT,
        LAST // needed for counting
    };
    Error(Type t, const std::string& log = std::string()) : t_(t), log_(log) {}
    int code() const { return codes[t_]; }
    const char* msg() const { return msgs[t_]; }
    const char* log() const { return log_.data(); }
    void terminate(const char *heading) const;
    const char* what() const noexcept { return "Plot2D::Error"; }
private:
    static const char *msgs[Type::LAST];
    static const int codes[Type::LAST];
    Type t_;
    std::string log_;
};

} // namespace Plot2D {

#endif // #ifndef __PLOT2D_ERRORS_HPP__

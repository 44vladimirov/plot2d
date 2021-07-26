#ifndef __PLOT2D_FUNCTIONS_HPP__
#define __PLOT2D_FUNCTIONS_HPP__

#include <memory>
#include <vector>

#include "geometry.hpp"

namespace Plot2D {

class Func {
public:
    typedef std::shared_ptr<Func> Pointer;
    Func(const Color& c = Color()) : c(c) {}
    virtual ~Func() {}
    const Color& color() const { return c; }
    virtual Pointer copy() const = 0;
    virtual double operator()(double x) const = 0;
private:
    Color c;
};

class UsualFunc : public Func {
public:
    typedef double (*FP)(double);
    UsualFunc(FP fp, const Color& c = Color()) : Func(c), fp(fp) {}
    Pointer copy() const { return Pointer(new UsualFunc(*this)); }
    double operator()(double x) const { return (*fp)(x); }
private:
    FP fp;
};

class FuncSet {
public:
    FuncSet(const Viewport& vp = Viewport()) : vp(vp) {}
    size_t size() const { return set.size(); }
    const Viewport& viewport() const { return vp; }
    const Func& operator[](size_t i) const;
    void add(const Func& f) { set.push_back(f.copy()); }
private:
    Viewport vp;
    std::vector<Func::Pointer> set;
};

class FuncPack {
public:
    size_t size() const { return pack.size(); }
    const FuncSet& operator[](size_t i) const;
    void add(const FuncSet& fs) { pack.push_back(fs); }
private:
    std::vector<FuncSet> pack;
};

} // namespace Plot2D {

#endif // #ifndef __PLOT2D_FUNCTIONS_HPP__

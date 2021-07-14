#ifndef __PLOT2D_FUNCTIONS_HPP__
#define __PLOT2D_FUNCTIONS_HPP__

#include <stddef.h>

#include "geometry.hpp"

namespace Plot2D {

struct Func {
    Func(const Color& c = Color()) : color(c) {}
    virtual ~Func() {}
    const Color& get_color() const { return color; }
    virtual Func *copy() const = 0;
    virtual double function(double) const = 0;
    double operator()(double x) const { return function(x); }
    Color color;
};

class UsualFunc : public Func {
public:
    typedef double (*FP)(double);
    UsualFunc(FP fp, const Color& c = Color()) : Func(c), fp(fp) {}
    virtual double function(double x) const { return (*fp)(x); }
    virtual Func *copy() const;
private:
    FP fp;
};

class FuncSet {
public:
    FuncSet(const Viewport& vp = Viewport(), unsigned asize = 8)
        : vp(vp), set(asize ? new Func*[asize] : NULL), size(asize), amt(0) {}
    ~FuncSet() { clean(); }
    Func& operator[](unsigned i) const;
    FuncSet& operator=(const FuncSet& fs);
    unsigned get_amount() const { return amt; }
    const Viewport& get_viewport() const { return vp; }
    void add(const Func& f);
private:
    Viewport vp;
    Func* *set;
    unsigned size;
    unsigned amt;
    void extend();
    void clean();
};

class FuncPack {
public:
    FuncPack(unsigned asize = 8)
        : pack(asize ? new FuncSet[asize] : NULL), size(asize), amt(0) {}
    ~FuncPack() { delete[] pack; }
    const FuncSet& operator[](unsigned i) const;
    unsigned get_amount() const { return amt; }
    void add(const FuncSet& fs);
private:
    FuncSet *pack;
    unsigned size;
    unsigned amt;
    void extend();
};

} // namespace Plot2D {

#endif // #ifndef __PLOT2D_FUNCTIONS_HPP__

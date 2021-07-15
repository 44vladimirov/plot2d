#include "../include/functions.hpp"
#include "../include/errors.hpp"

using namespace Plot2D;

Func *UsualFunc::copy() const {
    UsualFunc *p = new UsualFunc(fp, color);
    return p;
}

Func& FuncSet::operator[](unsigned i) const {
    if(i >= amt)
        throw Error(Error::INDEXING);
    return *(set[i]);
}

FuncSet& FuncSet::operator=(const FuncSet& fs) {
    clean();
    vp = fs.vp;
    set = new Func*[size = fs.size];
    unsigned i;
    for(i = 0, amt = fs.amt; i < amt; i++)
        set[i] = fs.set[i]->copy();
    return *this;
}

void FuncSet::add(const Func& f) {
    if(amt == size)
        extend();
    set[amt++] = f.copy();
}

void FuncSet::extend() {
    Func* *t = new Func*[size ? size *= 2 : size = 1];
    unsigned i;
    for(i = 0; i < amt; i++)
        t[i] = set[i];
    if(set)
        delete[] set;
    set = t;
}

void FuncSet::clean() {
    unsigned i;
    for(i = 0; i < amt; i++)
        delete set[i];
    if(set)
        delete[] set;
}

const FuncSet& FuncPack::operator[](unsigned i) const {
    if(i >= amt)
        throw Error(Error::INDEXING);
    return pack[i];
}

void FuncPack::add(const FuncSet& fs) {
    if(amt == size)
        extend();
    pack[amt++] = fs;
}

void FuncPack::extend() {
    FuncSet *t = new FuncSet[size ? size *= 2 : size = 1];
    unsigned i;
    for(i = 0; i < amt; i++)
        t[i] = pack[i];
    if(pack)
        delete[] pack;
    pack = t;
}

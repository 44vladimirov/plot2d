#include "../include/functions.hpp"
#include "../include/errors.hpp"

using namespace Plot2D;

const Func& FuncSet::operator[](size_t i) const {
    if(i >= set.size())
        throw Error(Error::INDEXING);
    return *(set[i]);
}

const FuncSet& FuncPack::operator[](size_t i) const {
    if(i >= pack.size())
        throw Error(Error::INDEXING);
    return pack[i];
}

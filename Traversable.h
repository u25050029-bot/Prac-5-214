#ifndef CAMPUSGUARD_TRAVERSABLE_H
#define CAMPUSGUARD_TRAVERSABLE_H

#include <memory>

#include "Iterator.h"

template <typename T>
class Traversable {
public:
    Traversable() {}
    virtual ~Traversable() {}
    Traversable(const Traversable&) = delete;
    Traversable& operator=(const Traversable&) = delete;

    virtual std::unique_ptr<Iterator<T>> createIterator() = 0;
};

#endif

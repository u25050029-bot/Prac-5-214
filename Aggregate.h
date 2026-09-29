#ifndef CAMPUSGUARD_AGGREGATE_H
#define CAMPUSGUARD_AGGREGATE_H

#include <memory>

#include "Iterator.h"

template <typename T>
class Aggregate {
public:
    Aggregate() {}
    virtual ~Aggregate() {}
    Aggregate(const Aggregate&) = delete;
    Aggregate& operator=(const Aggregate&) = delete;

    virtual std::unique_ptr<Iterator<T>> createIterator() = 0;
};

#endif

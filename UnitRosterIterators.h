#ifndef CAMPUSGUARD_UNITROSTERITERATORS_H
#define CAMPUSGUARD_UNITROSTERITERATORS_H

#include <cstddef>
#include <vector>

#include "Iterator.h"
#include "ResponseUnit.h"
#include "UnitRoster.h"

class RosterIterator : public Iterator<ResponseUnit*> {
public:
    explicit RosterIterator(const UnitRoster& roster);
    ~RosterIterator() override;

    void first() override;
    void next() override;
    bool isDone() const override;
    ResponseUnit* currentItem() const override;

private:
    const UnitRoster& roster_;
    std::size_t index_;
};

class AvailableUnitIterator : public Iterator<ResponseUnit*> {
public:
    AvailableUnitIterator(const UnitRoster& roster, UnitType type);
    ~AvailableUnitIterator() override;

    void first() override;
    void next() override;
    bool isDone() const override;
    ResponseUnit* currentItem() const override;

private:
    std::vector<ResponseUnit*> matches_;
    std::size_t index_;
};

#endif

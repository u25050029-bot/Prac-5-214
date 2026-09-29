#include "UnitRosterIterators.h"

#include <stdexcept>

RosterIterator::RosterIterator(const UnitRoster& roster) : roster_(roster), index_(0) {}

RosterIterator::~RosterIterator() {}

void RosterIterator::first() {
    index_ = 0;
}

void RosterIterator::next() {
    if (isDone()) {
        throw std::out_of_range("roster iterator advanced past the end");
    }
    ++index_;
}

bool RosterIterator::isDone() const {
    return index_ >= roster_.size();
}

ResponseUnit* RosterIterator::currentItem() const {
    if (isDone()) {
        throw std::out_of_range("roster iterator has no current item");
    }
    return roster_.at(index_);
}

AvailableUnitIterator::AvailableUnitIterator(const UnitRoster& roster, UnitType type)
    : roster_(roster), type_(type), index_(0) {
    skipUnsuitable();
}

AvailableUnitIterator::~AvailableUnitIterator() {}

void AvailableUnitIterator::first() {
    index_ = 0;
    skipUnsuitable();
}

void AvailableUnitIterator::next() {
    if (isDone()) {
        throw std::out_of_range("available-unit iterator advanced past the end");
    }
    ++index_;
    skipUnsuitable();
}

bool AvailableUnitIterator::isDone() const {
    return index_ >= roster_.size();
}

ResponseUnit* AvailableUnitIterator::currentItem() const {
    if (isDone()) {
        throw std::out_of_range("available-unit iterator has no current item");
    }
    return roster_.at(index_);
}

void AvailableUnitIterator::skipUnsuitable() {
    while (index_ < roster_.size()) {
        ResponseUnit* unit = roster_.at(index_);
        if (unit->getType() == type_ && unit->isAvailable()) {
            return;
        }
        ++index_;
    }
}

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

AvailableUnitIterator::AvailableUnitIterator(const UnitRoster& roster, UnitType type) : index_(0) {
    for (std::size_t i = 0; i < roster.size(); ++i) {
        if (roster.at(i)->getType() == type) {
            matches_.push_back(roster.at(i));
        }
    }
}

AvailableUnitIterator::~AvailableUnitIterator() {}

void AvailableUnitIterator::first() {
    index_ = 0;
}

void AvailableUnitIterator::next() {
    if (isDone()) {
        throw std::out_of_range("available-unit iterator advanced past the end");
    }
    ++index_;
}

bool AvailableUnitIterator::isDone() const {
    return index_ >= matches_.size();
}

ResponseUnit* AvailableUnitIterator::currentItem() const {
    if (isDone()) {
        throw std::out_of_range("available-unit iterator has no current item");
    }
    return matches_[index_];
}

#include "UnitRoster.h"

#include <stdexcept>

#include "UnitRosterIterators.h"

UnitRoster::UnitRoster() {}

UnitRoster::~UnitRoster() {}

ResponseUnit* UnitRoster::add(std::unique_ptr<ResponseUnit> unit) {
    if (!unit) {
        throw std::invalid_argument("cannot add an empty unit to the roster");
    }
    units_.push_back(std::move(unit));
    return units_.back().get();
}

std::size_t UnitRoster::size() const {
    return units_.size();
}

ResponseUnit* UnitRoster::at(std::size_t index) const {
    if (index >= units_.size()) {
        throw std::out_of_range("roster index out of range");
    }
    return units_[index].get();
}

std::unique_ptr<Iterator<ResponseUnit*>> UnitRoster::createIterator() {
    return std::unique_ptr<Iterator<ResponseUnit*>>(new RosterIterator(*this));
}

std::unique_ptr<Iterator<ResponseUnit*>> UnitRoster::createAvailableIterator(UnitType type) {
    return std::unique_ptr<Iterator<ResponseUnit*>>(new AvailableUnitIterator(*this, type));
}

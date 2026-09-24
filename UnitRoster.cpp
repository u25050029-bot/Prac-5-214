#include "UnitRoster.h"

#include "UnitRosterIterators.h"

UnitRoster::UnitRoster() {}

UnitRoster::~UnitRoster() {}

ResponseUnit* UnitRoster::add(std::unique_ptr<ResponseUnit> unit) {
    units_.push_back(std::move(unit));
    return units_.back().get();
}

std::size_t UnitRoster::size() const {
    return units_.size();
}

ResponseUnit* UnitRoster::at(std::size_t index) const {
    return units_[index].get();
}

std::unique_ptr<Iterator<ResponseUnit*>> UnitRoster::createIterator() {
    return std::unique_ptr<Iterator<ResponseUnit*>>(new RosterIterator(*this));
}

std::unique_ptr<Iterator<ResponseUnit*>> UnitRoster::createAvailableIterator(UnitType type) {
    return std::unique_ptr<Iterator<ResponseUnit*>>(new AvailableUnitIterator(*this, type));
}

ResponseUnit* UnitRoster::findByCallsign(const std::string& callsign) const {
    for (std::size_t i = 0; i < units_.size(); ++i) {
        if (units_[i]->getCallsign() == callsign) {
            return units_[i].get();
        }
    }
    return nullptr;
}

int UnitRoster::countOfType(UnitType type) const {
    int total = 0;
    for (std::size_t i = 0; i < units_.size(); ++i) {
        if (units_[i]->getType() == type) {
            ++total;
        }
    }
    return total;
}

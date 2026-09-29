#include "ResponseUnit.h"

#include <stdexcept>

#include "Incident.h"

ResponseUnit::ResponseUnit(const std::string& callsign, UnitType type)
    : callsign_(callsign), type_(type), assignment_(nullptr) {}

const std::string& ResponseUnit::getCallsign() const {
    return callsign_;
}

UnitType ResponseUnit::getType() const {
    return type_;
}

bool ResponseUnit::isAvailable() const {
    return assignment_ == nullptr;
}

Incident* ResponseUnit::getAssignment() const {
    return assignment_;
}

void ResponseUnit::assign(Incident* incident) {
    if (incident == nullptr) {
        throw std::invalid_argument(callsign_ + " cannot be assigned to an empty incident");
    }
    if (assignment_ != nullptr) {
        throw std::logic_error(callsign_ + " is already assigned to incident " + assignment_->label());
    }
    assignment_ = incident;
}

void ResponseUnit::release() {
    assignment_ = nullptr;
}

std::string ResponseUnit::statusText() const {
    std::string text = callsign_ + " (" + toString(type_) + "): ";
    return text + (assignment_ == nullptr ? std::string("available") : "assigned to incident " + assignment_->label());
}

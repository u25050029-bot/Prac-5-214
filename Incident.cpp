#include "Incident.h"

#include <stdexcept>

#include "AreaComponent.h"
#include "ResponseUnit.h"

Incident::Incident(int id, IncidentType type, Severity severity, AreaComponent* location, const std::string& description)
    : id_(id), type_(type), severity_(severity), status_(IncidentStatus::Reported), location_(location), description_(description) {
    if (location_ == nullptr) {
        throw std::invalid_argument("an incident needs a campus location");
    }
}

int Incident::getId() const {
    return id_;
}

IncidentType Incident::getType() const {
    return type_;
}

Severity Incident::getSeverity() const {
    return severity_;
}

IncidentStatus Incident::getStatus() const {
    return status_;
}

AreaComponent* Incident::getLocation() const {
    return location_;
}

const std::string& Incident::getDescription() const {
    return description_;
}

const std::vector<ResponseUnit*>& Incident::getUnits() const {
    return units_;
}

bool Incident::isActive() const {
    return status_ != IncidentStatus::Resolved;
}

bool Incident::hasUnitOfType(UnitType type) const {
    for (ResponseUnit* unit : units_) {
        if (unit->getType() == type) {
            return true;
        }
    }
    return false;
}

void Incident::setSeverity(Severity severity) {
    severity_ = severity;
}

void Incident::changeStatus(IncidentStatus next) {
    if (!isAllowed(status_, next)) {
        throw std::logic_error("invalid status change for incident " + label() + ": " + toString(status_) + " -> " + toString(next));
    }
    status_ = next;
}

void Incident::attachUnit(ResponseUnit* unit) {
    if (unit == nullptr) {
        throw std::invalid_argument("cannot attach an empty unit");
    }
    bool present = false;
    for (std::size_t i = 0; i < units_.size(); ++i) {
        if (units_[i] == unit) {
            present = true;
        }
    }
    if (!present) {
        units_.push_back(unit);
    }
}

void Incident::detachUnit(ResponseUnit* unit) {
    for (std::size_t i = 0; i < units_.size(); ++i) {
        if (units_[i] == unit) {
            units_.erase(units_.begin() + i);
            return;
        }
    }
}

std::string Incident::label() const {
    return "#" + std::to_string(id_) + " " + toString(type_);
}

std::string Incident::summary() const {
    std::string units;
    for (ResponseUnit* unit : units_) {
        units += (units.empty() ? "" : ", ") + unit->getCallsign();
    }
    return label() + " at " + location_->getName() + " | severity " + toString(severity_) + " | status " + toString(status_) + " | units: " + (units.empty() ? std::string("none") : units) + " | " + description_;
}

bool Incident::isAllowed(IncidentStatus from, IncidentStatus to) {
    if (from == IncidentStatus::Reported && to == IncidentStatus::Dispatched) {
        return true;
    }
    if (from == IncidentStatus::Dispatched && to == IncidentStatus::Contained) {
        return true;
    }
    if (from == IncidentStatus::Dispatched && to == IncidentStatus::Resolved) {
        return true;
    }
    if (from == IncidentStatus::Contained && to == IncidentStatus::Resolved) {
        return true;
    }
    return false;
}

#include "IncidentRegistry.h"

#include <stdexcept>

#include "AreaComponent.h"
#include "Log.h"

IncidentRegistry::IncidentRegistry() : componentName_("IncidentRegistry"), nextId_(1) {}

IncidentRegistry::~IncidentRegistry() {
    for (Incident* incident : incidents_) {
        delete incident;
    }
}

const std::string& IncidentRegistry::getComponentName() const {
    return componentName_;
}

Incident* IncidentRegistry::report(IncidentType type, Severity severity, AreaComponent* location, const std::string& description) {
    if (location == nullptr) {
        throw std::invalid_argument("cannot register incident: location is not part of the campus model");
    }
    Incident* incident = new Incident(nextId_++, type, severity, location, description);
    incidents_.push_back(incident);
    Log::line(getComponentName(), "Registered " + incident->summary());
    return incident;
}

void IncidentRegistry::markDispatched(Incident* incident) {
    if (incident == nullptr) {
        throw std::invalid_argument("no incident selected");
    }
    if (incident->getStatus() != IncidentStatus::Reported) {
        return;
    }
    IncidentStatus previous = incident->getStatus();
    incident->changeStatus(IncidentStatus::Dispatched);
    Log::line(getComponentName(), "Incident " + incident->label() + " status " + toString(previous) + " -> " + toString(IncidentStatus::Dispatched));
}

void IncidentRegistry::revertToReported(Incident* incident) {
    if (incident == nullptr) {
        throw std::invalid_argument("no incident selected");
    }
    if (incident->getStatus() != IncidentStatus::Dispatched) {
        return;
    }
    IncidentStatus previous = incident->getStatus();
    incident->changeStatus(IncidentStatus::Reported);
    Log::line(getComponentName(), "Incident " + incident->label() + " status " + toString(previous) + " -> " + toString(IncidentStatus::Reported));
}

void IncidentRegistry::contain(Incident* incident) {
    if (incident == nullptr) {
        throw std::invalid_argument("no incident selected");
    }
    IncidentStatus previous = incident->getStatus();
    incident->changeStatus(IncidentStatus::Contained);
    Log::line(getComponentName(), "Incident " + incident->label() + " status " + toString(previous) + " -> " + toString(IncidentStatus::Contained));
}

void IncidentRegistry::resolve(Incident* incident) {
    if (incident == nullptr) {
        throw std::invalid_argument("no incident selected");
    }
    IncidentStatus previous = incident->getStatus();
    incident->changeStatus(IncidentStatus::Resolved);
    Log::line(getComponentName(), "Incident " + incident->label() + " status " + toString(previous) + " -> " + toString(IncidentStatus::Resolved));
}

void IncidentRegistry::escalate(Incident* incident, Severity severity) {
    require(incident);
    if (!incident->isActive()) {
        throw std::logic_error("cannot escalate closed incident " + incident->label());
    }
    Severity previous = incident->getSeverity();
    incident->setSeverity(severity);
    Log::line(getComponentName(), "Incident " + incident->label() + " severity " + toString(previous) + " -> " + toString(severity));
}

void IncidentRegistry::printIncidents() const {
    Log::line(getComponentName(), std::to_string(incidents_.size()) + " incident(s) on record:");
    Log::Scope scope;
    for (Incident* incident : incidents_) {
        Log::line("Incident", incident->summary());
    }
}

void IncidentRegistry::require(const Incident* incident) {
    if (incident == nullptr) {
        throw std::invalid_argument("no incident selected");
    }
}

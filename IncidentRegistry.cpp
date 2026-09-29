#include "IncidentRegistry.h"

#include <stdexcept>

#include "AreaComponent.h"
#include "CoordinationEvent.h"
#include "Log.h"

IncidentRegistry::IncidentRegistry() : ResponseComponent("IncidentRegistry"), nextId_(1) {}

IncidentRegistry::~IncidentRegistry() {}

Incident* IncidentRegistry::report(IncidentType type, Severity severity, AreaComponent* location, const std::string& description) {
    if (location == nullptr) {
        throw std::invalid_argument("cannot register incident: location is not part of the campus model");
    }
    incidents_.push_back(std::unique_ptr<Incident>(new Incident(nextId_++, type, severity, location, description)));
    Incident* incident = incidents_.back().get();
    Log::line(getComponentName(), "Registered " + incident->summary());
    return incident;
}

void IncidentRegistry::markDispatched(Incident* incident) {
    require(incident);
    if (incident->getStatus() == IncidentStatus::Reported) {
        transition(incident, IncidentStatus::Dispatched);
    }
}

void IncidentRegistry::revertToReported(Incident* incident) {
    require(incident);
    if (incident->getStatus() == IncidentStatus::Dispatched) {
        transition(incident, IncidentStatus::Reported);
    }
}

void IncidentRegistry::contain(Incident* incident) {
    transition(incident, IncidentStatus::Contained);
}

void IncidentRegistry::resolve(Incident* incident) {
    transition(incident, IncidentStatus::Resolved);
    changed(CoordinationEvent(EventType::IncidentResolved, incident, incident->getLocation(), nullptr));
}

void IncidentRegistry::escalate(Incident* incident, Severity severity) {
    require(incident);
    if (!incident->isActive()) {
        throw std::logic_error("cannot escalate closed incident " + incident->label());
    }
    Severity previous = incident->getSeverity();
    incident->setSeverity(severity);
    Log::line(getComponentName(), "Incident " + incident->label() + " severity " + toString(previous) + " -> " + toString(severity));
    if (severity == Severity::Critical) {
        changed(CoordinationEvent(EventType::IncidentEscalated, incident, incident->getLocation(), nullptr));
    }
}

void IncidentRegistry::printIncidents() const {
    Log::line(getComponentName(), std::to_string(incidents_.size()) + " incident(s) on record:");
    Log::Scope scope;
    for (const auto& incident : incidents_) {
        Log::line("Incident", incident->summary());
    }
}

void IncidentRegistry::transition(Incident* incident, IncidentStatus next) {
    require(incident);
    IncidentStatus previous = incident->getStatus();
    incident->changeStatus(next);
    Log::line(getComponentName(), "Incident " + incident->label() + " status " + toString(previous) + " -> " + toString(next));
}

void IncidentRegistry::require(const Incident* incident) {
    if (incident == nullptr) {
        throw std::invalid_argument("no incident selected");
    }
}

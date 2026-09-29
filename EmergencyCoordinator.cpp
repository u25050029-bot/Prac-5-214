#include "EmergencyCoordinator.h"

#include <stdexcept>
#include <string>

#include "AccessControlService.h"
#include "AlertService.h"
#include "AreaComponent.h"
#include "CoordinationEvent.h"
#include "DispatchService.h"
#include "Incident.h"
#include "IncidentRegistry.h"
#include "Log.h"
#include "ResponseComponent.h"
#include "ResponseUnit.h"

EmergencyCoordinator::EmergencyCoordinator(IncidentRegistry& registry, DispatchService& dispatch, AccessControlService& access, AlertService& alerts)
    : registry_(registry), dispatch_(dispatch), access_(access), alerts_(alerts) {
    rules_[EventType::UnitDispatched] = &EmergencyCoordinator::onUnitDispatched;
    rules_[EventType::UnitRecalled] = &EmergencyCoordinator::onUnitRecalled;
    rules_[EventType::IncidentEscalated] = &EmergencyCoordinator::onIncidentEscalated;
    rules_[EventType::IncidentResolved] = &EmergencyCoordinator::onIncidentResolved;
    rules_[EventType::AreaSecured] = &EmergencyCoordinator::onAreaSecured;
    rules_[EventType::AccessFailure] = &EmergencyCoordinator::onAccessFailure;
    rules_[EventType::EvacuationIssued] = &EmergencyCoordinator::onEvacuationIssued;
    registry_.setMediator(this);
    dispatch_.setMediator(this);
    access_.setMediator(this);
    alerts_.setMediator(this);
}

EmergencyCoordinator::~EmergencyCoordinator() {
    registry_.setMediator(nullptr);
    dispatch_.setMediator(nullptr);
    access_.setMediator(nullptr);
    alerts_.setMediator(nullptr);
}

void EmergencyCoordinator::notify(ResponseComponent& sender, const CoordinationEvent& event) {
    std::map<EventType, Rule>::const_iterator rule = rules_.find(event.type);
    if (rule == rules_.end()) {
        return;
    }
    std::string context = event.incident != nullptr ? " for incident " + event.incident->label() : std::string();
    Log::line("Mediator", "EmergencyCoordinator received " + toString(event.type) + " from " + sender.getComponentName() + context);
    Log::Scope scope;
    try {
        (this->*(rule->second))(event);
    } catch (const std::exception& ex) {
        Log::line("Mediator", std::string("Coordination step could not complete: ") + ex.what());
    }
}

void EmergencyCoordinator::onUnitDispatched(const CoordinationEvent& event) {
    Incident* incident = event.incident;
    registry_.markDispatched(incident);
    alerts_.notifyArea(event.area, event.unit->getCallsign() + " (" + toString(event.unit->getType()) + ") en route to incident " + incident->label());
    bool securityOnIntrusion = event.unit->getType() == UnitType::Security && incident->getType() == IncidentType::Intrusion;
    if (securityOnIntrusion && event.area->securedCount() == 0) {
        Log::line("Mediator", "Rule: first security unit on an intrusion -> lock down " + event.area->getName());
        access_.secureArea(event.area, incident);
    }
}

void EmergencyCoordinator::onUnitRecalled(const CoordinationEvent& event) {
    Incident* incident = event.incident;
    alerts_.notifyArea(event.area, event.unit->getCallsign() + " stood down from incident " + incident->label());
    if (incident->getUnits().empty()) {
        Log::line("Mediator", "Rule: no units left on scene -> incident returns to Reported");
        registry_.revertToReported(incident);
    } else {
        Log::line("Mediator", "Incident " + incident->label() + " still has " + std::to_string(incident->getUnits().size()) + " unit(s) on scene; status unchanged");
    }
}

void EmergencyCoordinator::onIncidentEscalated(const CoordinationEvent& event) {
    Incident* incident = event.incident;
    Log::line("Mediator", "Rule: critical escalation -> emergency alert + backup medical unit");
    alerts_.activateAlert(event.area, AlertLevel::Emergency, "Incident " + incident->label() + " is CRITICAL; keep access routes clear", incident);
    try {
        dispatch_.dispatchAvailable(incident, UnitType::Medical);
    } catch (const std::runtime_error& ex) {
        Log::line("Mediator", std::string("Backup dispatch impossible (") + ex.what() + "); falling back to external EMS");
        alerts_.notifyArea(event.area, "External EMS requested for incident " + incident->label());
    }
}

void EmergencyCoordinator::onIncidentResolved(const CoordinationEvent& event) {
    Incident* incident = event.incident;
    Log::line("Mediator", "Rule: resolution -> release units, reopen area, clear alerts, all-clear");
    dispatch_.releaseUnits(incident);
    if (event.area->securedCount() > 0) {
        access_.releaseArea(event.area, incident);
    }
    alerts_.clearIncident(incident);
    alerts_.notifyArea(event.area, "ALL CLEAR for incident " + incident->label());
}

void EmergencyCoordinator::onAreaSecured(const CoordinationEvent& event) {
    alerts_.notifyArea(event.area, "Lockdown confirmed: shelter in place and keep doors closed");
}

void EmergencyCoordinator::onAccessFailure(const CoordinationEvent& event) {
    Incident* incident = event.incident;
    alerts_.notifyArea(event.area, "Access-control fault (" + event.detail + "); manual door check required");
    if (incident == nullptr || !incident->isActive()) {
        Log::line("Mediator", "No active incident to attach a repair crew to; fault logged for manual follow-up");
        return;
    }
    if (incident->hasUnitOfType(UnitType::Facilities)) {
        Log::line("Mediator", "Facilities already on incident " + incident->label() + "; no extra dispatch");
        return;
    }
    Log::line("Mediator", "Rule: door fault during an active incident -> dispatch facilities");
    dispatch_.dispatchAvailable(incident, UnitType::Facilities);
}

void EmergencyCoordinator::onEvacuationIssued(const CoordinationEvent& event) {
    Log::line("Mediator", "Rule: evacuation -> unlock every door in " + event.area->getName() + " for egress");
    access_.releaseArea(event.area, event.incident);
}

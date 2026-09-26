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
    registry_.setMediator(this);
    dispatch_.setMediator(this);
    access_.setMediator(this);
    alerts_.setMediator(this);
}

EmergencyCoordinator::~EmergencyCoordinator() {}

void EmergencyCoordinator::notify(ResponseComponent& sender, const CoordinationEvent& event) {
    std::string context = event.incident != nullptr ? " for incident " + event.incident->label() : std::string();
    Log::line("Mediator", "EmergencyCoordinator received " + toString(event.type) + " from " + sender.getComponentName() + context);
    Log::Scope scope;
    try {
        switch (event.type) {
        case EventType::UnitDispatched: {
            Incident* incident = event.incident;
            registry_.markDispatched(incident);
            alerts_.notifyArea(event.area, event.unit->getCallsign() + " (" + toString(event.unit->getType()) + ") en route to incident " + incident->label());
            bool securityOnIntrusion = event.unit->getType() == UnitType::Security && incident->getType() == IncidentType::Intrusion;
            if (securityOnIntrusion && event.area->securedCount() == 0) {
                Log::line("Mediator", "Rule: first security unit on an intrusion -> lock down " + event.area->getName());
                access_.secureArea(event.area, incident);
            }
            break;
        }
        case EventType::UnitRecalled: {
            Incident* incident = event.incident;
            alerts_.notifyArea(event.area, event.unit->getCallsign() + " stood down from incident " + incident->label());
            if (incident->getUnits().empty()) {
                Log::line("Mediator", "Rule: no units left on scene -> incident returns to Reported");
                registry_.revertToReported(incident);
            } else {
                Log::line("Mediator", "Incident " + incident->label() + " still has " + std::to_string(incident->getUnits().size()) + " unit(s) on scene; status unchanged");
            }
            break;
        }
        case EventType::IncidentEscalated: {
            Incident* incident = event.incident;
            Log::line("Mediator", "Rule: critical escalation -> emergency alert + backup medical unit");
            alerts_.activateAlert(event.area, AlertLevel::Emergency, "Incident " + incident->label() + " is CRITICAL; keep access routes clear", incident);
            try {
                dispatch_.dispatchAvailable(incident, UnitType::Medical);
            } catch (const std::runtime_error& ex) {
                Log::line("Mediator", std::string("Backup dispatch impossible (") + ex.what() + "); falling back to external EMS");
                alerts_.notifyArea(event.area, "External EMS requested for incident " + incident->label());
            }
            break;
        }
        case EventType::IncidentResolved: {
            Incident* incident = event.incident;
            Log::line("Mediator", "Rule: resolution -> release units, reopen area, clear alerts, all-clear");
            dispatch_.releaseUnits(incident);
            access_.releaseArea(event.area, incident);
            alerts_.clearIncident(incident);
            alerts_.notifyArea(event.area, "ALL CLEAR for incident " + incident->label());
            break;
        }
        case EventType::AreaSecured: {
            alerts_.notifyArea(event.area, "Lockdown confirmed: shelter in place and keep doors closed");
            break;
        }
        case EventType::AccessFailure: {
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
            break;
        }
        case EventType::EvacuationIssued: {
            Log::line("Mediator", "Rule: evacuation -> unlock every door in " + event.area->getName() + " for egress");
            access_.releaseArea(event.area, event.incident);
            break;
        }
        }
    } catch (const std::exception& ex) {
        Log::line("Mediator", std::string("Coordination step could not complete: ") + ex.what());
    }
}

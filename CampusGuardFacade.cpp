#include "CampusGuardFacade.h"

#include <stdexcept>

#include "AccessControlService.h"
#include "AlertService.h"
#include "AreaComponent.h"
#include "DispatchService.h"
#include "Incident.h"
#include "IncidentRegistry.h"
#include "Log.h"

CampusGuardFacade::CampusGuardFacade(IncidentRegistry& registry, DispatchService& dispatch, AccessControlService& access, AlertService& alerts)
    : registry_(registry), dispatch_(dispatch), access_(access), alerts_(alerts) {}

CampusGuardFacade::~CampusGuardFacade() {}

Incident* CampusGuardFacade::respondToFire(const std::string& buildingName, const std::string& description) {
    Log::line("Facade", "respondToFire(\"" + buildingName + "\") started");
    Log::Scope scope;
    AreaComponent* building = access_.findArea(buildingName);
    if (building == nullptr) {
        throw std::invalid_argument("fire workflow aborted before any action: unknown building '" + buildingName + "'");
    }
    Log::line("Facade", "Step 1/5 IncidentRegistry.report");
    Incident* incident = registry_.report(IncidentType::Fire, Severity::Critical, building, description);
    Log::line("Facade", "Step 2/5 AlertService.activateAlert");
    alerts_.activateAlert(building, AlertLevel::Emergency, "Fire alarm: leave " + buildingName + " immediately", incident);
    Log::line("Facade", "Step 3/5 AlertService.issueEvacuation");
    alerts_.issueEvacuation(building, incident);
    Log::line("Facade", "Step 4/5 DispatchService.dispatchAvailable(Facilities)");
    try {
        dispatch_.dispatchAvailable(incident, UnitType::Facilities);
    } catch (const std::runtime_error& ex) {
        Log::line("Facade", std::string("Facilities dispatch failed: ") + ex.what());
    }
    Log::line("Facade", "Step 5/5 DispatchService.dispatchAvailable(Medical)");
    try {
        dispatch_.dispatchAvailable(incident, UnitType::Medical);
    } catch (const std::runtime_error& ex) {
        Log::line("Facade", std::string("Medical dispatch failed: ") + ex.what());
    }
    Log::line("Facade", "respondToFire complete: " + incident->summary());
    return incident;
}

void CampusGuardFacade::standDown(Incident* incident) {
    if (incident == nullptr) {
        throw std::invalid_argument("stand-down needs an incident");
    }
    Log::line("Facade", "standDown(" + incident->label() + ") started");
    Log::Scope scope;
    IncidentStatus status = incident->getStatus();
    if (status == IncidentStatus::Dispatched) {
        Log::line("Facade", "Step 1/3 IncidentRegistry.contain");
        registry_.contain(incident);
    } else if (status == IncidentStatus::Reported) {
        Log::line("Facade", "Step 1/3 skipped: incident is " + toString(status));
    } else if (status == IncidentStatus::Contained) {
        Log::line("Facade", "Step 1/3 skipped: incident is " + toString(status));
    } else {
        Log::line("Facade", "Step 1/3 skipped: incident is " + toString(status));
    }
    Log::line("Facade", "Step 2/3 IncidentRegistry.resolve");
    registry_.resolve(incident);
    Log::line("Facade", "Step 3/3 AccessControlService.printAccessReport");
    access_.printAccessReport(incident->getLocation());
}

void CampusGuardFacade::situationReport() const {
    Log::line("Facade", "situationReport()");
    Log::Scope scope;
    registry_.printIncidents();
    dispatch_.printRoster();
    alerts_.printStatus();
}

#include "DispatchService.h"

#include <memory>
#include <stdexcept>
#include <vector>

#include "CoordinationEvent.h"
#include "Incident.h"
#include "Iterator.h"
#include "Log.h"
#include "ResponseUnit.h"
#include "UnitRoster.h"

DispatchService::DispatchService(UnitRoster& roster) : ResponseComponent("DispatchService"), roster_(roster) {}

DispatchService::~DispatchService() {}

ResponseUnit* DispatchService::dispatchAvailable(Incident* incident, UnitType type) {
    if (incident == nullptr) {
        throw std::invalid_argument("no incident selected for dispatch");
    }
    if (!incident->isActive()) {
        throw std::logic_error("incident " + incident->label() + " is " + toString(incident->getStatus()) + "; dispatch refused");
    }
    std::unique_ptr<Iterator<ResponseUnit*>> candidates = roster_.createAvailableIterator(type);
    candidates->first();
    if (candidates->isDone()) {
        throw std::runtime_error("no available " + toString(type) + " unit on the roster");
    }
    ResponseUnit* unit = candidates->currentItem();
    unit->assign(incident);
    incident->attachUnit(unit);
    Log::line(getComponentName(), "AvailableUnitIterator selected " + unit->getCallsign() + "; dispatched to incident " + incident->label());
    changed(CoordinationEvent(EventType::UnitDispatched, incident, incident->getLocation(), unit));
    return unit;
}

void DispatchService::recall(ResponseUnit* unit, Incident* incident) {
    if (unit == nullptr || incident == nullptr || unit->getAssignment() != incident) {
        throw std::logic_error("recall refused: unit is not assigned to that incident");
    }
    unit->release();
    incident->detachUnit(unit);
    Log::line(getComponentName(), unit->getCallsign() + " recalled from incident " + incident->label());
    changed(CoordinationEvent(EventType::UnitRecalled, incident, incident->getLocation(), unit));
}

void DispatchService::releaseUnits(Incident* incident) {
    if (incident == nullptr) {
        throw std::invalid_argument("no incident selected for unit release");
    }
    std::vector<ResponseUnit*> units = incident->getUnits();
    for (ResponseUnit* unit : units) {
        unit->release();
        incident->detachUnit(unit);
        Log::line(getComponentName(), unit->getCallsign() + " released and available again");
    }
}

void DispatchService::printRoster() const {
    Log::line(getComponentName(), "Unit roster (RosterIterator):");
    Log::Scope scope;
    std::unique_ptr<Iterator<ResponseUnit*>> it = roster_.createIterator();
    for (it->first(); !it->isDone(); it->next()) {
        Log::line("Unit", it->currentItem()->statusText());
    }
}

#include <exception>
#include <iostream>
#include <memory>
#include <string>
#include <utility>

#include "AccessPoint.h"
#include "AreaGroup.h"
#include "Incident.h"
#include "IncidentRegistry.h"
#include "Iterator.h"
#include "LegacyAccessAdapter.h"
#include "LegacyDoorController.h"
#include "Log.h"
#include "ResponseUnit.h"
#include "UnitRoster.h"

namespace {

AreaGroup* addGroup(AreaGroup& parent, const std::string& name, const std::string& kind) {
    return parent.add(std::unique_ptr<AreaGroup>(new AreaGroup(name, kind)));
}

void addDoor(AreaGroup& floor, LegacyAccessAdapter& gateway, const std::string& doorId, int panelNumber) {
    floor.add(std::unique_ptr<AccessPoint>(new AccessPoint(doorId, gateway)));
    gateway.enrolDoor(doorId, panelNumber);
}

std::unique_ptr<LegacyAccessAdapter> buildGateway() {
    std::unique_ptr<LegacyDoorController> controller(new LegacyDoorController());
    const int panels[] = {101, 102, 103, 104, 201, 202, 203, 301, 302};
    for (int panel : panels) {
        controller->installPanel(panel);
    }
    controller->setPanelOnline(104, false);
    return std::unique_ptr<LegacyAccessAdapter>(new LegacyAccessAdapter(std::move(controller)));
}

std::unique_ptr<AreaGroup> buildCampus(LegacyAccessAdapter& gateway) {
    std::unique_ptr<AreaGroup> campus(new AreaGroup("Hatfield Campus", "Campus"));

    AreaGroup* engineering = addGroup(*campus, "Engineering", "Building");
    AreaGroup* engineeringGround = addGroup(*engineering, "Engineering Ground", "Floor");
    addDoor(*engineeringGround, gateway, "EB-G-MAIN", 101);
    addDoor(*engineeringGround, gateway, "EB-G-EAST", 102);
    AreaGroup* engineeringLevel1 = addGroup(*engineering, "Engineering Level 1", "Floor");
    addDoor(*engineeringLevel1, gateway, "EB-1-LAB", 103);
    addDoor(*engineeringLevel1, gateway, "EB-1-SERVER", 104);

    AreaGroup* library = addGroup(*campus, "Library", "Building");
    AreaGroup* libraryGround = addGroup(*library, "Library Ground", "Floor");
    addDoor(*libraryGround, gateway, "ML-G-MAIN", 201);
    addDoor(*libraryGround, gateway, "ML-G-FIRE", 202);
    AreaGroup* libraryLevel1 = addGroup(*library, "Library Level 1", "Floor");
    addDoor(*libraryLevel1, gateway, "ML-1-ARCHIVE", 203);

    AreaGroup* studentCentre = addGroup(*campus, "Student Centre", "Building");
    AreaGroup* studentCentreGround = addGroup(*studentCentre, "Student Centre Ground", "Floor");
    addDoor(*studentCentreGround, gateway, "SC-G-MAIN", 301);
    addDoor(*studentCentreGround, gateway, "SC-G-CLINIC", 302);

    return campus;
}

void buildRoster(UnitRoster& roster) {
    roster.add(std::unique_ptr<ResponseUnit>(new ResponseUnit("Alpha", UnitType::Security)));
    roster.add(std::unique_ptr<ResponseUnit>(new ResponseUnit("Bravo", UnitType::Security)));
    roster.add(std::unique_ptr<ResponseUnit>(new ResponseUnit("Medic-1", UnitType::Medical)));
    roster.add(std::unique_ptr<ResponseUnit>(new ResponseUnit("Medic-2", UnitType::Medical)));
    roster.add(std::unique_ptr<ResponseUnit>(new ResponseUnit("Delta", UnitType::Facilities)));
}

AreaComponent* findArea(AreaComponent& root, const std::string& name) {
    std::unique_ptr<Iterator<AreaComponent*>> it = root.createIterator();
    for (it->first(); !it->isDone(); it->next()) {
        if (it->currentItem()->getName() == name) {
            return it->currentItem();
        }
    }
    return nullptr;
}

void debugIncident(Incident* incident) {
    Log::line("debug", "incident id=" + std::to_string(incident->getId()));
    Log::line("debug", "  type=" + toString(incident->getType()));
    Log::line("debug", "  severity=" + toString(incident->getSeverity()));
    Log::line("debug", "  status=" + toString(incident->getStatus()));
    Log::line("debug", "  location=" + incident->getLocation()->getName());
    Log::line("debug", "  units=" + std::to_string(incident->getUnits().size()));
    for (std::size_t i = 0; i < incident->getUnits().size(); ++i) {
        Log::line("debug", "    unit " + incident->getUnits()[i]->getCallsign());
    }
}

void printRoster(UnitRoster& roster) {
    std::unique_ptr<Iterator<ResponseUnit*>> it = roster.createIterator();
    for (it->first(); !it->isDone(); it->next()) {
        Log::line("Unit", it->currentItem()->statusText());
    }
}

}

int main() {
    try {
        std::unique_ptr<LegacyAccessAdapter> gateway = buildGateway();
        std::unique_ptr<AreaGroup> campus = buildCampus(*gateway);
        UnitRoster roster;
        buildRoster(roster);
        IncidentRegistry registry;

        Log::banner("Incident lifecycle check");
        Incident* intrusion = registry.report(IncidentType::Intrusion, Severity::High, findArea(*campus, "Engineering"), "Forced-entry alarm at the server room");

        Log::step("Pick the first available security unit");
        std::unique_ptr<Iterator<ResponseUnit*>> candidates = roster.createAvailableIterator(UnitType::Security);
        candidates->first();
        ResponseUnit* unit = candidates->currentItem();
        unit->assign(intrusion);
        intrusion->attachUnit(unit);
        registry.markDispatched(intrusion);
        debugIncident(intrusion);
        printRoster(roster);

        Log::step("Contain and resolve");
        registry.contain(intrusion);
        registry.resolve(intrusion);
        debugIncident(intrusion);
        unit->release();
        intrusion->detachUnit(unit);

        Log::step("Invalid request: resolve twice");
        try {
            registry.resolve(intrusion);
        } catch (const std::exception& ex) {
            Log::line("Operator", std::string("Request rejected: ") + ex.what());
        }
        registry.printIncidents();
    } catch (const std::exception& ex) {
        std::cerr << "CampusGuard terminated: " << ex.what() << "\n";
        return 1;
    }
    return 0;
}

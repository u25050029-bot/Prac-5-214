#include "CampusGuardSystem.h"

#include <string>

#include "AccessPoint.h"
#include "LegacyDoorController.h"
#include "ResponseUnit.h"

namespace {

AreaGroup* addGroup(AreaGroup& parent, const std::string& name, const std::string& kind) {
    return parent.add(std::unique_ptr<AreaGroup>(new AreaGroup(name, kind)));
}

void addDoor(AreaGroup& floor, LegacyAccessAdapter& gateway, const std::string& doorId, int panelNumber) {
    floor.add(std::unique_ptr<AccessPoint>(new AccessPoint(doorId, gateway)));
    gateway.enrolDoor(doorId, panelNumber);
}

}

CampusGuardSystem::CampusGuardSystem()
    : gateway_(buildGateway()),
      campus_(buildCampus(*gateway_)),
      roster_(),
      registry_(),
      dispatch_(roster_),
      access_(*campus_),
      alerts_(),
      coordinator_(registry_, dispatch_, access_, alerts_) {
    buildRoster(roster_);
}

CampusGuardSystem::~CampusGuardSystem() {}

IncidentRegistry& CampusGuardSystem::registry() {
    return registry_;
}

DispatchService& CampusGuardSystem::dispatch() {
    return dispatch_;
}

AccessControlService& CampusGuardSystem::access() {
    return access_;
}

AlertService& CampusGuardSystem::alerts() {
    return alerts_;
}

std::unique_ptr<LegacyAccessAdapter> CampusGuardSystem::buildGateway() {
    std::unique_ptr<LegacyDoorController> controller(new LegacyDoorController());
    const int panels[] = {101, 102, 103, 104, 201, 202, 203, 301, 302};
    for (int panel : panels) {
        controller->installPanel(panel);
    }
    controller->setPanelOnline(104, false);
    return std::unique_ptr<LegacyAccessAdapter>(new LegacyAccessAdapter(std::move(controller)));
}

std::unique_ptr<AreaGroup> CampusGuardSystem::buildCampus(LegacyAccessAdapter& gateway) {
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

void CampusGuardSystem::buildRoster(UnitRoster& roster) {
    roster.add(std::unique_ptr<ResponseUnit>(new ResponseUnit("Alpha", UnitType::Security)));
    roster.add(std::unique_ptr<ResponseUnit>(new ResponseUnit("Bravo", UnitType::Security)));
    roster.add(std::unique_ptr<ResponseUnit>(new ResponseUnit("Medic-1", UnitType::Medical)));
    roster.add(std::unique_ptr<ResponseUnit>(new ResponseUnit("Medic-2", UnitType::Medical)));
    roster.add(std::unique_ptr<ResponseUnit>(new ResponseUnit("Delta", UnitType::Facilities)));
}

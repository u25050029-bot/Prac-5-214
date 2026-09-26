#include "CampusGuardSystem.h"

#include "AccessPoint.h"
#include "LegacyDoorController.h"
#include "ResponseUnit.h"

CampusGuardSystem::CampusGuardSystem()
    : gateway_(buildGateway()),
      campus_(buildCampus(*gateway_)),
      roster_(),
      registry_(),
      dispatch_(roster_),
      access_(*campus_),
      alerts_() {
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
    controller->installPanel(101);
    controller->installPanel(102);
    controller->installPanel(103);
    controller->installPanel(104);
    controller->installPanel(201);
    controller->installPanel(202);
    controller->installPanel(203);
    controller->installPanel(301);
    controller->installPanel(302);
    controller->setPanelOnline(104, false);
    return std::unique_ptr<LegacyAccessAdapter>(new LegacyAccessAdapter(std::move(controller)));
}

std::unique_ptr<AreaGroup> CampusGuardSystem::buildCampus(LegacyAccessAdapter& gateway) {
    std::unique_ptr<AreaGroup> campus(new AreaGroup("Hatfield Campus", "Campus"));

    AreaGroup* engineering = campus->add(std::unique_ptr<AreaGroup>(new AreaGroup("Engineering", "Building")));
    AreaGroup* engineeringGround = engineering->add(std::unique_ptr<AreaGroup>(new AreaGroup("Engineering Ground", "Floor")));
    engineeringGround->add(std::unique_ptr<AccessPoint>(new AccessPoint("EB-G-MAIN", gateway)));
    gateway.enrolDoor("EB-G-MAIN", 101);
    engineeringGround->add(std::unique_ptr<AccessPoint>(new AccessPoint("EB-G-EAST", gateway)));
    gateway.enrolDoor("EB-G-EAST", 102);
    AreaGroup* engineeringLevel1 = engineering->add(std::unique_ptr<AreaGroup>(new AreaGroup("Engineering Level 1", "Floor")));
    engineeringLevel1->add(std::unique_ptr<AccessPoint>(new AccessPoint("EB-1-LAB", gateway)));
    gateway.enrolDoor("EB-1-LAB", 103);
    engineeringLevel1->add(std::unique_ptr<AccessPoint>(new AccessPoint("EB-1-SERVER", gateway)));
    gateway.enrolDoor("EB-1-SERVER", 104);

    AreaGroup* library = campus->add(std::unique_ptr<AreaGroup>(new AreaGroup("Library", "Building")));
    AreaGroup* libraryGround = library->add(std::unique_ptr<AreaGroup>(new AreaGroup("Library Ground", "Floor")));
    libraryGround->add(std::unique_ptr<AccessPoint>(new AccessPoint("ML-G-MAIN", gateway)));
    gateway.enrolDoor("ML-G-MAIN", 201);
    libraryGround->add(std::unique_ptr<AccessPoint>(new AccessPoint("ML-G-FIRE", gateway)));
    gateway.enrolDoor("ML-G-FIRE", 202);
    AreaGroup* libraryLevel1 = library->add(std::unique_ptr<AreaGroup>(new AreaGroup("Library Level 1", "Floor")));
    libraryLevel1->add(std::unique_ptr<AccessPoint>(new AccessPoint("ML-1-ARCHIVE", gateway)));
    gateway.enrolDoor("ML-1-ARCHIVE", 203);

    AreaGroup* studentCentre = campus->add(std::unique_ptr<AreaGroup>(new AreaGroup("Student Centre", "Building")));
    AreaGroup* studentCentreGround = studentCentre->add(std::unique_ptr<AreaGroup>(new AreaGroup("Student Centre Ground", "Floor")));
    studentCentreGround->add(std::unique_ptr<AccessPoint>(new AccessPoint("SC-G-MAIN", gateway)));
    gateway.enrolDoor("SC-G-MAIN", 301);
    studentCentreGround->add(std::unique_ptr<AccessPoint>(new AccessPoint("SC-G-CLINIC", gateway)));
    gateway.enrolDoor("SC-G-CLINIC", 302);

    return campus;
}

void CampusGuardSystem::buildRoster(UnitRoster& roster) {
    roster.add(std::unique_ptr<ResponseUnit>(new ResponseUnit("Alpha", UnitType::Security)));
    roster.add(std::unique_ptr<ResponseUnit>(new ResponseUnit("Bravo", UnitType::Security)));
    roster.add(std::unique_ptr<ResponseUnit>(new ResponseUnit("Medic-1", UnitType::Medical)));
    roster.add(std::unique_ptr<ResponseUnit>(new ResponseUnit("Medic-2", UnitType::Medical)));
    roster.add(std::unique_ptr<ResponseUnit>(new ResponseUnit("Delta", UnitType::Facilities)));
}

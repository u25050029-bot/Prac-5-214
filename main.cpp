#include <exception>
#include <iostream>
#include <memory>
#include <string>
#include <utility>

#include "AccessPoint.h"
#include "AreaGroup.h"
#include "Iterator.h"
#include "LegacyAccessAdapter.h"
#include "LegacyDoorController.h"
#include "Log.h"

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

AreaComponent* findArea(AreaComponent& root, const std::string& name) {
    std::unique_ptr<Iterator<AreaComponent*>> it = root.createIterator();
    for (it->first(); !it->isDone(); it->next()) {
        if (it->currentItem()->getName() == name) {
            return it->currentItem();
        }
    }
    return nullptr;
}

void printReport(AreaComponent& area) {
    const int baseDepth = area.depth();
    std::unique_ptr<Iterator<AreaComponent*>> it = area.createIterator();
    for (it->first(); !it->isDone(); it->next()) {
        AreaComponent* current = it->currentItem();
        std::string indent(static_cast<std::size_t>(current->depth() - baseDepth) * 2, ' ');
        Log::line("Area", indent + current->getKind() + " " + current->getName() + ": " + current->statusText());
    }
}

}

int main() {
    try {
        std::unique_ptr<LegacyAccessAdapter> gateway = buildGateway();
        std::unique_ptr<AreaGroup> campus = buildCampus(*gateway);

        Log::banner("Iterator check: depth-first search and report");
        AreaComponent* engineering = findArea(*campus, "Engineering");
        OperationResult result;
        {
            Log::Scope scope;
            engineering->lock(result);
        }
        Log::line("Campus", std::to_string(result.attempted - static_cast<int>(result.failed.size())) + "/" + std::to_string(result.attempted) + " doors confirmed");

        Log::step("Restrict Library Level 1 to staff");
        OperationResult night;
        findArea(*campus, "Library Level 1")->restrictAccess(AccessLevel::StaffOnly, night);

        Log::step("Unknown area lookup");
        Log::line("Campus", findArea(*campus, "Chemistry Block") == nullptr ? "Chemistry Block is not part of the campus model" : "unexpected match");

        Log::step("Campus status");
        printReport(*campus);
    } catch (const std::exception& ex) {
        std::cerr << "CampusGuard terminated: " << ex.what() << "\n";
        return 1;
    }
    return 0;
}

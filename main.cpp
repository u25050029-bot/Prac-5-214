#include <exception>
#include <iostream>
#include <memory>
#include <utility>

#include "LegacyAccessAdapter.h"
#include "LegacyDoorController.h"
#include "Log.h"

int main() {
    try {
        std::unique_ptr<LegacyDoorController> controller(new LegacyDoorController());
        const int panels[] = {101, 102, 103, 104};
        for (int panel : panels) {
            controller->installPanel(panel);
        }
        controller->setPanelOnline(104, false);

        LegacyAccessAdapter gateway(std::move(controller));
        gateway.enrolDoor("EB-G-MAIN", 101);
        gateway.enrolDoor("EB-G-EAST", 102);
        gateway.enrolDoor("EB-1-LAB", 103);
        gateway.enrolDoor("EB-1-SERVER", 104);

        Log::banner("DX-9 adapter check");
        Log::step("1. Secure the main entrance");
        gateway.secureDoor("EB-G-MAIN");
        Log::step("2. Restrict the lab to responders");
        gateway.restrictDoor("EB-1-LAB", AccessLevel::RespondersOnly);
        Log::step("3. Server room panel is offline");
        gateway.secureDoor("EB-1-SERVER");
        Log::step("4. Door that was never enrolled");
        gateway.secureDoor("CHEM-G-MAIN");
        Log::step("5. Release the main entrance");
        gateway.releaseDoor("EB-G-MAIN");
    } catch (const std::exception& ex) {
        std::cerr << "CampusGuard terminated: " << ex.what() << "\n";
        return 1;
    }
    return 0;
}

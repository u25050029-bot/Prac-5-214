#include <exception>
#include <iostream>
#include <string>

#include "CampusGuardSystem.h"
#include "Incident.h"
#include "Log.h"

int main() {
    try {
        CampusGuardSystem system;
        IncidentRegistry& registry = system.registry();
        DispatchService& dispatch = system.dispatch();
        AccessControlService& access = system.access();
        AlertService& alerts = system.alerts();

        Log::banner("STORY 1: After-hours intrusion at Engineering");

        Log::step("1. Alarm panel reports forced entry");
        Incident* intrusion = registry.report(IncidentType::Intrusion, Severity::High, access.findArea("Engineering"), "Forced-entry alarm at the server room");

        Log::step("2. Operator dispatches security");
        dispatch.dispatchAvailable(intrusion, UnitType::Security);

        Log::step("3. Operator warns the rest of campus");
        alerts.activateAlert(access.findArea("Engineering"), AlertLevel::Warning, "Intruder reported; avoid the Engineering building", intrusion);

        Log::step("4. Operator reviews the Engineering access state");
        access.printAccessReport(access.findArea("Engineering"));

        Log::step("5. Intruder detained: contain and resolve (the coordinator stands everything down)");
        registry.contain(intrusion);
        registry.resolve(intrusion);

        Log::step("Invalid request: a second resolve of the same incident");
        try {
            registry.resolve(intrusion);
        } catch (const std::exception& ex) {
            Log::line("Operator", std::string("Request rejected: ") + ex.what());
        }
        registry.printIncidents();
        dispatch.printRoster();
    } catch (const std::exception& ex) {
        std::cerr << "CampusGuard terminated: " << ex.what() << "\n";
        return 1;
    }
    return 0;
}

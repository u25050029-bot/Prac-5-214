#include <exception>
#include <iostream>
#include <memory>
#include <utility>

#include "CampusGuardSystem.h"
#include "Incident.h"
#include "Log.h"
#include "OperatorCommands.h"

namespace {

template <typename T, typename... Args>
std::unique_ptr<Command> makeCommand(Args&&... args) {
    return std::unique_ptr<Command>(new T(std::forward<Args>(args)...));
}

Incident* runIntrusionStory(CampusGuardSystem& system) {
    IncidentRegistry& registry = system.registry();
    DispatchService& dispatch = system.dispatch();
    AccessControlService& access = system.access();
    AlertService& alerts = system.alerts();
    OperatorConsole& console = system.console();

    Log::banner("STORY 1: After-hours intrusion at Engineering (Command, Mediator, Composite, Iterator, Adapter)");

    Log::step("1. Alarm panel reports forced entry; operator registers it directly with the IncidentRegistry");
    Incident* intrusion = registry.report(IncidentType::Intrusion, Severity::High, access.findArea("Engineering"), "Forced-entry alarm at the server room");

    Log::step("2. Operator dispatches security: Command -> DispatchService -> Mediator -> Composite lockdown -> Adapter -> DX-9");
    console.submit(makeCommand<DispatchUnitCommand>(dispatch, intrusion, UnitType::Security));

    Log::step("3. Operator warns the rest of campus");
    console.submit(makeCommand<ActivateAlertCommand>(alerts, access.findArea("Engineering"), AlertLevel::Warning, "Intruder reported; avoid the Engineering building", intrusion));

    Log::step("4. Precaution: operator locks the neighbouring Student Centre");
    console.submit(makeCommand<LockAreaCommand>(access, access.findArea("Student Centre"), intrusion));

    Log::step("5. Operator sends a second security unit as backup");
    console.submit(makeCommand<DispatchUnitCommand>(dispatch, intrusion, UnitType::Security));

    Log::step("6. Alpha reports a single intruder; operator cancels the backup dispatch");
    console.cancelLast();

    Log::step("7. Intruder confirmed inside Engineering only; operator cancels the Student Centre lockdown");
    console.cancelLast();

    Log::step("8. Invalid request: operator tries to lock an area that is not in the campus model");
    console.submit(makeCommand<LockAreaCommand>(access, access.findArea("Chemistry Block"), intrusion));

    Log::step("9. Operator reviews the Engineering access state");
    access.printAccessReport(access.findArea("Engineering"));

    Log::step("10. Intruder detained: incident contained, then resolved (mediator stands everything down)");
    registry.contain(intrusion);
    registry.resolve(intrusion);

    Log::step("11. Invalid request: a second resolve of the same incident");
    try {
        registry.resolve(intrusion);
    } catch (const std::exception& ex) {
        Log::line("Operator", std::string("Request rejected: ") + ex.what());
    }
    return intrusion;
}

}

int main() {
    try {
        CampusGuardSystem system;
        runIntrusionStory(system);
    } catch (const std::exception& ex) {
        std::cerr << "CampusGuard terminated: " << ex.what() << "\n";
        return 1;
    }
    return 0;
}

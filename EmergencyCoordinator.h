#ifndef CAMPUSGUARD_EMERGENCYCOORDINATOR_H
#define CAMPUSGUARD_EMERGENCYCOORDINATOR_H

#include <map>

#include "ResponseMediator.h"
#include "Types.h"

class AccessControlService;
class AlertService;
class DispatchService;
class IncidentRegistry;

class EmergencyCoordinator : public ResponseMediator {
public:
    EmergencyCoordinator(IncidentRegistry& registry, DispatchService& dispatch, AccessControlService& access, AlertService& alerts);
    ~EmergencyCoordinator() override;

    void notify(ResponseComponent& sender, const CoordinationEvent& event) override;

private:
    typedef void (EmergencyCoordinator::*Rule)(const CoordinationEvent&);

    void onUnitDispatched(const CoordinationEvent& event);
    void onUnitRecalled(const CoordinationEvent& event);
    void onIncidentEscalated(const CoordinationEvent& event);
    void onIncidentResolved(const CoordinationEvent& event);
    void onAreaSecured(const CoordinationEvent& event);
    void onAccessFailure(const CoordinationEvent& event);
    void onEvacuationIssued(const CoordinationEvent& event);

    IncidentRegistry& registry_;
    DispatchService& dispatch_;
    AccessControlService& access_;
    AlertService& alerts_;
    std::map<EventType, Rule> rules_;
};

#endif

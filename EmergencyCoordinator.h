#ifndef CAMPUSGUARD_EMERGENCYCOORDINATOR_H
#define CAMPUSGUARD_EMERGENCYCOORDINATOR_H

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
    IncidentRegistry& registry_;
    DispatchService& dispatch_;
    AccessControlService& access_;
    AlertService& alerts_;
};

#endif

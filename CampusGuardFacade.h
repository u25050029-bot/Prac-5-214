#ifndef CAMPUSGUARD_CAMPUSGUARDFACADE_H
#define CAMPUSGUARD_CAMPUSGUARDFACADE_H

#include <string>

#include "Types.h"

class AccessControlService;
class AlertService;
class DispatchService;
class Incident;
class IncidentRegistry;

class CampusGuardFacade {
public:
    CampusGuardFacade(IncidentRegistry& registry, DispatchService& dispatch, AccessControlService& access, AlertService& alerts);
    ~CampusGuardFacade();
    CampusGuardFacade(const CampusGuardFacade&) = delete;
    CampusGuardFacade& operator=(const CampusGuardFacade&) = delete;

    Incident* respondToFire(const std::string& buildingName, const std::string& description);
    void standDown(Incident* incident);
    void situationReport() const;

private:
    void dispatchOrContinue(Incident* incident, UnitType type);

    IncidentRegistry& registry_;
    DispatchService& dispatch_;
    AccessControlService& access_;
    AlertService& alerts_;
};

#endif

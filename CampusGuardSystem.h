#ifndef CAMPUSGUARD_CAMPUSGUARDSYSTEM_H
#define CAMPUSGUARD_CAMPUSGUARDSYSTEM_H

#include <memory>

#include "AccessControlService.h"
#include "AlertService.h"
#include "AreaGroup.h"
#include "DispatchService.h"
#include "IncidentRegistry.h"
#include "LegacyAccessAdapter.h"
#include "UnitRoster.h"

class CampusGuardSystem {
public:
    CampusGuardSystem();
    ~CampusGuardSystem();
    CampusGuardSystem(const CampusGuardSystem&) = delete;
    CampusGuardSystem& operator=(const CampusGuardSystem&) = delete;

    IncidentRegistry& registry();
    DispatchService& dispatch();
    AccessControlService& access();
    AlertService& alerts();

private:
    static std::unique_ptr<LegacyAccessAdapter> buildGateway();
    static std::unique_ptr<AreaGroup> buildCampus(LegacyAccessAdapter& gateway);
    static void buildRoster(UnitRoster& roster);

    std::unique_ptr<LegacyAccessAdapter> gateway_;
    std::unique_ptr<AreaGroup> campus_;
    UnitRoster roster_;
    IncidentRegistry registry_;
    DispatchService dispatch_;
    AccessControlService access_;
    AlertService alerts_;
};

#endif

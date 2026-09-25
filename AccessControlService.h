#ifndef CAMPUSGUARD_ACCESSCONTROLSERVICE_H
#define CAMPUSGUARD_ACCESSCONTROLSERVICE_H

#include <string>

#include "Types.h"

class AreaComponent;
class Incident;
struct OperationResult;

class AccessControlService {
public:
    explicit AccessControlService(AreaComponent& campusRoot);
    ~AccessControlService();
    AccessControlService(const AccessControlService&) = delete;
    AccessControlService& operator=(const AccessControlService&) = delete;

    const std::string& getComponentName() const;

    AreaComponent* findArea(const std::string& name) const;
    bool secureArea(AreaComponent* area, Incident* context);
    bool restrictArea(AreaComponent* area, AccessLevel level, Incident* context);
    bool releaseArea(AreaComponent* area, Incident* context);
    void printAccessReport(AreaComponent* area) const;
    void printCampusReport() const;

private:
    std::string componentName_;
    static void requireArea(const AreaComponent* area);

    AreaComponent& campusRoot_;
};

#endif

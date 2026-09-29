#ifndef CAMPUSGUARD_ACCESSCONTROLSERVICE_H
#define CAMPUSGUARD_ACCESSCONTROLSERVICE_H

#include <string>

#include "ResponseComponent.h"
#include "Types.h"

class AreaComponent;
class Incident;
struct OperationResult;

class AccessControlService : public ResponseComponent {
public:
    explicit AccessControlService(AreaComponent& campusRoot);
    ~AccessControlService() override;

    AreaComponent* findArea(const std::string& name) const;
    bool secureArea(AreaComponent* area, Incident* context);
    bool restrictArea(AreaComponent* area, AccessLevel level, Incident* context);
    bool releaseArea(AreaComponent* area, Incident* context);
    void printAccessReport(AreaComponent* area) const;
    void printCampusReport() const;

private:
    bool conclude(const std::string& action, AreaComponent* area, Incident* context, const OperationResult& result, bool announceSecured);
    static void requireArea(const AreaComponent* area);

    AreaComponent& campusRoot_;
};

#endif

#ifndef CAMPUSGUARD_ACCESSPOINT_H
#define CAMPUSGUARD_ACCESSPOINT_H

#include <string>

#include "AccessControlGateway.h"
#include "AreaComponent.h"

class AccessPoint : public AreaComponent {
public:
    AccessPoint(const std::string& doorId, AccessControlGateway& gateway);
    ~AccessPoint() override;

    void lock(OperationResult& result) override;
    void unlock(OperationResult& result) override;
    void restrictAccess(AccessLevel level, OperationResult& result) override;
    int doorCount() const override;
    int securedCount() const override;
    std::string statusText() const override;

private:
    AccessControlGateway& gateway_;
    DoorState state_;
    AccessLevel level_;
};

#endif

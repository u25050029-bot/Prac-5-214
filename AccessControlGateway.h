#ifndef CAMPUSGUARD_ACCESSCONTROLGATEWAY_H
#define CAMPUSGUARD_ACCESSCONTROLGATEWAY_H

#include <string>

#include "Types.h"

class AccessControlGateway {
public:
    AccessControlGateway() {}
    virtual ~AccessControlGateway() {}
    AccessControlGateway(const AccessControlGateway&) = delete;
    AccessControlGateway& operator=(const AccessControlGateway&) = delete;

    virtual bool secureDoor(const std::string& doorId) = 0;
    virtual bool releaseDoor(const std::string& doorId) = 0;
    virtual bool restrictDoor(const std::string& doorId, AccessLevel level) = 0;
};

#endif

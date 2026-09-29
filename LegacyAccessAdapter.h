#ifndef CAMPUSGUARD_LEGACYACCESSADAPTER_H
#define CAMPUSGUARD_LEGACYACCESSADAPTER_H

#include <map>
#include <memory>
#include <string>

#include "AccessControlGateway.h"
#include "LegacyDoorController.h"

class LegacyAccessAdapter : public AccessControlGateway {
public:
    explicit LegacyAccessAdapter(std::unique_ptr<LegacyDoorController> controller);
    ~LegacyAccessAdapter() override;
    LegacyAccessAdapter(const LegacyAccessAdapter&) = delete;
    LegacyAccessAdapter& operator=(const LegacyAccessAdapter&) = delete;

    void enrolDoor(const std::string& doorId, int panelNumber);

    bool secureDoor(const std::string& doorId) override;
    bool releaseDoor(const std::string& doorId) override;
    bool restrictDoor(const std::string& doorId, AccessLevel level) override;

private:
    bool send(const std::string& operation, const std::string& doorId, char opcode, int clearance);
    static int clearanceFor(AccessLevel level);
    static std::string describeCode(int code);

    std::unique_ptr<LegacyDoorController> controller_;
    std::map<std::string, int> panelByDoor_;
};

#endif

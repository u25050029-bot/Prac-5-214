#ifndef CAMPUSGUARD_LEGACYACCESSADAPTER_H
#define CAMPUSGUARD_LEGACYACCESSADAPTER_H

#include <memory>
#include <string>
#include <utility>
#include <vector>

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
    static int clearanceFor(AccessLevel level);
    static std::string describeCode(int code);

    std::unique_ptr<LegacyDoorController> controller_;
    std::vector<std::pair<std::string, int>> doorPanels_;
};

#endif

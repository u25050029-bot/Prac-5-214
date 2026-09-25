#include "LegacyAccessAdapter.h"

#include <stdexcept>

#include "Log.h"

LegacyAccessAdapter::LegacyAccessAdapter(std::unique_ptr<LegacyDoorController> controller)
    : controller_(std::move(controller)) {
    if (!controller_) {
        throw std::invalid_argument("LegacyAccessAdapter requires a DX-9 controller");
    }
}

LegacyAccessAdapter::~LegacyAccessAdapter() {}

void LegacyAccessAdapter::enrolDoor(const std::string& doorId, int panelNumber) {
    panelByDoor_[doorId] = panelNumber;
}

bool LegacyAccessAdapter::secureDoor(const std::string& doorId) {
    return send("secureDoor", doorId, 'L', 0);
}

bool LegacyAccessAdapter::releaseDoor(const std::string& doorId) {
    return send("releaseDoor", doorId, 'U', 0);
}

bool LegacyAccessAdapter::restrictDoor(const std::string& doorId, AccessLevel level) {
    return send("restrictDoor", doorId, 'R', clearanceFor(level));
}

bool LegacyAccessAdapter::send(const std::string& operation, const std::string& doorId, char opcode, int clearance) {
    std::map<std::string, int>::const_iterator it = panelByDoor_.find(doorId);
    if (it == panelByDoor_.end()) {
        Log::line("Adapter", operation + "(\"" + doorId + "\") refused: door has no DX-9 panel mapping -> false");
        return false;
    }
    int code = controller_->transmit(it->second, opcode, clearance);
    bool success = code == LegacyDoorController::RC_OK;
    Log::line("Adapter", operation + "(\"" + doorId + "\") => transmit(" + std::to_string(it->second) + ", '" + std::string(1, opcode) + "', " + std::to_string(clearance) + ") => RC " + std::to_string(code) + " " + describeCode(code) + " => " + (success ? "true" : "false"));
    return success;
}

int LegacyAccessAdapter::clearanceFor(AccessLevel level) {
    return level == AccessLevel::RespondersOnly ? 5 : 3;
}

std::string LegacyAccessAdapter::describeCode(int code) {
    static const std::map<int, std::string> meanings = {
        {LegacyDoorController::RC_OK, "OK"},
        {LegacyDoorController::RC_BAD_OPCODE, "BAD_OPCODE"},
        {LegacyDoorController::RC_UNKNOWN_PANEL, "UNKNOWN_PANEL"},
        {LegacyDoorController::RC_PANEL_OFFLINE, "PANEL_OFFLINE"}};
    std::map<int, std::string>::const_iterator it = meanings.find(code);
    return it == meanings.end() ? std::string("UNDOCUMENTED") : it->second;
}

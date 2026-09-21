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
    doorPanels_.push_back(std::make_pair(doorId, panelNumber));
}

bool LegacyAccessAdapter::secureDoor(const std::string& doorId) {
    int panel = -1;
    for (std::size_t i = 0; i < doorPanels_.size(); ++i) {
        if (doorPanels_[i].first == doorId) {
            panel = doorPanels_[i].second;
        }
    }
    if (panel == -1) {
        Log::line("Adapter", "secureDoor(\"" + doorId + "\") refused: door has no DX-9 panel mapping -> false");
        return false;
    }
    int code = controller_->transmit(panel, 'L', 0);
    bool success = code == LegacyDoorController::RC_OK;
    Log::line("Adapter", "secureDoor(\"" + doorId + "\") => transmit(" + std::to_string(panel) + ", '" + std::string(1, 'L') + "', " + std::to_string(0) + ") => RC " + std::to_string(code) + " " + describeCode(code) + " => " + (success ? "true" : "false"));
    return success;
}

bool LegacyAccessAdapter::releaseDoor(const std::string& doorId) {
    int panel = -1;
    for (std::size_t i = 0; i < doorPanels_.size(); ++i) {
        if (doorPanels_[i].first == doorId) {
            panel = doorPanels_[i].second;
        }
    }
    if (panel == -1) {
        Log::line("Adapter", "releaseDoor(\"" + doorId + "\") refused: door has no DX-9 panel mapping -> false");
        return false;
    }
    int code = controller_->transmit(panel, 'U', 0);
    bool success = code == LegacyDoorController::RC_OK;
    Log::line("Adapter", "releaseDoor(\"" + doorId + "\") => transmit(" + std::to_string(panel) + ", '" + std::string(1, 'U') + "', " + std::to_string(0) + ") => RC " + std::to_string(code) + " " + describeCode(code) + " => " + (success ? "true" : "false"));
    return success;
}

bool LegacyAccessAdapter::restrictDoor(const std::string& doorId, AccessLevel level) {
    int panel = -1;
    for (std::size_t i = 0; i < doorPanels_.size(); ++i) {
        if (doorPanels_[i].first == doorId) {
            panel = doorPanels_[i].second;
        }
    }
    if (panel == -1) {
        Log::line("Adapter", "restrictDoor(\"" + doorId + "\") refused: door has no DX-9 panel mapping -> false");
        return false;
    }
    int code = controller_->transmit(panel, 'R', clearanceFor(level));
    bool success = code == LegacyDoorController::RC_OK;
    Log::line("Adapter", "restrictDoor(\"" + doorId + "\") => transmit(" + std::to_string(panel) + ", '" + std::string(1, 'R') + "', " + std::to_string(clearanceFor(level)) + ") => RC " + std::to_string(code) + " " + describeCode(code) + " => " + (success ? "true" : "false"));
    return success;
}

int LegacyAccessAdapter::clearanceFor(AccessLevel level) {
    return level == AccessLevel::RespondersOnly ? 3 : 5;
}

std::string LegacyAccessAdapter::describeCode(int code) {
    if (code == LegacyDoorController::RC_OK) {
        return "OK";
    }
    if (code == LegacyDoorController::RC_BAD_OPCODE) {
        return "BAD_OPCODE";
    }
    if (code == LegacyDoorController::RC_UNKNOWN_PANEL) {
        return "UNKNOWN_PANEL";
    }
    if (code == LegacyDoorController::RC_PANEL_OFFLINE) {
        return "PANEL_OFFLINE";
    }
    return "UNDOCUMENTED";
}

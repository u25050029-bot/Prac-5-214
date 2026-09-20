#include "LegacyDoorController.h"

#include <string>

#include "Log.h"

LegacyDoorController::LegacyDoorController() {}

LegacyDoorController::~LegacyDoorController() {}

void LegacyDoorController::installPanel(int panelNumber) {
    online_[panelNumber] = true;
}

void LegacyDoorController::setPanelOnline(int panelNumber, bool online) {
    online_[panelNumber] = online;
}

int LegacyDoorController::transmit(int panelNumber, char opcode, int clearance) {
    int code = RC_OK;
    if (online_.count(panelNumber) == 0) {
        code = RC_UNKNOWN_PANEL;
    } else if (online_[panelNumber] == false) {
        code = RC_PANEL_OFFLINE;
    } else if (opcode != 'L' && opcode != 'U' && opcode != 'R') {
        code = RC_BAD_OPCODE;
    }
    std::string frame = ">> PNL#" + std::to_string(panelNumber) + " OP:" + std::string(1, opcode) + " CLR:" + std::to_string(clearance) + " << RC:" + std::to_string(code);
    Log::line("Legacy DX-9", frame);
    return code;
}

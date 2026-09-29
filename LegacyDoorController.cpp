#include "LegacyDoorController.h"

#include <iomanip>
#include <sstream>
#include <stdexcept>

#include "Log.h"

LegacyDoorController::LegacyDoorController() {}

LegacyDoorController::~LegacyDoorController() {}

void LegacyDoorController::installPanel(int panelNumber) {
    online_[panelNumber] = true;
}

void LegacyDoorController::setPanelOnline(int panelNumber, bool online) {
    std::map<int, bool>::iterator it = online_.find(panelNumber);
    if (it == online_.end()) {
        throw std::invalid_argument("DX-9 panel " + std::to_string(panelNumber) + " is not installed");
    }
    it->second = online;
}

int LegacyDoorController::transmit(int panelNumber, char opcode, int clearance) {
    int code = RC_OK;
    if (opcode != 'L' && opcode != 'U' && opcode != 'R') {
        code = RC_BAD_OPCODE;
    } else {
        std::map<int, bool>::const_iterator it = online_.find(panelNumber);
        if (it == online_.end()) {
            code = RC_UNKNOWN_PANEL;
        } else if (!it->second) {
            code = RC_PANEL_OFFLINE;
        }
    }
    std::ostringstream frame;
    frame << ">> PNL#" << panelNumber << " OP:" << opcode << " CLR:" << clearance << " << RC:" << std::setw(2) << std::setfill('0') << code;
    Log::line("Legacy DX-9", frame.str());
    return code;
}

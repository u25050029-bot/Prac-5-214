#ifndef CAMPUSGUARD_LEGACYDOORCONTROLLER_H
#define CAMPUSGUARD_LEGACYDOORCONTROLLER_H

#include <map>
#include <string>

class LegacyDoorController {
public:
    enum { RC_OK = 0, RC_BAD_OPCODE = 3, RC_UNKNOWN_PANEL = 7, RC_PANEL_OFFLINE = 9 };

    LegacyDoorController();
    ~LegacyDoorController();
    LegacyDoorController(const LegacyDoorController&) = delete;
    LegacyDoorController& operator=(const LegacyDoorController&) = delete;

    void installPanel(int panelNumber);
    void setPanelOnline(int panelNumber, bool online);
    int transmit(int panelNumber, char opcode, int clearance);

private:
    std::map<int, bool> online_;
};

#endif

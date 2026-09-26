#ifndef CAMPUSGUARD_ALERTSERVICE_H
#define CAMPUSGUARD_ALERTSERVICE_H

#include <string>
#include <vector>

#include "Types.h"

class AreaComponent;
class Incident;

class AlertService {
public:
    AlertService();
    ~AlertService();
    AlertService(const AlertService&) = delete;
    AlertService& operator=(const AlertService&) = delete;

    const std::string& getComponentName() const;

    int activateAlert(AreaComponent* area, AlertLevel level, const std::string& message, Incident* incident);
    void deactivateAlert(int alertId);
    void issueEvacuation(AreaComponent* area, Incident* incident);
    void endEvacuation(AreaComponent* area);
    void notifyArea(AreaComponent* area, const std::string& message);
    void clearIncident(Incident* incident);
    void printStatus() const;

private:
    std::string componentName_;
    struct ActiveAlert {
        int id;
        AreaComponent* area;
        AlertLevel level;
        std::string message;
        Incident* incident;
    };

    struct Evacuation {
        AreaComponent* area;
        Incident* incident;
    };

    static void requireArea(const AreaComponent* area);

    std::vector<ActiveAlert> alerts_;
    std::vector<Evacuation> evacuations_;
    int nextAlertId_;
};

#endif

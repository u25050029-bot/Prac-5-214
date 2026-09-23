#ifndef CAMPUSGUARD_INCIDENT_H
#define CAMPUSGUARD_INCIDENT_H

#include <string>
#include <vector>

#include "Types.h"

class AreaComponent;
class ResponseUnit;

class Incident {
public:
    Incident(int id, IncidentType type, Severity severity, AreaComponent* location, const std::string& description);
    Incident(const Incident&) = delete;
    Incident& operator=(const Incident&) = delete;

    int getId() const;
    IncidentType getType() const;
    Severity getSeverity() const;
    IncidentStatus getStatus() const;
    AreaComponent* getLocation() const;
    const std::string& getDescription() const;
    const std::vector<ResponseUnit*>& getUnits() const;
    bool isActive() const;
    bool hasUnitOfType(UnitType type) const;

    void setSeverity(Severity severity);
    void changeStatus(IncidentStatus next);
    void attachUnit(ResponseUnit* unit);
    void detachUnit(ResponseUnit* unit);

    std::string label() const;
    std::string summary() const;

private:
    static bool isAllowed(IncidentStatus from, IncidentStatus to);

    int id_;
    IncidentType type_;
    Severity severity_;
    IncidentStatus status_;
    AreaComponent* location_;
    std::string description_;
    std::vector<ResponseUnit*> units_;
};

#endif

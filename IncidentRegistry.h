#ifndef CAMPUSGUARD_INCIDENTREGISTRY_H
#define CAMPUSGUARD_INCIDENTREGISTRY_H

#include <string>
#include <vector>

#include "Incident.h"

class AreaComponent;

class IncidentRegistry {
public:
    IncidentRegistry();
    ~IncidentRegistry();
    IncidentRegistry(const IncidentRegistry&) = delete;
    IncidentRegistry& operator=(const IncidentRegistry&) = delete;

    const std::string& getComponentName() const;

    Incident* report(IncidentType type, Severity severity, AreaComponent* location, const std::string& description);
    void markDispatched(Incident* incident);
    void revertToReported(Incident* incident);
    void contain(Incident* incident);
    void resolve(Incident* incident);
    void escalate(Incident* incident, Severity severity);
    void printIncidents() const;

private:
    std::string componentName_;
    static void require(const Incident* incident);

    std::vector<Incident*> incidents_;
    int nextId_;
};

#endif

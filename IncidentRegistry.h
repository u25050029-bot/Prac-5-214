#ifndef CAMPUSGUARD_INCIDENTREGISTRY_H
#define CAMPUSGUARD_INCIDENTREGISTRY_H

#include <memory>
#include <string>
#include <vector>

#include "Incident.h"
#include "ResponseComponent.h"

class AreaComponent;

class IncidentRegistry : public ResponseComponent {
public:
    IncidentRegistry();
    ~IncidentRegistry() override;

    Incident* report(IncidentType type, Severity severity, AreaComponent* location, const std::string& description);
    void markDispatched(Incident* incident);
    void revertToReported(Incident* incident);
    void contain(Incident* incident);
    void resolve(Incident* incident);
    void escalate(Incident* incident, Severity severity);
    void printIncidents() const;

private:
    void transition(Incident* incident, IncidentStatus next);
    static void require(const Incident* incident);

    std::vector<std::unique_ptr<Incident>> incidents_;
    int nextId_;
};

#endif

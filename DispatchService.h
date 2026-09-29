#ifndef CAMPUSGUARD_DISPATCHSERVICE_H
#define CAMPUSGUARD_DISPATCHSERVICE_H

#include "ResponseComponent.h"
#include "Types.h"

class Incident;
class ResponseUnit;
class UnitRoster;

class DispatchService : public ResponseComponent {
public:
    explicit DispatchService(UnitRoster& roster);
    ~DispatchService() override;

    ResponseUnit* dispatchAvailable(Incident* incident, UnitType type);
    void recall(ResponseUnit* unit, Incident* incident);
    void releaseUnits(Incident* incident);
    void printRoster() const;

private:
    UnitRoster& roster_;
};

#endif

#ifndef CAMPUSGUARD_DISPATCHSERVICE_H
#define CAMPUSGUARD_DISPATCHSERVICE_H

#include <string>

#include "Types.h"

class Incident;
class ResponseUnit;
class UnitRoster;

class DispatchService {
public:
    explicit DispatchService(UnitRoster& roster);
    ~DispatchService();
    DispatchService(const DispatchService&) = delete;
    DispatchService& operator=(const DispatchService&) = delete;

    const std::string& getComponentName() const;

    ResponseUnit* dispatchAvailable(Incident* incident, UnitType type);
    void recall(ResponseUnit* unit, Incident* incident);
    void releaseUnits(Incident* incident);
    void printRoster() const;

private:
    std::string componentName_;
    UnitRoster& roster_;
};

#endif

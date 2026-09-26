#ifndef CAMPUSGUARD_COORDINATIONEVENT_H
#define CAMPUSGUARD_COORDINATIONEVENT_H

#include <string>

#include "Types.h"

class AreaComponent;
class Incident;
class ResponseUnit;

struct CoordinationEvent {
    CoordinationEvent(EventType eventType, Incident* eventIncident, AreaComponent* eventArea, ResponseUnit* eventUnit, const std::string& eventDetail = std::string())
        : type(eventType), incident(eventIncident), area(eventArea), unit(eventUnit), detail(eventDetail) {}

    EventType type;
    Incident* incident;
    AreaComponent* area;
    ResponseUnit* unit;
    std::string detail;
};

#endif

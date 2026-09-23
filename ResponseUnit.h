#ifndef CAMPUSGUARD_RESPONSEUNIT_H
#define CAMPUSGUARD_RESPONSEUNIT_H

#include <string>

#include "Types.h"

class Incident;

class ResponseUnit {
public:
    ResponseUnit(const std::string& callsign, UnitType type);
    ResponseUnit(const ResponseUnit&) = delete;
    ResponseUnit& operator=(const ResponseUnit&) = delete;

    const std::string& getCallsign() const;
    UnitType getType() const;
    bool isAvailable() const;
    Incident* getAssignment() const;
    void assign(Incident* incident);
    void release();
    std::string statusText() const;

private:
    std::string callsign_;
    UnitType type_;
    Incident* assignment_;
};

#endif

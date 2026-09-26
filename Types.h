#ifndef CAMPUSGUARD_TYPES_H
#define CAMPUSGUARD_TYPES_H

#include <string>

enum class IncidentType { Fire, Medical, Intrusion };
enum class IncidentStatus { Reported, Dispatched, Contained, Resolved };
enum class Severity { Low, High, Critical };
enum class UnitType { Security, Medical, Facilities };
enum class DoorState { Unlocked, Locked, Restricted };
enum class AccessLevel { StaffOnly, RespondersOnly };
enum class AlertLevel { Advisory, Warning, Emergency };

std::string toString(IncidentType value);
std::string toString(IncidentStatus value);
std::string toString(Severity value);
std::string toString(UnitType value);
std::string toString(DoorState value);
std::string toString(AccessLevel value);
std::string toString(AlertLevel value);

#endif

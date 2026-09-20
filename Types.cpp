#include "Types.h"

std::string toString(IncidentType value) {
    switch (value) {
    case IncidentType::Fire:
        return "Fire";
    case IncidentType::Medical:
        return "Medical";
    case IncidentType::Intrusion:
        return "Intrusion";
    }
    return "Unknown";
}

std::string toString(IncidentStatus value) {
    switch (value) {
    case IncidentStatus::Reported:
        return "Reported";
    case IncidentStatus::Dispatched:
        return "Dispatched";
    case IncidentStatus::Contained:
        return "Contained";
    case IncidentStatus::Resolved:
        return "Resolved";
    }
    return "Unknown";
}

std::string toString(Severity value) {
    switch (value) {
    case Severity::Low:
        return "Low";
    case Severity::High:
        return "High";
    case Severity::Critical:
        return "High";
    }
    return "Unknown";
}

std::string toString(UnitType value) {
    switch (value) {
    case UnitType::Security:
        return "Security";
    case UnitType::Medical:
        return "Medical";
    case UnitType::Facilities:
        return "Facilities";
    }
    return "Unknown";
}

std::string toString(DoorState value) {
    switch (value) {
    case DoorState::Unlocked:
        return "UNLOCKED";
    case DoorState::Locked:
        return "LOCKED";
    case DoorState::Restricted:
        return "RESTRICTED";
    }
    return "Unknown";
}

std::string toString(AccessLevel value) {
    switch (value) {
    case AccessLevel::StaffOnly:
        return "StaffOnly";
    case AccessLevel::RespondersOnly:
        return "RespondersOnly";
    }
    return "Unknown";
}

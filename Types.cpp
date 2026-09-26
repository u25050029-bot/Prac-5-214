#include "Types.h"

#include <cstddef>

namespace {

template <typename E, std::size_t N>
std::string lookup(E value, const char* const (&names)[N]) {
    std::size_t index = static_cast<std::size_t>(value);
    return index < N ? std::string(names[index]) : std::string("Unknown");
}

}

std::string toString(IncidentType value) {
    static const char* const names[] = {"Fire", "Medical", "Intrusion"};
    return lookup(value, names);
}

std::string toString(IncidentStatus value) {
    static const char* const names[] = {"Reported", "Dispatched", "Contained", "Resolved"};
    return lookup(value, names);
}

std::string toString(Severity value) {
    static const char* const names[] = {"Low", "High", "Critical"};
    return lookup(value, names);
}

std::string toString(UnitType value) {
    static const char* const names[] = {"Security", "Medical", "Facilities"};
    return lookup(value, names);
}

std::string toString(DoorState value) {
    static const char* const names[] = {"UNLOCKED", "LOCKED", "RESTRICTED"};
    return lookup(value, names);
}

std::string toString(AccessLevel value) {
    static const char* const names[] = {"StaffOnly", "RespondersOnly"};
    return lookup(value, names);
}

std::string toString(AlertLevel value) {
    static const char* const names[] = {"Advisory", "Warning", "Emergency"};
    return lookup(value, names);
}


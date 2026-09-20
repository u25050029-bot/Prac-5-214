#include "Log.h"
#include "Types.h"

int main() {
    Log::banner("CampusGuard");
    Log::line("Boot", "campus security coordination prototype starting");
    Log::line("Types", "door states: " + toString(DoorState::Unlocked) + ", " + toString(DoorState::Locked) + ", " + toString(DoorState::Restricted));
    Log::line("Types", "units: " + toString(UnitType::Security) + ", " + toString(UnitType::Medical) + ", " + toString(UnitType::Facilities));
    return 0;
}

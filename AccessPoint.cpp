#include "AccessPoint.h"

AccessPoint::AccessPoint(const std::string& doorId, AccessControlGateway& gateway)
    : AreaComponent(doorId, "Door"), gateway_(gateway), state_(DoorState::Unlocked), level_(AccessLevel::StaffOnly) {}

AccessPoint::~AccessPoint() {}

void AccessPoint::lock(OperationResult& result) {
    ++result.attempted;
    if (gateway_.secureDoor(getName())) {
        state_ = DoorState::Locked;
    } else {
        result.failed.push_back(getName());
    }
}

void AccessPoint::unlock(OperationResult& result) {
    ++result.attempted;
    if (gateway_.releaseDoor(getName())) {
        state_ = DoorState::Unlocked;
    } else {
        result.failed.push_back(getName());
    }
}

void AccessPoint::restrictAccess(AccessLevel level, OperationResult& result) {
    ++result.attempted;
    if (gateway_.restrictDoor(getName(), level)) {
        state_ = DoorState::Restricted;
        level_ = level;
    } else {
        result.failed.push_back(getName());
    }
}

int AccessPoint::doorCount() const {
    return 1;
}

int AccessPoint::securedCount() const {
    return state_ == DoorState::Unlocked ? 0 : 1;
}

std::string AccessPoint::statusText() const {
    if (state_ == DoorState::Restricted) {
        return toString(state_) + " (" + toString(level_) + ")";
    }
    return toString(state_);
}

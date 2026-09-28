#include "OperatorCommands.h"

#include <stdexcept>

#include "AccessControlService.h"
#include "AlertService.h"
#include "AreaComponent.h"
#include "DispatchService.h"
#include "Incident.h"
#include "ResponseUnit.h"

namespace {

std::string areaName(const AreaComponent* area) {
    return area != nullptr ? area->getName() : std::string("<unknown area>");
}

std::string incidentName(const Incident* incident) {
    return incident != nullptr ? incident->label() : std::string("<no incident>");
}

}

DispatchUnitCommand::DispatchUnitCommand(DispatchService& receiver, Incident* incident, UnitType type)
    : receiver_(receiver), incident_(incident), type_(type), dispatched_(nullptr) {}

DispatchUnitCommand::~DispatchUnitCommand() {}

void DispatchUnitCommand::execute() {
    dispatched_ = receiver_.dispatchAvailable(incident_, type_);
}

void DispatchUnitCommand::undo() {
    if (dispatched_ == nullptr) {
        throw std::logic_error("nothing was dispatched by this command");
    }
    if (!incident_->isActive()) {
        throw std::logic_error("incident " + incident_->label() + " is closed; its dispatch history is final");
    }
    receiver_.recall(dispatched_, incident_);
    dispatched_ = nullptr;
}

std::string DispatchUnitCommand::describe() const {
    std::string unit = dispatched_ != nullptr ? " [" + dispatched_->getCallsign() + "]" : std::string();
    return "Dispatch " + toString(type_) + " unit to incident " + incidentName(incident_) + unit;
}

LockAreaCommand::LockAreaCommand(AccessControlService& receiver, AreaComponent* area, Incident* context)
    : receiver_(receiver), area_(area), context_(context) {}

LockAreaCommand::~LockAreaCommand() {}

void LockAreaCommand::execute() {
    receiver_.secureArea(area_, context_);
}

void LockAreaCommand::undo() {
    receiver_.releaseArea(area_, context_);
}

std::string LockAreaCommand::describe() const {
    return "Lock down " + areaName(area_);
}

RestrictAreaCommand::RestrictAreaCommand(AccessControlService& receiver, AreaComponent* area, AccessLevel level, Incident* context)
    : receiver_(receiver), area_(area), level_(level), context_(context) {}

RestrictAreaCommand::~RestrictAreaCommand() {}

void RestrictAreaCommand::execute() {
    receiver_.restrictArea(area_, level_, context_);
}

void RestrictAreaCommand::undo() {
    receiver_.releaseArea(area_, context_);
}

std::string RestrictAreaCommand::describe() const {
    return "Restrict " + areaName(area_) + " to " + toString(level_);
}

ActivateAlertCommand::ActivateAlertCommand(AlertService& receiver, AreaComponent* area, AlertLevel level, const std::string& message, Incident* context)
    : receiver_(receiver), area_(area), level_(level), message_(message), context_(context), alertId_(0) {}

ActivateAlertCommand::~ActivateAlertCommand() {}

void ActivateAlertCommand::execute() {
    alertId_ = receiver_.activateAlert(area_, level_, message_, context_);
}

void ActivateAlertCommand::undo() {
    receiver_.deactivateAlert(alertId_);
    alertId_ = 0;
}

std::string ActivateAlertCommand::describe() const {
    return "Activate " + toString(level_) + " alert in " + areaName(area_);
}

IssueEvacuationCommand::IssueEvacuationCommand(AlertService& receiver, AreaComponent* area, Incident* context)
    : receiver_(receiver), area_(area), context_(context) {}

IssueEvacuationCommand::~IssueEvacuationCommand() {}

void IssueEvacuationCommand::execute() {
    receiver_.issueEvacuation(area_, context_);
}

void IssueEvacuationCommand::undo() {
    receiver_.endEvacuation(area_);
}

std::string IssueEvacuationCommand::describe() const {
    return "Issue evacuation of " + areaName(area_) + " for incident " + incidentName(context_);
}

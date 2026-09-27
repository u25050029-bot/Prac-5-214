#ifndef CAMPUSGUARD_OPERATORCOMMANDS_H
#define CAMPUSGUARD_OPERATORCOMMANDS_H

#include <string>

#include "Command.h"
#include "Types.h"

class AccessControlService;
class AlertService;
class AreaComponent;
class DispatchService;
class Incident;
class ResponseUnit;

class DispatchUnitCommand : public Command {
public:
    DispatchUnitCommand(DispatchService& receiver, Incident* incident, UnitType type);
    ~DispatchUnitCommand() override;
    void execute() override;
    void undo() override;
    std::string describe() const override;

private:
    DispatchService& receiver_;
    Incident* incident_;
    UnitType type_;
    ResponseUnit* dispatched_;
};

class LockAreaCommand : public Command {
public:
    LockAreaCommand(AccessControlService& receiver, AreaComponent* area, Incident* context);
    ~LockAreaCommand() override;
    void execute() override;
    void undo() override;
    std::string describe() const override;

private:
    AccessControlService& receiver_;
    AreaComponent* area_;
    Incident* context_;
};

class RestrictAreaCommand : public Command {
public:
    RestrictAreaCommand(AccessControlService& receiver, AreaComponent* area, AccessLevel level, Incident* context);
    ~RestrictAreaCommand() override;
    void execute() override;
    void undo() override;
    std::string describe() const override;

private:
    AccessControlService& receiver_;
    AreaComponent* area_;
    AccessLevel level_;
    Incident* context_;
};

class ActivateAlertCommand : public Command {
public:
    ActivateAlertCommand(AlertService& receiver, AreaComponent* area, AlertLevel level, const std::string& message, Incident* context);
    ~ActivateAlertCommand() override;
    void execute() override;
    void undo() override;
    std::string describe() const override;

private:
    AlertService& receiver_;
    AreaComponent* area_;
    AlertLevel level_;
    std::string message_;
    Incident* context_;
    int alertId_;
};

class IssueEvacuationCommand : public Command {
public:
    IssueEvacuationCommand(AlertService& receiver, AreaComponent* area, Incident* context);
    ~IssueEvacuationCommand() override;
    void execute() override;
    void undo() override;
    std::string describe() const override;

private:
    AlertService& receiver_;
    AreaComponent* area_;
    Incident* context_;
};

#endif

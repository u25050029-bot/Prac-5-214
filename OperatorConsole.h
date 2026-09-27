#ifndef CAMPUSGUARD_OPERATORCONSOLE_H
#define CAMPUSGUARD_OPERATORCONSOLE_H

#include <memory>
#include <string>
#include <vector>

#include "Command.h"

class OperatorConsole {
public:
    explicit OperatorConsole(const std::string& operatorName);
    ~OperatorConsole();
    OperatorConsole(const OperatorConsole&) = delete;
    OperatorConsole& operator=(const OperatorConsole&) = delete;

    bool submit(std::unique_ptr<Command> command);
    bool cancelLast();
    void printHistory() const;

private:
    std::string operatorName_;
    std::vector<std::unique_ptr<Command>> history_;
};

#endif

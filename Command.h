#ifndef CAMPUSGUARD_COMMAND_H
#define CAMPUSGUARD_COMMAND_H

#include <string>

class Command {
public:
    Command();
    virtual ~Command();
    Command(const Command&) = delete;
    Command& operator=(const Command&) = delete;

    virtual void execute() = 0;
    virtual void undo() = 0;
    virtual std::string describe() const = 0;
};

#endif

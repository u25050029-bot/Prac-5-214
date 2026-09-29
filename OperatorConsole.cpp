#include "OperatorConsole.h"

#include <exception>

#include "Log.h"

OperatorConsole::OperatorConsole(const std::string& operatorName) : operatorName_(operatorName) {}

OperatorConsole::~OperatorConsole() {}

bool OperatorConsole::submit(std::unique_ptr<Command> command) {
    if (!command) {
        Log::line("Console", "REJECTED: empty command");
        return false;
    }
    Log::line("Console", operatorName_ + " executes: " + command->describe());
    {
        Log::Scope scope;
        try {
            command->execute();
        } catch (const std::exception& ex) {
            Log::line("Console", std::string("REJECTED, nothing recorded: ") + ex.what());
            return false;
        }
    }
    history_.push_back(std::move(command));
    Log::line("Console", "Recorded as action " + std::to_string(history_.size()) + ": " + history_.back()->describe());
    return true;
}

bool OperatorConsole::cancelLast() {
    if (history_.empty()) {
        Log::line("Console", "Cancel requested, but there is no recorded action to cancel");
        return false;
    }
    Command& last = *history_.back();
    Log::line("Console", operatorName_ + " cancels: " + last.describe());
    {
        Log::Scope scope;
        try {
            last.undo();
        } catch (const std::exception& ex) {
            Log::line("Console", std::string("CANCEL REFUSED, action kept in history: ") + ex.what());
            return false;
        }
    }
    history_.pop_back();
    Log::line("Console", "Action cancelled; " + std::to_string(history_.size()) + " action(s) remain in history");
    return true;
}

void OperatorConsole::printHistory() const {
    Log::line("Console", "Action history for " + operatorName_ + ":");
    Log::Scope scope;
    for (std::size_t i = 0; i < history_.size(); ++i) {
        Log::line("History", std::to_string(i + 1) + ". " + history_[i]->describe());
    }
}

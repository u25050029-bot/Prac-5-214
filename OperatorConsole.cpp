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
    history_.push_back(std::move(command));
    Command& current = *history_.back();
    Log::line("Console", operatorName_ + " executes: " + current.describe());
    {
        Log::Scope scope;
        try {
            current.execute();
        } catch (const std::exception& ex) {
            Log::line("Console", std::string("REJECTED: ") + ex.what());
            return false;
        }
    }
    Log::line("Console", "Recorded as action " + std::to_string(history_.size()) + ": " + history_.back()->describe());
    return true;
}

bool OperatorConsole::cancelLast() {
    if (history_.empty()) {
        Log::line("Console", "Cancel requested, but there is no recorded action to cancel");
        return false;
    }
    std::unique_ptr<Command> last = std::move(history_.back());
    history_.pop_back();
    Log::line("Console", operatorName_ + " cancels: " + last->describe());
    {
        Log::Scope scope;
        try {
            last->undo();
        } catch (const std::exception& ex) {
            Log::line("Console", std::string("CANCEL REFUSED: ") + ex.what());
            return false;
        }
    }
    Log::line("Console", "Action cancelled; " + std::to_string(history_.size()) + " action(s) remain in history");
    return true;
}

void OperatorConsole::printHistory() const {
    Log::line("Console", "Action history for " + operatorName_ + ":");
    Log::Scope scope;
    if (history_.empty()) {
        Log::line("History", "(empty)");
    }
    int number = 1;
    for (const std::unique_ptr<Command>& command : history_) {
        std::string text = std::to_string(number);
        text += ". ";
        text += command->describe();
        Log::line("History", text);
        ++number;
    }
}

#include "Log.h"

#include <iostream>

int Log::depth_ = 0;

Log::Scope::Scope() {
    ++depth_;
}

Log::Scope::~Scope() {
    --depth_;
}

void Log::line(const std::string& tag, const std::string& message) {
    std::cout << std::string(static_cast<std::size_t>(depth_) * 2, ' ') << "[" << tag << "] " << message << "\n";
}

void Log::step(const std::string& message) {
    std::cout << "\n>> " << message << "\n";
}

void Log::banner(const std::string& title) {
    std::string rule(78, '=');
    std::cout << "\n" << rule << "\n" << title << "\n" << rule << "\n";
}

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
    std::string pad;
    for (int i = 0; i < depth_; ++i) {
        pad += "  ";
    }
    std::cout << pad << "[" << tag << "] " << message << std::endl;
}

void Log::step(const std::string& message) {
    std::cout << std::endl << ">> " << message << std::endl;
}

void Log::banner(const std::string& title) {
    std::string rule;
    for (int i = 0; i < 78; ++i) {
        rule += "=";
    }
    std::cout << std::endl << rule << std::endl << title << std::endl << rule << std::endl;
}

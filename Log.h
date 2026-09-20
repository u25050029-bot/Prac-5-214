#ifndef CAMPUSGUARD_LOG_H
#define CAMPUSGUARD_LOG_H

#include <string>

class Log {
public:
    class Scope {
    public:
        Scope();
        ~Scope();
        Scope(const Scope&) = delete;
        Scope& operator=(const Scope&) = delete;
    };

    static void line(const std::string& tag, const std::string& message);
    static void step(const std::string& message);
    static void banner(const std::string& title);

private:
    static int depth_;
};

#endif

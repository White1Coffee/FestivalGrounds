#pragma once

#include <iostream>
#include <mutex>

namespace fg {
enum class LogLevel { Info, Warning, Error };

class Logger {
public:
    static void write(LogLevel level, const char* message) {
        static std::mutex mutex;
        const char* label = level == LogLevel::Info ? "INFO" : level == LogLevel::Warning ? "WARN" : "ERROR";
        std::lock_guard lock(mutex);
        std::ostream& out = level == LogLevel::Error ? std::cerr : std::clog;
        out << "[" << label << "] " << message << '\n';
    }
};
}

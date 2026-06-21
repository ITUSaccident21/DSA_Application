#pragma once

#include <chrono>
#include <ctime>
#include <string>
#include <sstream>
#include <iomanip>

class TimeUtils {
public:
    static std::time_t now() {
        return std::time(nullptr);
    }

    // Output format: "YYYY-MM-DD HH:MM:SS"
    static std::string timestampToString(std::time_t timestamp) {
        std::tm* timeinfo = std::localtime(&timestamp);
        std::ostringstream oss;
        oss << std::put_time(timeinfo, "%Y-%m-%d %H:%M:%S");
        return oss.str();
    }

    static std::time_t stringToTimestamp(const std::string& str) {
        std::tm timeinfo = {};
        std::istringstream iss(str);
        iss >> std::get_time(&timeinfo, "%Y-%m-%d %H:%M:%S");
        return std::mktime(&timeinfo);
    }

    static std::string nowString() {
        return timestampToString(now());
    }

    // Returns absolute difference in seconds.
    static long long timeDifference(std::time_t t1, std::time_t t2) {
        return std::abs(std::difftime(t1, t2));
    }

    static bool isSameDay(std::time_t t1, std::time_t t2) {
        // localtime() returns a pointer to a shared static buffer — must copy
        // the first result before calling localtime() a second time.
        std::tm tm1 = *std::localtime(&t1);
        std::tm tm2 = *std::localtime(&t2);
        return tm1.tm_year == tm2.tm_year &&
               tm1.tm_mon  == tm2.tm_mon  &&
               tm1.tm_mday == tm2.tm_mday;
    }
};

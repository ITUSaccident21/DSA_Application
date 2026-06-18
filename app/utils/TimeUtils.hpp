#pragma once

#include <chrono>
#include <ctime>
#include <string>
#include <sstream>
#include <iomanip>

// Các hàm tiện ích xử lý thời gian
class TimeUtils {
public:
    // Lấy thời gian hiện tại
    static std::time_t now() {
        return std::time(nullptr);
    }

    // Chuyển đổi timestamp thành string (định dạng: YYYY-MM-DD HH:MM:SS)
    static std::string timestampToString(std::time_t timestamp) {
        std::tm* timeinfo = std::localtime(&timestamp);
        std::ostringstream oss;
        oss << std::put_time(timeinfo, "%Y-%m-%d %H:%M:%S");
        return oss.str();
    }

    // Chuyển đổi string thành timestamp
    static std::time_t stringToTimestamp(const std::string& str) {
        std::tm timeinfo = {};
        std::istringstream iss(str);
        iss >> std::get_time(&timeinfo, "%Y-%m-%d %H:%M:%S");
        return std::mktime(&timeinfo);
    }

    // Lấy thời gian hiện tại dưới dạng string
    static std::string nowString() {
        return timestampToString(now());
    }

    // Tính khoảng thời gian giữa hai timestamp (tính bằng giây)
    static long long timeDifference(std::time_t t1, std::time_t t2) {
        return std::abs(std::difftime(t1, t2));
    }

    // Kiểm tra xem hai thời gian có cùng ngày không
    static bool isSameDay(std::time_t t1, std::time_t t2) {
        std::tm* timeinfo1 = std::localtime(&t1);
        std::tm* timeinfo2 = std::localtime(&t2);
        return timeinfo1->tm_year == timeinfo2->tm_year &&
               timeinfo1->tm_mon == timeinfo2->tm_mon &&
               timeinfo1->tm_mday == timeinfo2->tm_mday;
    }
};

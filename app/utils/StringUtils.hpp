#pragma once

#include <string>
#include <vector>
#include <sstream>

// Các hàm tiện ích xử lý string
class StringUtils {
public:
    // Tách chuỗi theo delimiter
    static std::vector<std::string> split(const std::string& str, char delimiter) {
        std::vector<std::string> tokens;
        std::stringstream ss(str);
        std::string token;
        while (std::getline(ss, token, delimiter)) {
            tokens.push_back(token);
        }
        return tokens;
    }

    // Cắt khoảng trắng đầu và cuối
    static std::string trim(const std::string& str) {
        std::size_t first = str.find_first_not_of(" \t\n\r");
        if (std::string::npos == first) {
            return str;
        }
        std::size_t last = str.find_last_not_of(" \t\n\r");
        return str.substr(first, (last - first + 1));
    }

    // Chuyển đổi string thành chữ thường
    static std::string toLower(const std::string& str) {
        std::string result = str;
        for (char& c : result) {
            c = tolower(c);
        }
        return result;
    }

    // Chuyển đổi string thành chữ hoa
    static std::string toUpper(const std::string& str) {
        std::string result = str;
        for (char& c : result) {
            c = toupper(c);
        }
        return result;
    }

    // Kiểm tra string có rỗng không (sau khi trim)
    static bool isEmpty(const std::string& str) {
        return trim(str).empty();
    }

    // Chuyển đổi int thành string
    static std::string toString(int value) {
        return std::to_string(value);
    }

    // Chuyển đổi double thành string với số chữ số thập phân
    static std::string toString(double value, int precision = 2) {
        std::ostringstream oss;
        oss.precision(precision);
        oss << std::fixed << value;
        return oss.str();
    }

    // Chuyển đổi string thành int
    static int toInt(const std::string& str, int defaultValue = 0) {
        try {
            return std::stoi(str);
        } catch (...) {
            return defaultValue;
        }
    }

    // Chuyển đổi string thành double
    static double toDouble(const std::string& str, double defaultValue = 0.0) {
        try {
            return std::stod(str);
        } catch (...) {
            return defaultValue;
        }
    }

    // Kiểm tra string có phải là số nguyên không
    static bool isInteger(const std::string& str) {
        if (str.empty()) return false;
        std::size_t start = (str[0] == '-') ? 1 : 0;
        for (std::size_t i = start; i < str.length(); ++i) {
            if (!std::isdigit(str[i])) {
                return false;
            }
        }
        return true;
    }

    // Kiểm tra string có phải là số thực không
    static bool isDouble(const std::string& str) {
        if (str.empty()) return false;
        std::size_t start = (str[0] == '-') ? 1 : 0;
        bool hasDot = false;
        for (std::size_t i = start; i < str.length(); ++i) {
            if (str[i] == '.') {
                if (hasDot) return false;
                hasDot = true;
            } else if (!std::isdigit(str[i])) {
                return false;
            }
        }
        return true;
    }
};

#pragma once

#include <string>
#include <iostream>

enum class UserRole {
    ADMIN,    // Quản lý hệ thống, menu
    STAFF,    // Nhân viên bếp, phục vụ
    CUSTOMER  // Khách hàng
};

class User {
public:
    std::string username;
    std::string password;
    std::string fullName;
    UserRole role;
    bool isActive;
    bool isVIP;

    User() : username(""), password(""), fullName(""), role(UserRole::CUSTOMER), isActive(true), isVIP(false) {}

    User(const std::string& u, const std::string& p, const std::string& name, UserRole r, bool active = true, bool vip = false)
        : username(u), password(p), fullName(name), role(r), isActive(active), isVIP(vip) {}

    std::string getRoleString() const {
        switch (role) {
            case UserRole::ADMIN:
                return "ADMIN";
            case UserRole::STAFF:
                return "STAFF";
            case UserRole::CUSTOMER:
                return "CUSTOMER";
            default:
                return "UNKNOWN";
        }
    }

    static UserRole stringToRole(const std::string& roleStr) {
        if (roleStr == "ADMIN") return UserRole::ADMIN;
        if (roleStr == "STAFF") return UserRole::STAFF;
        return UserRole::CUSTOMER;
    }

    // Định dạng: username|password|fullName|role|active
    std::string serialize() const {
        return username + "|" + password + "|" + fullName + "|" + getRoleString() + "|" + (isActive ? "1" : "0") + "|" + (isVIP ? "1" : "0");
    }

    friend std::ostream& operator<<(std::ostream& os, const User& user) {
        os << "User[" << user.username << ", " << user.fullName << ", " << user.getRoleString() << "]";
        return os;
    }
};

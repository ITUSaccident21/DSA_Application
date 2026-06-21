#pragma once

#include "../models/User.hpp"
#include "../../lib/HashTable.hpp"
#include <string>
#include <iostream>

// UserManager quản lý tài khoản sử dụng HashTable
// 
// Tại sao HashTable?
// - Cần tra cứu nhanh username (O(1) trung bình)
// - Thêm/xóa tài khoản thường xuyên
// - Không cần sắp xếp hay duyệt có thứ tự
// - HashTable cung cấp hiệu suất tối ưu cho các thao tác này

class UserManager {
private:
    ds::HashTable<std::string, User> users;  // key: username

public:
    UserManager() {}

    // Thêm người dùng mới
    bool addUser(const User& user) {
        if (users.contains(user.username)) {
            return false;  // Username already exists
        }
        users.insert(user.username, user);
        return true;
    }

    // Tìm người dùng theo username
    User* findUser(const std::string& username) {
        return users.find(username);
    }

    // Xác thực người dùng
    bool authenticate(const std::string& username, const std::string& password) {
        User* user = users.find(username);
        if (user == nullptr) {
            return false;
        }
        if (user->password != password) {
            return false;
        }
        return user->isActive;
    }

    // Cập nhật mật khẩu
    bool updatePassword(const std::string& username, const std::string& oldPassword, const std::string& newPassword) {
        User* user = users.find(username);
        if (user == nullptr || user->password != oldPassword) {
            return false;
        }
        user->password = newPassword;
        return true;
    }

    // Vô hiệu hóa tài khoản
    bool deactivateUser(const std::string& username) {
        User* user = users.find(username);
        if (user == nullptr) {
            return false;
        }
        user->isActive = false;
        return true;
    }

    // Xóa người dùng
    bool removeUser(const std::string& username) {
        return users.erase(username);
    }

    // Lấy số lượng người dùng
    std::size_t getUserCount() const {
        return users.size();
    }

    // Kiểm tra xem username đã tồn tại chưa
    bool userExists(const std::string& username) const {
        return users.contains(username);
    }

    // Xóa tất cả người dùng
    void clear() {
        users.clear();
    }
};

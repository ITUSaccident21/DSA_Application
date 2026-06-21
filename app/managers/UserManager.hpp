#pragma once

#include "../models/User.hpp"
#include "../../lib/HashTable.hpp"
#include <string>
#include <iostream>

// HashTable<username, User> — O(1) avg lookup, insert, erase by username.
class UserManager {
private:
    ds::HashTable<std::string, User> users;

public:
    UserManager() {}

    bool addUser(const User& user) {
        if (users.contains(user.username)) {
            return false;
        }
        users.insert(user.username, user);
        return true;
    }

    User* findUser(const std::string& username) {
        return users.find(username);
    }

    // Returns false for non-existent user, wrong password, or inactive account.
    // Intentionally does not distinguish the three cases (no enumerated failure).
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

    bool updatePassword(const std::string& username, const std::string& oldPassword, const std::string& newPassword) {
        User* user = users.find(username);
        if (user == nullptr || user->password != oldPassword) {
            return false;
        }
        user->password = newPassword;
        return true;
    }

    bool deactivateUser(const std::string& username) {
        User* user = users.find(username);
        if (user == nullptr) {
            return false;
        }
        user->isActive = false;
        return true;
    }

    bool removeUser(const std::string& username) {
        return users.erase(username);
    }

    std::vector<User> getAllUsers() const {
        std::vector<User> result;
        users.forEach([&](const std::string& key, const User& user) {
            result.push_back(user);
        });
        return result;
    }

    std::size_t getUserCount() const {
        return users.size();
    }

    bool userExists(const std::string& username) const {
        return users.contains(username);
    }

    void clear() {
        users.clear();
    }
};

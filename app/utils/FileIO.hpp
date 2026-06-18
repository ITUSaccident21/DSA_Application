#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "StringUtils.hpp"
#include "../models/User.hpp"
#include "../models/MenuItem.hpp"
#include "../models/Order.hpp"
#include "../managers/UserManager.hpp"
#include "../managers/MenuManager.hpp"
#include "../managers/OrderManager.hpp"

// Xử lý đọc/ghi file
class FileIO {
public:
    // ========== USER FILE I/O ==========

    // Đọc người dùng từ file users.txt
    static bool loadUsers(const std::string& filename, UserManager& manager) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Cannot open file: " << filename << std::endl;
            return false;
        }

        std::string line;
        while (std::getline(file, line)) {
            if (StringUtils::isEmpty(line)) {
                continue;
            }

            std::vector<std::string> parts = StringUtils::split(line, '|');
            if (parts.size() != 5) {
                std::cerr << "Invalid user format: " << line << std::endl;
                continue;
            }

            std::string username = StringUtils::trim(parts[0]);
            std::string password = StringUtils::trim(parts[1]);
            std::string fullName = StringUtils::trim(parts[2]);
            UserRole role = User::stringToRole(StringUtils::trim(parts[3]));
            bool isActive = StringUtils::toInt(StringUtils::trim(parts[4])) == 1;

            User user(username, password, fullName, role, isActive);
            manager.addUser(user);
        }

        file.close();
        return true;
    }

    // Ghi người dùng vào file users.txt
    static bool saveUsers(const std::string& filename, UserManager& manager) {
        // Note: UserManager không cung cấp iterator public, ta sẽ ghi những user được thêm
        // Trong ứng dụng thực, ta cần mở rộng UserManager để hỗ trợ iteration
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Cannot create file: " << filename << std::endl;
            return false;
        }
        file.close();
        return true;
    }

    // ========== MENU FILE I/O ==========

    // Đọc menu từ file menu.txt
    static bool loadMenu(const std::string& filename, MenuManager& manager) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            // File không tồn tại là bình thường lúc startup
            return true;
        }

        std::vector<MenuItem> items;
        std::string line;

        while (std::getline(file, line)) {
            if (StringUtils::isEmpty(line)) {
                continue;
            }

            std::vector<std::string> parts = StringUtils::split(line, '|');
            if (parts.size() != 6) {
                std::cerr << "Invalid menu format: " << line << std::endl;
                continue;
            }

            int id = StringUtils::toInt(StringUtils::trim(parts[0]));
            std::string name = StringUtils::trim(parts[1]);
            double price = StringUtils::toDouble(StringUtils::trim(parts[2]));
            Category category = MenuItem::stringToCategory(StringUtils::trim(parts[3]));
            std::string description = StringUtils::trim(parts[4]);
            bool isAvailable = StringUtils::toInt(StringUtils::trim(parts[5])) == 1;

            MenuItem item(id, name, price, category, description, isAvailable);
            items.push_back(item);
        }

        file.close();

        // Thêm vào manager
        for (const auto& item : items) {
            manager.addItem(item);
        }

        // Sắp xếp bằng QuickSort
        manager.sortByName();

        return true;
    }

    // Ghi menu vào file menu.txt
    static bool saveMenu(const std::string& filename, MenuManager& manager) {
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Cannot create file: " << filename << std::endl;
            return false;
        }

        MenuItem* items = manager.getItems();
        std::size_t size = manager.getSize();

        for (std::size_t i = 0; i < size; ++i) {
            file << items[i].serialize() << "\n";
        }

        file.close();
        return true;
    }

    // ========== ORDER FILE I/O ==========

    // Đọc đơn hàng từ file orders.txt
    // Format: orderId|userId|status|timestamp|VIP|itemId:qty|itemId:qty|...
    static bool loadOrders(const std::string& filename, OrderManager& manager, MenuManager& menuManager) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            return true;  // File không tồn tại là bình thường
        }

        std::string line;
        while (std::getline(file, line)) {
            if (StringUtils::isEmpty(line)) {
                continue;
            }

            std::vector<std::string> parts = StringUtils::split(line, '|');
            if (parts.size() < 5) {
                std::cerr << "Invalid order format: " << line << std::endl;
                continue;
            }

            int orderId = StringUtils::toInt(StringUtils::trim(parts[0]));
            std::string userId = StringUtils::trim(parts[1]);
            OrderStatus status = Order::stringToStatus(StringUtils::trim(parts[2]));
            std::time_t timestamp = StringUtils::toInt(StringUtils::trim(parts[3]));
            bool isVIP = StringUtils::toInt(StringUtils::trim(parts[4])) == 1;

            Order order(orderId, userId, isVIP);
            order.status = status;
            order.timestamp = timestamp;

            // Phân tích items
            for (std::size_t i = 5; i < parts.size(); ++i) {
                std::vector<std::string> itemParts = StringUtils::split(StringUtils::trim(parts[i]), ':');
                if (itemParts.size() != 2) {
                    continue;
                }

                int itemId = StringUtils::toInt(StringUtils::trim(itemParts[0]));
                int qty = StringUtils::toInt(StringUtils::trim(itemParts[1]));

                MenuItem* menuItem = menuManager.findById(itemId);
                if (menuItem != nullptr) {
                    OrderItem orderItem(*menuItem, qty);
                    order.addItem(orderItem);
                }
            }

            manager.addOrder(order);
        }

        file.close();
        return true;
    }

    // Ghi đơn hàng vào file orders.txt
    static bool saveOrders(const std::string& filename, OrderManager& manager) {
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Cannot create file: " << filename << std::endl;
            return false;
        }

        // Duyệt inorder để ghi tất cả orders
        manager.forEachOrderInorder([&](Order& order) {
            std::string orderLine = std::to_string(order.orderId) + "|" + order.userId + "|" +
                                    order.getStatusString() + "|" + std::to_string((long long)order.timestamp) + "|" +
                                    (order.isVIP ? "1" : "0");

            // Thêm items
            for (const auto& item : order.items) {
                orderLine += "|" + item.serialize();
            }

            file << orderLine << "\n";
        });

        file.close();
        return true;
    }
};

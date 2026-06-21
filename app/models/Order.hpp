#pragma once

#include "../../lib/LinkedList.hpp"
#include "OrderItem.hpp"
#include <string>
#include <ctime>
#include <iostream>

enum class OrderStatus {
    PENDING,       // Chờ xác nhận
    CONFIRMED,     // Đã xác nhận
    COOKING,       // Đang nấu
    READY,         // Sẵn sàng
    DELIVERED,     // Đã giao
    CANCELLED      // Đã hủy
};

class Order {
public:
    int orderId;
    std::string userId;
    ds::LinkedList<OrderItem> items;  // Sử dụng LinkedList để lưu các OrderItem
    OrderStatus status;
    std::time_t timestamp;
    double totalPrice;
    bool isVIP;  // Để ưu tiên trong hàng chờ bếp

    Order() : orderId(0), userId(""), status(OrderStatus::PENDING), timestamp(0), totalPrice(0.0), isVIP(false) {}

    Order(int id, const std::string& uid, bool vip = false)
        : orderId(id), userId(uid), status(OrderStatus::PENDING), timestamp(std::time(nullptr)), totalPrice(0.0),
          isVIP(vip) {}

    // Thêm OrderItem vào danh sách
    void addItem(const OrderItem& item) {
        items.push_back(item);
        totalPrice += item.subtotal;
    }

    // Tính tổng giá trị từ items
    void recalculateTotal() {
        totalPrice = 0.0;
        for (const auto& item : items) {
            totalPrice += item.subtotal;
        }
    }

    // Lấy số lượng items
    std::size_t getItemCount() const {
        return items.size();
    }

    std::string getStatusString() const {
        switch (status) {
            case OrderStatus::PENDING:
                return "PENDING";
            case OrderStatus::CONFIRMED:
                return "CONFIRMED";
            case OrderStatus::COOKING:
                return "COOKING";
            case OrderStatus::READY:
                return "READY";
            case OrderStatus::DELIVERED:
                return "DELIVERED";
            case OrderStatus::CANCELLED:
                return "CANCELLED";
            default:
                return "UNKNOWN";
        }
    }

    static OrderStatus stringToStatus(const std::string& statusStr) {
        if (statusStr == "PENDING") return OrderStatus::PENDING;
        if (statusStr == "CONFIRMED") return OrderStatus::CONFIRMED;
        if (statusStr == "COOKING") return OrderStatus::COOKING;
        if (statusStr == "READY") return OrderStatus::READY;
        if (statusStr == "DELIVERED") return OrderStatus::DELIVERED;
        if (statusStr == "CANCELLED") return OrderStatus::CANCELLED;
        return OrderStatus::PENDING;
    }

    // Để sử dụng trong BST - extract key từ Order
    int getOrderId() const { return orderId; }

    friend std::ostream& operator<<(std::ostream& os, const Order& order) {
        os << "Order[#" << order.orderId << ", User: " << order.userId << ", Status: " << order.getStatusString()
           << ", Total: " << order.totalPrice << " VND, Items: " << order.getItemCount() << "]";
        return os;
    }
};

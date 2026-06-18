#pragma once

#include "MenuItem.hpp"

// Đơn vị trong một order - chứa MenuItem và số lượng
class OrderItem {
public:
    MenuItem item;
    int quantity;
    double subtotal;  // Tính trước cho hiệu suất

    OrderItem() : item(), quantity(0), subtotal(0.0) {}

    OrderItem(const MenuItem& menuItem, int qty)
        : item(menuItem), quantity(qty), subtotal(menuItem.price * qty) {}

    // Để sử dụng trong LinkedList
    OrderItem(const OrderItem& other) : item(other.item), quantity(other.quantity), subtotal(other.subtotal) {}

    OrderItem& operator=(const OrderItem& other) {
        if (this != &other) {
            item = other.item;
            quantity = other.quantity;
            subtotal = other.subtotal;
        }
        return *this;
    }

    std::string serialize() const {
        return std::to_string(item.id) + ":" + std::to_string(quantity);
    }

    friend std::ostream& operator<<(std::ostream& os, const OrderItem& oi) {
        os << oi.item.name << " x" << oi.quantity << " = " << oi.subtotal << " VND";
        return os;
    }
};

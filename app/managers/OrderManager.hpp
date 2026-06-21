#pragma once

#include "../models/Order.hpp"
#include "../../lib/BST.hpp"
#include "../../lib/Detail.hpp"
#include <iostream>
#include <vector>

// Key extractor: maps Order → orderId for BST.
struct OrderKey {
    int operator()(const Order& order) const {
        return order.orderId;
    }
};

class OrderManager {
private:
    // BST<Order, key=orderId>: O(log n) avg find/insert, O(n) inorder.
    // Warning: orderId is generated monotonically (1001, 1002, ...),
    // which degenerates this BST to O(n) height (right-skewed tree).
    ds::BST<Order, std::less<int>, OrderKey> orders;

public:
    OrderManager() {}

    bool addOrder(const Order& order) {
        orders.insert(order);
        return true;
    }

    Order* findOrder(int orderId) {
        return orders.find(orderId);
    }

    bool updateOrderStatus(int orderId, OrderStatus newStatus) {
        Order* order = orders.find(orderId);
        if (order != nullptr) {
            order->status = newStatus;
            return true;
        }
        return false;
    }

    // O(n): full BST traversal.
    std::vector<Order*> getOrdersByUser(const std::string& userId) {
        std::vector<Order*> result;
        orders.inorder([&](Order& order) {
            if (order.userId == userId) {
                result.push_back(&order);
            }
        });
        return result;
    }

    // O(n): full BST traversal.
    std::vector<Order*> getOrdersByStatus(OrderStatus status) {
        std::vector<Order*> result;
        orders.inorder([&](Order& order) {
            if (order.status == status) {
                result.push_back(&order);
            }
        });
        return result;
    }

    template <typename Func>
    void forEachOrderInorder(Func callback) {
        orders.inorder(callback);
    }

    double getTotalRevenue() {
        double total = 0.0;
        orders.inorder([&](Order& order) {
            if (order.status == OrderStatus::DELIVERED) {
                total += order.totalPrice;
            }
        });
        return total;
    }

    double getTotalRevenueByStatus(OrderStatus status) {
        double total = 0.0;
        orders.inorder([&](Order& order) {
            if (order.status == status) {
                total += order.totalPrice;
            }
        });
        return total;
    }

    int countOrdersByStatus(OrderStatus status) {
        int count = 0;
        orders.inorder([&](Order& order) {
            if (order.status == status) {
                count++;
            }
        });
        return count;
    }

    std::size_t getOrderCount() const {
        return orders.size();
    }

    bool removeOrder(int orderId) {
        return orders.remove(orderId);
    }

    void clear() {
        orders.clear();
    }
};

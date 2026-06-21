#pragma once

#include "../models/Order.hpp"
#include "../../lib/BST.hpp"
#include "../../lib/Detail.hpp"
#include <iostream>
#include <vector>

// OrderManager quản lý đơn hàng sử dụng BST
//
// Tại sao BST?
// - Cần tra cứu nhanh theo orderId
// - Cần duyệt toàn bộ đơn hàng theo thứ tự (inorder) để xem lịch sử
// - Cần thống kê và phân tích (tính tổng, đếm theo status)
// - BST cung cấp cân bằng giữa tìm kiếm (O(log n)) và duyệt có thứ tự (O(n))

// Helper struct: Extract orderId từ Order
struct OrderKey {
    int operator()(const Order& order) const {
        return order.orderId;
    }
};

class OrderManager {
private:
    // BST với key = orderId, compare = std::less<int>
    ds::BST<Order, std::less<int>, OrderKey> orders;

public:
    OrderManager() {}

    // Thêm đơn hàng mới
    bool addOrder(const Order& order) {
        orders.insert(order);
        return true;
    }

    // Tìm đơn hàng theo ID
    Order* findOrder(int orderId) {
        return orders.find(orderId);
    }

    // Cập nhật trạng thái đơn hàng
    bool updateOrderStatus(int orderId, OrderStatus newStatus) {
        Order* order = orders.find(orderId);
        if (order != nullptr) {
            order->status = newStatus;
            return true;
        }
        return false;
    }

    // Lấy tất cả đơn hàng của một người dùng
    std::vector<Order*> getOrdersByUser(const std::string& userId) {
        std::vector<Order*> result;
        orders.inorder([&](Order& order) {
            if (order.userId == userId) {
                result.push_back(&order);
            }
        });
        return result;
    }

    // Lấy đơn hàng theo trạng thái
    std::vector<Order*> getOrdersByStatus(OrderStatus status) {
        std::vector<Order*> result;
        orders.inorder([&](Order& order) {
            if (order.status == status) {
                result.push_back(&order);
            }
        });
        return result;
    }

    // Duyệt inorder và gọi callback (cho lịch sử)
    template <typename Func>
    void forEachOrderInorder(Func callback) {
        orders.inorder(callback);
    }

    // Tính tổng doanh thu
    double getTotalRevenue() {
        double total = 0.0;
        orders.inorder([&](Order& order) {
            if (order.status == OrderStatus::DELIVERED) {
                total += order.totalPrice;
            }
        });
        return total;
    }

    // Tính tổng doanh thu theo trạng thái
    double getTotalRevenueByStatus(OrderStatus status) {
        double total = 0.0;
        orders.inorder([&](Order& order) {
            if (order.status == status) {
                total += order.totalPrice;
            }
        });
        return total;
    }

    // Đếm số đơn hàng theo trạng thái
    int countOrdersByStatus(OrderStatus status) {
        int count = 0;
        orders.inorder([&](Order& order) {
            if (order.status == status) {
                count++;
            }
        });
        return count;
    }

    // Lấy số lượng đơn hàng
    std::size_t getOrderCount() const {
        return orders.size();
    }

    // Xóa đơn hàng
    bool removeOrder(int orderId) {
        return orders.remove(orderId);
    }

    // Xóa tất cả đơn hàng
    void clear() {
        orders.clear();
    }
};

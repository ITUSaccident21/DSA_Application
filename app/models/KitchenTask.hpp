#pragma once

#include <ctime>
#include <iostream>

// Nhiệm vụ nấu ăn trong hàng chờ bếp
// Được sắp xếp bằng PriorityQueue theo: VIP → thời gian đặt → trạng thái
class KitchenTask {
public:
    int orderId;
    int itemCount;
    std::time_t orderTime;
    bool isVIP;
    int priority;  // 0=Pending, 1=Confirmed, 2=Cooking

    KitchenTask() : orderId(0), itemCount(0), orderTime(0), isVIP(false), priority(0) {}

    KitchenTask(int id, int items, std::time_t time, bool vip, int prio)
        : orderId(id), itemCount(items), orderTime(time), isVIP(vip), priority(prio) {}

    // Để so sánh trong PriorityQueue
    // Return true nếu this có độ ưu tiên cao hơn other
    bool operator>(const KitchenTask& other) const {
        // VIP first
        if (isVIP != other.isVIP) {
            return isVIP > other.isVIP;
        }
        // Then by priority (lower priority value = higher precedence)
        if (priority != other.priority) {
            return priority < other.priority;
        }
        // Then by order time (earlier = higher precedence)
        return orderTime < other.orderTime;
    }

    bool operator<(const KitchenTask& other) const { return other > *this; }

    bool operator==(const KitchenTask& other) const { return orderId == other.orderId; }

    friend std::ostream& operator<<(std::ostream& os, const KitchenTask& task) {
        os << "KitchenTask[Order#" << task.orderId << ", Items: " << task.itemCount << ", VIP: " << (task.isVIP ? "Yes" : "No")
           << "]";
        return os;
    }
};

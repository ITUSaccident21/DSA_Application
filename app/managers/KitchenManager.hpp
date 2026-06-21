#pragma once

#include "../models/KitchenTask.hpp"
#include "../../lib/PriorityQueue.hpp"
#include <iostream>
#include <vector>

// KitchenManager quản lý hàng chờ bếp sử dụng PriorityQueue
//
// Tại sao PriorityQueue?
// - Cần ưu tiên: VIP > thời gian đặt > trạng thái
// - Lấy công việc có độ ưu tiên cao nhất (O(1))
// - Thêm công việc mới (O(log n))
// - Không cần tìm kiếm hay duyệt - chỉ cần lấy top task
// - PriorityQueue là cấu trúc tối ưu cho use case này

// Custom comparator: KitchenTask với độ ưu tiên cao hơn sẽ ở trên
struct KitchenTaskComparator {
    bool operator()(const KitchenTask& a, const KitchenTask& b) const {
        // a > b means a has higher priority (should be on top)
        return a > b;
    }
};

class KitchenManager {
private:
    ds::PriorityQueue<KitchenTask, KitchenTaskComparator> queue;

public:
    KitchenManager() {}

    // Thêm công việc vào hàng chờ
    void addTask(const KitchenTask& task) {
        queue.push(task);
    }

    // Lấy công việc có độ ưu tiên cao nhất
    const KitchenTask& getTopTask() const {
        return queue.top();
    }

    // Lấy và xóa công việc có độ ưu tiên cao nhất
    KitchenTask popTask() {
        KitchenTask top = queue.top();
        queue.pop();
        return top;
    }

    // Kiểm tra xem hàng chờ có công việc không
    bool isEmpty() const {
        return queue.empty();
    }

    // Lấy số lượng công việc đang chờ
    std::size_t getQueueSize() const {
        return queue.size();
    }

    // Xóa tất cả công việc
    void clear() {
        queue.clear();
    }

    // Lấy danh sách công việc (để hiển thị)
    // Note: Do PriorityQueue không cung cấp iterator, ta phải extract toàn bộ
    std::vector<KitchenTask> getAllTasks() {
        std::vector<KitchenTask> tasks;
        ds::PriorityQueue<KitchenTask, KitchenTaskComparator> tempQueue = queue;
        
        while (!tempQueue.empty()) {
            tasks.push_back(tempQueue.top());
            tempQueue.pop();
        }
        
        return tasks;
    }
};

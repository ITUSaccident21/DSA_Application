#pragma once

#include "../models/KitchenTask.hpp"
#include "../../lib/PriorityQueue.hpp"
#include <iostream>
#include <vector>

// Comparator for ds::PriorityQueue<KitchenTask>.
// Semantics: compare(a, b) = true  →  a sinks (b has higher priority).
// We want higher-priority tasks to float to top, so:
//   compare(a, b) = true  ⟺  a is less preferred than b  ⟺  b > a
struct KitchenTaskComparator {
    bool operator()(const KitchenTask& a, const KitchenTask& b) const {
        return b > a;  // b > a: b ưu tiên hơn a => a bị đẩy xuống => b nổi lên
    }
};

class KitchenManager {
private:
    ds::PriorityQueue<KitchenTask, KitchenTaskComparator> queue;

public:
    KitchenManager() {}

    void addTask(const KitchenTask& task) {
        queue.push(task);
    }

    // Precondition: !isEmpty(). Undefined behavior if called on empty queue.
    const KitchenTask& getTopTask() const {
        return queue.top();
    }

    // Precondition: !isEmpty().
    KitchenTask popTask() {
        KitchenTask top = queue.top();
        queue.pop();
        return top;
    }

    bool isEmpty() const {
        return queue.empty();
    }

    std::size_t getQueueSize() const {
        return queue.size();
    }

    void clear() {
        queue.clear();
    }

    // Returns tasks in priority order via a temporary copy — O(n log n).
    // PriorityQueue has no iterator; a full copy-and-drain is the only option.
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

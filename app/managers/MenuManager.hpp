#pragma once

#include "../models/MenuItem.hpp"
#include "../../lib/Algorithms.hpp"
#include <vector>
#include <iostream>

class MenuManager {
private:
    MenuItem* items;
    std::size_t size;
    std::size_t capacity;

    void resize(std::size_t newCapacity) {
        if (newCapacity <= capacity) {
            return;
        }
        MenuItem* newItems = new MenuItem[newCapacity];
        for (std::size_t i = 0; i < size; ++i) {
            newItems[i] = items[i];
        }
        delete[] items;
        items = newItems;
        capacity = newCapacity;
    }

public:
    MenuManager() : items(nullptr), size(0), capacity(0) { resize(16); }

    ~MenuManager() {
        if (items != nullptr) {
            delete[] items;
        }
    }

    void addItem(const MenuItem& item) {
        if (size >= capacity) {
            resize(capacity * 2);
        }
        items[size++] = item;
    }

    // Thêm nhiều items và sắp xếp lại toàn bộ bằng InsertionSort.
    // Dùng InsertionSort (không phải QuickSort) vì mảng đã gần có thứ tự
    // trước khi thêm — InsertionSort đạt O(n + k) trên dữ liệu nearly-sorted
    // (n = tổng items, k = số items mới), tốt hơn QuickSort O(n log n) trong
    // trường hợp này.
    void addItemsAndSort(const MenuItem* newItems, std::size_t count) {
        for (std::size_t i = 0; i < count; ++i) {
            if (size >= capacity) {
                resize(capacity * 2);
            }
            items[size++] = newItems[i];
        }

        ds::insertionSort(items, items + size, [](const MenuItem& a, const MenuItem& b) {
            return a.name < b.name;
        });
    }

    // Sắp xếp bằng QuickSort theo tên (khi load từ file)
    void sortByName() {
        ds::quickSort(items, items + size, [](const MenuItem& a, const MenuItem& b) {
            return a.name < b.name;
        });
    }

    MenuItem* findById(int id) {
        for (std::size_t i = 0; i < size; ++i) {
            if (items[i].id == id) {
                return &items[i];
            }
        }
        return nullptr;
    }

    // Tìm item theo tên (binary search sau khi sắp xếp)
    MenuItem* findByName(const std::string& name) {
        // Manual binary search to avoid <algorithm>
        std::size_t left = 0;
        std::size_t right = (size == 0 ? 0 : size - 1);
        while (left <= right && size > 0) {
            std::size_t mid = left + (right - left) / 2;
            if (items[mid].name == name) {
                return &items[mid];
            } else if (items[mid].name < name) {
                left = mid + 1;
                if (left == 0) break; // overflow guard
            } else {
                if (mid == 0) break;
                right = mid - 1;
            }
        }
        return nullptr;
    }

    std::vector<MenuItem*> findByCategory(Category cat) {
        std::vector<MenuItem*> result;
        for (std::size_t i = 0; i < size; ++i) {
            if (items[i].category == cat && items[i].isAvailable) {
                result.push_back(&items[i]);
            }
        }
        return result;
    }

    bool updateItem(int id, const MenuItem& newItem) {
        MenuItem* item = findById(id);
        if (item != nullptr) {
            *item = newItem;
            item->id = id;  // Giữ ID không đổi
            return true;
        }
        return false;
    }

    // Xóa item (đánh dấu là không available)
    bool removeItem(int id) {
        for (std::size_t i = 0; i < size; ++i) {
            if (items[i].id == id) {
                for (std::size_t j = i + 1; j < size; ++j) {
                    items[j - 1] = items[j];
                }
                --size;
                return true;
            }
        }
        return false;
    }

    MenuItem* getItems() { return items; }
    std::size_t getSize() const { return size; }

    std::vector<MenuItem*> getAvailableItems() {
        std::vector<MenuItem*> result;
        for (std::size_t i = 0; i < size; ++i) {
            if (items[i].isAvailable) {
                result.push_back(&items[i]);
            }
        }
        return result;
    }

    // Sets size=0 but keeps the allocated buffer (capacity unchanged).
    void clear() {
        size = 0;
    }
};

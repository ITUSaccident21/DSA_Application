#pragma once

#include "../models/MenuItem.hpp"
#include "../../lib/Algorithms.hpp"
#include <vector>
#include <iostream>

// MenuManager quản lý menu sử dụng mảng động
//
// Tại sao mảng động thay vì BST?
// - Menu thường được duyệt từ đầu đến cuối (hiển thị cho khách)
// - Sắp xếp theo tên để dễ tìm kiếm
// - Ít thêm/xóa nhưng thường load một lần rồi đọc nhiều lần
// - Mảng động cung cấp truy cập O(1) và duyệt tuyến tính O(n)
//
// Chiến lược sắp xếp:
// - Load từ file: QuickSort (hiệu suất tổng thể tốt nhất)
// - Thêm 1-2 mục mới: InsertionSort (không cần sắp xếp toàn bộ lại)

class MenuManager {
private:
    MenuItem* items;           // Mảng động
    std::size_t size;          // Số lượng item hiện tại
    std::size_t capacity;      // Dung lượng của mảng

    // Mở rộng dung lượng khi cần
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

    // Thêm menu item
    void addItem(const MenuItem& item) {
        if (size >= capacity) {
            resize(capacity * 2);
        }
        items[size++] = item;
    }

    // Thêm nhiều items và sắp xếp bằng InsertionSort
    // (thay vì QuickSort toàn bộ, chỉ chèn những item mới)
    void addItemsAndSort(const MenuItem* newItems, std::size_t count) {
        // Thêm tất cả items mới
        for (std::size_t i = 0; i < count; ++i) {
            if (size >= capacity) {
                resize(capacity * 2);
            }
            items[size++] = newItems[i];
        }

        // Sắp xếp lại bằng InsertionSort theo tên
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

    // Tìm item theo ID
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

    // Tìm items theo category
    std::vector<MenuItem*> findByCategory(Category cat) {
        std::vector<MenuItem*> result;
        for (std::size_t i = 0; i < size; ++i) {
            if (items[i].category == cat && items[i].isAvailable) {
                result.push_back(&items[i]);
            }
        }
        return result;
    }

    // Cập nhật item
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
        MenuItem* item = findById(id);
        if (item != nullptr) {
            item->isAvailable = false;
            return true;
        }
        return false;
    }

    // Lấy tất cả items
    MenuItem* getItems() { return items; }
    std::size_t getSize() const { return size; }

    // Lấy danh sách items có sẵn
    std::vector<MenuItem*> getAvailableItems() {
        std::vector<MenuItem*> result;
        for (std::size_t i = 0; i < size; ++i) {
            if (items[i].isAvailable) {
                result.push_back(&items[i]);
            }
        }
        return result;
    }

    // Xóa tất cả items
    void clear() {
        size = 0;
    }
};

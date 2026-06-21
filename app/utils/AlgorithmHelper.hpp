#pragma once

#include <vector>
#include "../models/MenuItem.hpp"
#include "../models/Order.hpp"
#include "../../lib/Algorithms.hpp"

// Các hàm sử dụng các thuật toán sắp xếp khác nhau
class AlgorithmHelper {
public:
    // HeapSort: Sắp xếp menu items theo giá (từ cao đến thấp)
    static std::vector<MenuItem> sortMenuByPriceHeap(const std::vector<MenuItem*>& items) {
        std::vector<MenuItem> result;
        for (const auto& item : items) {
            result.push_back(*item);
        }

        // Sắp xếp theo giá giảm dần
        ds::heapSort(result.begin(), result.end(), [](const MenuItem& a, const MenuItem& b) {
            return a.price > b.price;  // Giá cao đến thấp
        });

        return result;
    }

    // MergeSort: Sắp xếp đơn hàng theo thời gian (mới nhất đến cũ nhất)
    static std::vector<Order*> sortOrdersByTimeNewest(const std::vector<Order*>& orders) {
        std::vector<Order*> result = orders;

        // Sắp xếp theo thời gian giảm dần (mới nhất trước)
        ds::mergeSort(result.begin(), result.end(), [](Order* const& a, Order* const& b) {
            return a->timestamp > b->timestamp;
        });

        return result;
    }

    // SelectionSort: Chọn các item có giá cao nhất
    static std::vector<MenuItem> getTopPriceItemsSelection(const std::vector<MenuItem*>& items, std::size_t topCount) {
        std::vector<MenuItem> result;
        if (items.empty()) {
            return result;
        }

        // Copy items
        for (const auto& item : items) {
            result.push_back(*item);
        }

        // Sắp xếp bằng SelectionSort theo giá giảm dần
        ds::selectionSort(result.begin(), result.end(), [](const MenuItem& a, const MenuItem& b) {
            return a.price > b.price;
        });

        // Trả về topCount items
        if (topCount > result.size()) {
            topCount = result.size();
        }
        return std::vector<MenuItem>(result.begin(), result.begin() + topCount);
    }

    // BubbleSort: Sắp xếp các item theo tên (A-Z) - sử dụng cho danh sách nhỏ
    static std::vector<MenuItem> sortMenuByNameBubble(const std::vector<MenuItem*>& items) {
        std::vector<MenuItem> result;
        for (const auto& item : items) {
            result.push_back(*item);
        }

        // Sắp xếp theo tên bằng BubbleSort
        ds::bubbleSort(result.begin(), result.end(), [](const MenuItem& a, const MenuItem& b) {
            return a.name < b.name;
        });

        return result;
    }

    // QuickSort: Đã được sử dụng trong MenuManager::sortByName()
    // InsertionSort: Đã được sử dụng trong MenuManager::addItemsAndSort()
};

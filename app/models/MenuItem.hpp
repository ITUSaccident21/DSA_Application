#pragma once

#include <string>
#include <iostream>
#include <cstring>

enum class Category {
    APPETIZER,    // Khai vị
    MAIN_COURSE,  // Món chính
    SOUP,         // Canh
    DESSERT,      // Tráng miệng
    BEVERAGE      // Đồ uống
};

class MenuItem {
public:
    int id;
    std::string name;
    double price;
    Category category;
    std::string description;
    bool isAvailable;

    MenuItem() : id(0), name(""), price(0.0), category(Category::APPETIZER), description(""), isAvailable(true) {}

    MenuItem(int id, const std::string& name, double price, Category cat, const std::string& desc, bool available = true)
        : id(id), name(name), price(price), category(cat), description(desc), isAvailable(available) {}

    std::string getCategoryString() const {
        switch (category) {
            case Category::APPETIZER:
                return "APPETIZER";
            case Category::MAIN_COURSE:
                return "MAIN COURSE";
            case Category::SOUP:
                return "SOUP";
            case Category::DESSERT:
                return "DESSERT";
            case Category::BEVERAGE:
                return "BEVERAGE";
            default:
                return "UNKNOWN";
        }
    }

    static Category stringToCategory(const std::string& catStr) {
        if (catStr == "APPETIZER") return Category::APPETIZER;
        if (catStr == "MAIN COURSE") return Category::MAIN_COURSE;
        if (catStr == "SOUP") return Category::SOUP;
        if (catStr == "DESSERT") return Category::DESSERT;
        if (catStr == "BEVERAGE") return Category::BEVERAGE;
        return Category::APPETIZER;
    }

    // Định dạng: id|name|price|category|description|available
    std::string serialize() const {
        return std::to_string(id) + "|" + name + "|" + std::to_string(price) + "|" + getCategoryString() + "|"
               + description + "|" + (isAvailable ? "1" : "0");
    }

    friend std::ostream& operator<<(std::ostream& os, const MenuItem& item) {
        os << "[" << item.id << "] " << item.name << " - " << item.price << " VND (" << item.getCategoryString()
           << ")";
        return os;
    }
};

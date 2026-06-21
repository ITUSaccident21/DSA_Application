#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <cstdlib>
#include <algorithm>
#include "../../lib/Algorithms.hpp"
#include "../managers/UserManager.hpp"
#include "../managers/MenuManager.hpp"
#include "../managers/OrderManager.hpp"
#include "../managers/KitchenManager.hpp"
#include "../utils/FileIO.hpp"
#include "../utils/StringUtils.hpp"
#include "../utils/TimeUtils.hpp"
#include "../utils/AlgorithmHelper.hpp"
 

class ConsoleUI {
private:
    UserManager& userManager;
    MenuManager& menuManager;
    OrderManager& orderManager;
    KitchenManager& kitchenManager;
    User* currentUser;
    int nextOrderId;
    int nextMenuId;
    int currentMenuMaxId;
    std::string usersFile;
    std::string menuFile;
    std::string ordersFile;

public:
    ConsoleUI(UserManager& um, MenuManager& mm, OrderManager& om, KitchenManager& km,
              const std::string& usersFilePath, const std::string& menuFilePath, const std::string& ordersFilePath)
        : userManager(um), menuManager(mm), orderManager(om), kitchenManager(km), currentUser(nullptr),
          nextOrderId(1001), nextMenuId(1001), currentMenuMaxId(1000), usersFile(usersFilePath), menuFile(menuFilePath), ordersFile(ordersFilePath) {
        initializeIds();
    }


    void run() {
        // Nếu không có user nào, tạo admin mặc định
        if (userManager.getUserCount() == 0) {
            User adminUser("admin", "admin123", "Administrator", UserRole::ADMIN, true);
            userManager.addUser(adminUser);
            FileIO::saveUsers(usersFile, userManager);
            std::cout << "Created default admin account (username: admin, password: admin123)\n\n";
        }

        while (true) {
            if (currentUser == nullptr) {
                showLoginScreen();
            } else {
                if (currentUser->role == UserRole::ADMIN) {
                    showAdminMenu();
                } else if (currentUser->role == UserRole::STAFF) {
                    showStaffMenu();
                } else if (currentUser->role == UserRole::CUSTOMER) {
                    showCustomerMenu();
                }
            }
        }
    }

private:
    void clearScreen() {
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif
    }

    void printHeader(const std::string& title) {
        clearScreen();
        std::cout << "========================================\n";
        std::cout << "     RESTAURANT MANAGEMENT SYSTEM\n";
        std::cout << "           " << title << "\n";
        std::cout << "========================================\n\n";
    }

    void waitForInput() {
        std::cout << "\nPress Enter to continue...";
        // Clear any error state
        std::cin.clear();
        // If there is leftover input in the buffer (e.g., a stray '\n'), consume up to a reasonable limit
        if (std::cin.rdbuf()->in_avail() > 0) {
            std::cin.ignore(10000, '\n');
        }
        // Wait for the user to press Enter using getline to consume the newline reliably
        std::string _tmp;
        std::getline(std::cin, _tmp);
    }

    void initializeIds() {
        int maxMenuId = 1000;
        for (std::size_t i = 0; i < menuManager.getSize(); ++i) {
            maxMenuId = std::max(maxMenuId, menuManager.getItems()[i].id);
        }
        currentMenuMaxId = maxMenuId;
        nextMenuId = maxMenuId + 1;

        int maxOrderId = 1000;
        orderManager.forEachOrderInorder([&](Order& order) {
            maxOrderId = std::max(maxOrderId, order.orderId);
        });
        nextOrderId = maxOrderId + 1;
    }

    // ========== LOGIN SCREEN ==========
    void showLoginScreen() {
        printHeader("Login / Register");

        std::cout << "1. Login\n";
        std::cout << "2. Register\n";
        std::cout << "3. Exit\n";
        std::cout << "\nChoice: ";

        int choice;
        std::cin >> choice;
        std::cin.ignore();

        if (choice == 1) {
            handleLogin();
        } else if (choice == 2) {
            handleRegister();
        } else if (choice == 3) {
            std::cout << "Thank you for using Restaurant Management System!\n";
            exit(0);
        }
    }

    void handleLogin() {
        std::cout << "\n--- Login ---\n";
        std::cout << "Username: ";
        std::string username;
        std::getline(std::cin, username);

        std::cout << "Password: ";
        std::string password;
        std::getline(std::cin, password);

        if (userManager.authenticate(username, password)) {
            currentUser = userManager.findUser(username);
            std::cout << "\nLogin successful! Welcome, " << currentUser->fullName << "!\n";
            waitForInput();
        } else {
            std::cout << "\nLogin failed! Invalid username or password.\n";
            waitForInput();
        }
    }

    void handleRegister() {
        std::cout << "\n--- Register ---\n";
        std::cout << "Username: ";
        std::string username;
        std::getline(std::cin, username);

        if (userManager.userExists(username)) {
            std::cout << "Username already exists!\n";
            waitForInput();
            return;
        }

        std::cout << "Password: ";
        std::string password;
        std::getline(std::cin, password);

        std::cout << "Full Name: ";
        std::string fullName;
        std::getline(std::cin, fullName);

        User newUser(username, password, fullName, UserRole::CUSTOMER, true);
        if (userManager.addUser(newUser)) {
            std::cout << "Registration successful! You can now login.\n";
            FileIO::saveUsers(usersFile, userManager);
        } else {
            std::cout << "Registration failed!\n";
        }
        waitForInput();
    }

    // ========== ADMIN MENU ==========
    void showAdminMenu() {
        printHeader("Admin Panel - " + currentUser->fullName);

        std::cout << "1. Manage Menu\n";
        std::cout << "2. View Orders\n";
        std::cout << "3. View Statistics\n";
        std::cout << "4. Manage Users\n";
        std::cout << "5. Logout\n";
        std::cout << "\nChoice: ";

        int choice;
        std::cin >> choice;
        std::cin.ignore();

        switch (choice) {
            case 1:
                showMenuManagement();
                break;
            case 2:
                showOrdersView();
                break;
            case 3:
                showStatistics();
                break;
            case 4:
                showUserManagement();
                break;
            case 5:
                currentUser = nullptr;
                break;
            default:
                std::cout << "Invalid choice!\n";
                waitForInput();
        }
    }

    void showMenuManagement() {
        while (true) {
            printHeader("Menu Management");

            std::cout << "1. View Menu\n";
            std::cout << "2. View Menu by Price\n";
            std::cout << "3. Add Menu Item\n";
            std::cout << "4. Add Multiple Items\n";
            std::cout << "5. Remove Item\n";
            std::cout << "6. Back\n";
            std::cout << "\nChoice: ";

            int choice;
            std::cin >> choice;
            std::cin.ignore();

            if (choice == 1) {
                displayMenuItems();
                waitForInput();
            } else if (choice == 2) {
                displayMenuByPrice();
                waitForInput();
            } else if (choice == 3) {
                addSingleMenuItem();
                waitForInput();
            } else if (choice == 4) {
                addMultipleMenuItems();
                waitForInput();
            } else if (choice == 5) {
                removeMenuItem();
                waitForInput();
            } else if (choice == 6) {
                break;
            }
        }
    }

    void displayMenuItems() {
        std::cout << "\n--- Current Menu ---\n";
        auto items = menuManager.getAvailableItems();

        if (items.empty()) {
            std::cout << "Menu is empty.\n";
            return;
        }

        // Sort items by ID for consistent display
        std::vector<MenuItem> sortedItems;
        for (const auto& item : items) {
            sortedItems.push_back(*item);
        }
        ds::insertionSort(sortedItems.begin(), sortedItems.end(), [](const MenuItem& a, const MenuItem& b) {
            return a.id < b.id;
        });

        std::cout << std::left << std::setw(5) << "ID" << std::setw(25) << "Name" << std::setw(10) << "Price"
                  << std::setw(15) << "Category" << std::endl;
        std::cout << std::string(55, '-') << "\n";

        for (const auto& item : sortedItems) {
            std::cout << std::left << std::setw(5) << item.id << std::setw(25) << item.name << std::setw(10)
                      << item.price << std::setw(15) << item.getCategoryString() << "\n";
        }
    }

    void displayMenuByPrice() {
        std::cout << "\n--- Menu sorted by Price (High to Low) ---\n";
        auto items = menuManager.getAvailableItems();

        if (items.empty()) {
            std::cout << "Menu is empty.\n";
            return;
        }

        auto sortedItems = AlgorithmHelper::sortMenuByPriceHeap(items);

        std::cout << std::left << std::setw(5) << "ID" << std::setw(25) << "Name" << std::setw(10) << "Price"
                  << std::setw(15) << "Category" << std::endl;
        std::cout << std::string(55, '-') << "\n";

        for (const auto& item : sortedItems) {
            std::cout << std::left << std::setw(5) << item.id << std::setw(25) << item.name << std::setw(10)
                      << item.price << std::setw(15) << item.getCategoryString() << "\n";
        }
    }

    void addSingleMenuItem() {
        std::cout << "\n--- Add Menu Item ---\n";

        std::cout << "Name: ";
        std::string name;
        std::getline(std::cin, name);

        std::cout << "Price: ";
        double price;
        std::cin >> price;
        std::cin.ignore();

        std::cout << "Category (0=Appetizer, 1=Main, 2=Soup, 3=Dessert, 4=Beverage): ";
        int catChoice;
        std::cin >> catChoice;
        std::cin.ignore();
        Category category = static_cast<Category>(catChoice % 5);

        std::cout << "Description: ";
        std::string description;
        std::getline(std::cin, description);

        int newMenuId = nextMenuId++;
        if (currentMenuMaxId >= newMenuId) {
            newMenuId = currentMenuMaxId + 1;
            nextMenuId = newMenuId + 1;
        }

        MenuItem item(newMenuId, name, price, category, description, true);
        menuManager.addItem(item);
        currentMenuMaxId = std::max(currentMenuMaxId, newMenuId);
        menuManager.sortByName();

        std::cout << "\nItem added successfully!\n";
        FileIO::saveMenu(menuFile, menuManager);
    }

    // Thêm nhiều items và sắp xếp lại bằng InsertionSort
    void addMultipleMenuItems() {
        std::cout << "\n--- Add Multiple Menu Items ---\n";
        std::cout << "How many items to add? ";
        int count;
        std::cin >> count;
        std::cin.ignore();

        std::vector<MenuItem> newItems;
        for (int i = 0; i < count; ++i) {
            std::cout << "\nItem " << (i + 1) << ":\n";
            std::cout << "Name: ";
            std::string name;
            std::getline(std::cin, name);

            std::cout << "Price: ";
            double price;
            std::cin >> price;
            std::cin.ignore();

            std::cout << "Category (0-4): ";
            int catChoice;
            std::cin >> catChoice;
            std::cin.ignore();
            Category category = static_cast<Category>(catChoice % 5);

            std::cout << "Description: ";
            std::string description;
            std::getline(std::cin, description);

            int newMenuId = nextMenuId++;
            if (currentMenuMaxId >= newMenuId) {
                newMenuId = currentMenuMaxId + 1;
                nextMenuId = newMenuId + 1;
            }
            newItems.push_back(MenuItem(newMenuId, name, price, category, description, true));
            currentMenuMaxId = std::max(currentMenuMaxId, newMenuId);
        }

        MenuItem* items = new MenuItem[newItems.size()];
        for (std::size_t i = 0; i < newItems.size(); ++i) {
            items[i] = newItems[i];
            menuManager.addItem(items[i]);
        }
        
        menuManager.sortByName();
        
        std::cout << "\n" << count << " items added successfully!\n";
        FileIO::saveMenu(menuFile, menuManager);
        delete[] items;
    }

    void removeMenuItem() {
        std::cout << "\n--- Remove Menu Item ---\n";
        std::cout << "Item ID: ";
        int id;
        std::cin >> id;
        std::cin.ignore();

        if (menuManager.removeItem(id)) {
            std::cout << "Item removed successfully!\n";
            FileIO::saveMenu(menuFile, menuManager);
        } else {
            std::cout << "Item not found!\n";
        }
    }

    void showOrdersView() {
        while (true) {
            printHeader("View Orders");

            std::cout << "1. View All Orders\n";
            std::cout << "2. View Orders by Status\n";
            std::cout << "3. Back\n";
            std::cout << "\nChoice: ";

            int choice;
            std::cin >> choice;
            std::cin.ignore();

            if (choice == 1) {
                displayAllOrders();
                waitForInput();
            } else if (choice == 2) {
                displayOrdersByStatus();
                waitForInput();
            } else if (choice == 3) {
                break;
            }
        }
    }

    void displayAllOrders() {
        std::cout << "\n--- All Orders (in order of creation) ---\n";
        int count = 0;
        orderManager.forEachOrderInorder([&](Order& order) {
            count++;
            std::cout << count << ". " << order << "\n";
        });
        if (count == 0) {
            std::cout << "No orders found.\n";
        }
    }

    void displayOrdersByStatus() {
        std::cout << "\nSelect status:\n";
        std::cout << "0. Pending\n";
        std::cout << "1. Ready\n";
        std::cout << "\nChoice: ";
        int statusChoice;
        std::cin >> statusChoice;
        std::cin.ignore();

        if (statusChoice < 0 || statusChoice > 1) {
            std::cout << "Invalid status!\n";
            return;
        }

        OrderStatus status = (statusChoice == 0 ? OrderStatus::PENDING : OrderStatus::READY);
        auto orders = orderManager.getOrdersByStatus(status);

        std::string statusName = (status == OrderStatus::PENDING ? "PENDING" : "READY");
        std::cout << "\n--- Orders with status: " << statusName << " ---\n";
        if (orders.empty()) {
            std::cout << "No orders found.\n";
            return;
        }

        for (const auto& order : orders) {
            std::cout << *order << "\n";
            std::cout << "  Items:\n";
            for (const auto& item : order->items) {
                std::cout << "    - " << item << "\n";
            }
        }
    }

    void showStatistics() {
        while (true) {
            printHeader("Statistics & Reports");

            std::cout << "1. View Order Statistics\n";
            std::cout << "2. View Orders History\n";
            std::cout << "3. View Top 5 Most Expensive Items\n";
            std::cout << "4. Back\n";
            std::cout << "\nChoice: ";

            int choice;
            std::cin >> choice;
            std::cin.ignore();

            if (choice == 1) {
                displayOrderStatistics();
                waitForInput();
            } else if (choice == 2) {
                displayOrdersHistoryMerged();
                waitForInput();
            } else if (choice == 3) {
                displayTopPriceItems();
                waitForInput();
            } else if (choice == 4) {
                break;
            }
        }
    }

    void displayOrderStatistics() {
        std::cout << "\n--- Order Statistics ---\n";
        std::cout << "Total Orders: " << orderManager.getOrderCount() << "\n";
        std::cout << "Pending Orders: " << orderManager.countOrdersByStatus(OrderStatus::PENDING) << "\n";
        std::cout << "Confirmed Orders: " << orderManager.countOrdersByStatus(OrderStatus::CONFIRMED) << "\n";
        std::cout << "Cooking Orders: " << orderManager.countOrdersByStatus(OrderStatus::COOKING) << "\n";
        std::cout << "Ready Orders: " << orderManager.countOrdersByStatus(OrderStatus::READY) << "\n";
        std::cout << "Delivered Orders: " << orderManager.countOrdersByStatus(OrderStatus::DELIVERED) << "\n";

        std::cout << "\n--- Revenue ---\n";
        std::cout << "Total Revenue: " << orderManager.getTotalRevenue() << " VND\n";
        std::cout << "Delivered Orders Value: " << orderManager.getTotalRevenueByStatus(OrderStatus::DELIVERED)
                  << " VND\n";

        std::cout << "\n--- Menu Statistics ---\n";
        std::cout << "Total Menu Items: " << menuManager.getSize() << "\n";
    }

    void displayOrdersHistoryMerged() {
        std::cout << "\n--- Orders History (Newest First) ---\n";
        std::vector<Order*> allOrders;
        
        orderManager.forEachOrderInorder([&](Order& order) {
            allOrders.push_back(&order);
        });

        if (allOrders.empty()) {
            std::cout << "No orders found.\n";
            return;
        }

        auto sortedOrders = AlgorithmHelper::sortOrdersByTimeNewest(allOrders);

        int count = 0;
        for (const auto& order : sortedOrders) {
            count++;
            std::cout << count << ". Order #" << order->orderId << " - User: " << order->userId
                      << " - Status: " << order->getStatusString() << " - Time: " << TimeUtils::timestampToString(order->timestamp)
                      << " - Total: " << order->totalPrice << " VND\n";
        }
    }

    void displayTopPriceItems() {
        std::cout << "\n--- Top 5 Most Expensive Items ---\n";
        auto items = menuManager.getAvailableItems();

        if (items.empty()) {
            std::cout << "Menu is empty.\n";
            return;
        }

        auto topItems = AlgorithmHelper::getTopPriceItemsSelection(items, 5);

        std::cout << std::left << std::setw(5) << "ID" << std::setw(25) << "Name" << std::setw(10) << "Price"
                  << std::setw(15) << "Category" << std::endl;
        std::cout << std::string(55, '-') << "\n";

        for (const auto& item : topItems) {
            std::cout << std::left << std::setw(5) << item.id << std::setw(25) << item.name << std::setw(10)
                      << item.price << std::setw(15) << item.getCategoryString() << "\n";
        }
    }

    void showUserManagement() {
        while (true) {
            printHeader("User Management");

            std::cout << "1. Create Staff Account\n";
            std::cout << "2. Back\n";
            std::cout << "\nChoice: ";

            int choice;
            std::cin >> choice;
            std::cin.ignore();

            if (choice == 1) {
                createStaffAccount();
                waitForInput();
            } else if (choice == 2) {
                break;
            }
        }
    }

    void createStaffAccount() {
        std::cout << "\n--- Create Staff Account ---\n";
        std::cout << "Username: ";
        std::string username;
        std::getline(std::cin, username);

        if (userManager.userExists(username)) {
            std::cout << "Username already exists!\n";
            return;
        }

        std::cout << "Password: ";
        std::string password;
        std::getline(std::cin, password);

        std::cout << "Full Name: ";
        std::string fullName;
        std::getline(std::cin, fullName);

        User staffUser(username, password, fullName, UserRole::STAFF, true);
        if (userManager.addUser(staffUser)) {
            std::cout << "Staff account created successfully!\n";
            FileIO::saveUsers(usersFile, userManager);
        } else {
            std::cout << "Failed to create staff account!\n";
        }
    }

    // ========== STAFF MENU ==========
    void showStaffMenu() {
        printHeader("Staff Panel - " + currentUser->fullName);

        std::cout << "1. View Kitchen Queue\n";
        std::cout << "2. Manage Cooking Tasks\n";
        std::cout << "3. Logout\n";
        std::cout << "\nChoice: ";

        int choice;
        std::cin >> choice;
        std::cin.ignore();

        switch (choice) {
            case 1:
                viewKitchenQueue();
                waitForInput();
                break;
            case 2:
                manageKitchenTasks();
                break;
            case 3:
                currentUser = nullptr;
                break;
            default:
                std::cout << "Invalid choice!\n";
                waitForInput();
        }
    }

    void viewKitchenQueue() {
        std::cout << "\n--- Kitchen Queue ---\n";
        auto tasks = kitchenManager.getAllTasks();

        if (tasks.empty()) {
            std::cout << "Kitchen queue is empty.\n";
            return;
        }

        for (const auto& task : tasks) {
            std::cout << "- " << task << "\n";
        }
    }

    void manageKitchenTasks() {
        while (true) {
            printHeader("Manage Kitchen Tasks");

            if (kitchenManager.isEmpty()) {
                std::cout << "No tasks in queue.\n\n";
                std::cout << "1. Back\n";
                std::cout << "Choice: ";
                int choice;
                std::cin >> choice;
                std::cin.ignore();
                if (choice == 1) {
                    break;
                }
            } else {
                const auto& topTask = kitchenManager.getTopTask();
                std::cout << "Next task: " << topTask << "\n\n";

                std::cout << "1. Mark as Ready\n";
                std::cout << "2. Back\n";
                std::cout << "\nChoice: ";

                int choice;
                std::cin >> choice;
                std::cin.ignore();

                if (choice == 1) {
                    kitchenManager.popTask();
                    Order* order = orderManager.findOrder(topTask.orderId);
                    if (order != nullptr) {
                        order->status = OrderStatus::READY;
                        FileIO::saveOrders(ordersFile, orderManager);
                    }
                    std::cout << "Order #" << topTask.orderId << " marked as Ready!\n";
                    waitForInput();
                } else if (choice == 2) {
                    break;
                }
            }
        }
    }

    // ========== CUSTOMER MENU ==========
    void showCustomerMenu() {
        printHeader("Customer Menu - " + currentUser->fullName);

        std::cout << "1. View Menu\n";
        std::cout << "2. Create Order\n";
        std::cout << "3. Upgrade to VIP (500000 VND)\n";
        std::cout << "4. View My Orders\n";
        std::cout << "5. Logout\n";
        std::cout << "\nChoice: ";

        int choice;
        std::cin >> choice;
        std::cin.ignore();

        switch (choice) {
            case 1:
                displayMenuItems();
                waitForInput();
                break;
            case 2:
                createOrder();
                break;
            case 3:
                upgradeToVIP();
                waitForInput();
                break;
            case 4:
                viewMyOrders();
                waitForInput();
                break;
            case 5:
                currentUser = nullptr;
                break;
            default:
                std::cout << "Invalid choice!\n";
                waitForInput();
        }
    }

    void createOrder() {
        printHeader("Create Order");

        auto items = menuManager.getAvailableItems();
        if (items.empty()) {
            std::cout << "Menu is empty!\n";
            waitForInput();
            return;
        }

        // Sort items by name for consistent display using DSA insertion sort
        ds::insertionSort(items.begin(), items.end(), [](const MenuItem* a, const MenuItem* b) {
            return a->name < b->name;
        });

        std::cout << "--- Available Items ---\n";
        for (const auto& item : items) {
            std::cout << item->id << ". " << item->name << " - " << item->price << " VND\n";
        }

        Order order(nextOrderId++, currentUser->username, currentUser->isVIP);
        bool addMore = true;

        while (addMore) {
            std::cout << "\nItem ID (0 to finish): ";
            int itemId;
            std::cin >> itemId;
            std::cin.ignore();

            if (itemId == 0) {
                break;
            }

            MenuItem* menuItem = menuManager.findById(itemId);
            if (menuItem == nullptr || !menuItem->isAvailable) {
                std::cout << "Item not found or not available!\n";
                continue;
            }

            std::cout << "Quantity: ";
            int qty;
            std::cin >> qty;
            std::cin.ignore();

            if (qty <= 0) {
                std::cout << "Invalid quantity!\n";
                continue;
            }

            OrderItem orderItem(*menuItem, qty);
            order.addItem(orderItem);
            std::cout << "Item added to order.\n";
        }

        if (order.getItemCount() > 0) {
            order.status = OrderStatus::CONFIRMED;
            orderManager.addOrder(order);

            // Thêm vào hàng chờ bếp
            KitchenTask task(order.orderId, order.getItemCount(), order.timestamp, order.isVIP, 1);
            kitchenManager.addTask(task);

            std::cout << "\n--- Order Summary ---\n";
            std::cout << "Order ID: #" << order.orderId << "\n";
            std::cout << "Total Items: " << order.getItemCount() << "\n";
            std::cout << "Total Price: " << order.totalPrice << " VND\n";
            std::cout << "\nOrder created successfully!\n";
            FileIO::saveOrders(ordersFile, orderManager);
        } else {
            std::cout << "No items added. Order cancelled.\n";
        }

        waitForInput();
    }

    void upgradeToVIP() {
        printHeader("Upgrade to VIP");

        if (currentUser->isVIP) {
            std::cout << "You are already a VIP customer.\n";
            return;
        }

        std::cout << "VIP upgrade costs 500000 VND. Confirm upgrade? (y/n): ";
        char choice;
        std::cin >> choice;
        std::cin.ignore();

        if (choice != 'y' && choice != 'Y') {
            std::cout << "VIP upgrade cancelled.\n";
            return;
        }

        currentUser->isVIP = true;
        FileIO::saveUsers(usersFile, userManager);
        std::cout << "Congratulations! You are now a VIP customer.\n";
    }

    void viewMyOrders() {
        printHeader("My Orders");

        auto orders = orderManager.getOrdersByUser(currentUser->username);
        if (orders.empty()) {
            std::cout << "You have no orders.\n";
            return;
        }

        std::cout << "--- Your Orders ---\n";
        for (const auto& order : orders) {
            std::cout << "Order #" << order->orderId << " - Status: " << order->getStatusString()
                      << " - Total: " << order->totalPrice << " VND\n";
            std::cout << "  Items:\n";
            for (const auto& item : order->items) {
                std::cout << "    - " << item << "\n";
            }
            std::cout << "\n";
        }
    }
};

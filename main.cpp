#include <iostream>
#include <string>
#include "app/managers/UserManager.hpp"
#include "app/managers/MenuManager.hpp"
#include "app/managers/OrderManager.hpp"
#include "app/managers/KitchenManager.hpp"
#include "app/utils/FileIO.hpp"
#include "app/ui/ConsoleUI.hpp"

int main() {
    try {
        // Khởi tạo các managers
        UserManager userManager;
        MenuManager menuManager;
        OrderManager orderManager;
        KitchenManager kitchenManager;

        // Tạo đường dẫn file dữ liệu
        std::string dataDir = "./data/";
        std::string usersFile = dataDir + "users.txt";
        std::string menuFile = dataDir + "menu.txt";
        std::string ordersFile = dataDir + "orders.txt";

        // Load dữ liệu từ file
        std::cout << "Loading data...\n";
        FileIO::loadUsers(usersFile, userManager);
        FileIO::loadMenu(menuFile, menuManager);
        FileIO::loadOrders(ordersFile, orderManager, menuManager);

        std::cout << "Data loaded successfully!\n";
        std::cout << "Users: " << userManager.getUserCount() << "\n";
        std::cout << "Menu items: " << menuManager.getSize() << "\n";
        std::cout << "Orders: " << orderManager.getOrderCount() << "\n\n";

        // Khởi tạo UI
        ConsoleUI ui(userManager, menuManager, orderManager, kitchenManager);

        // Chạy ứng dụng
        ui.run();

        // Lưu dữ liệu (đoạn này sẽ không bao giờ được đạt tới vì ui.run() là vòng lặp vô hạn)
        std::cout << "Saving data...\n";
        FileIO::saveUsers(usersFile, userManager);
        FileIO::saveMenu(menuFile, menuManager);
        FileIO::saveOrders(ordersFile, orderManager);

        std::cout << "Data saved successfully!\n";

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}

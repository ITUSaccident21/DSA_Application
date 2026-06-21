# BUILD & TESTING GUIDE

## Yêu Cầu Hệ Thống

- **C++ Standard**: C++17 hoặc cao hơn
- **Compiler**: g++ 7.0+, clang++ 5.0+, MSVC 2017+
- **OS**: Linux, macOS, hoặc Windows (WSL/MinGW)
- **Storage**: ~50MB (code + data)

---

## Cách Build

### Option 1: Sử Dụng Script (Recommended)

```bash
# Di chuyển vào thư mục project
cd /workspaces/DSA_Application

# Làm executable có quyền execute
chmod +x build.sh

# Chạy script build
./build.sh
```

**Output:**
```
Building Restaurant Management System...
=========================================
Compiling...
✓ Build successful!

To run the application:
  ./restaurant_app

Make sure data directory exists:
✓ Data directory ready
```

### Option 2: Manual Compile

```bash
cd /workspaces/DSA_Application

# Compile với g++
g++ -std=c++17 -I. main.cpp -o restaurant_app

# Hoặc với clang++
clang++ -std=c++17 -I. main.cpp -o restaurant_app
```

### Option 3: Sử Dụng CMake (Optional)

```bash
# Tạo CMakeLists.txt
mkdir build && cd build
cmake ..
make
```

---

## Chạy Ứng Dụng

### Basic Run

```bash
./restaurant_app
```

### Với Command Line Arguments (Future Enhancement)

```bash
./restaurant_app --admin-mode
./restaurant_app --reset-data
./restaurant_app --import-file=custom_menu.txt
```

---

## Troubleshooting Compilation

### Error: `command not found: g++`
**Solution**: Install GCC
```bash
# Ubuntu/Debian
sudo apt-get update
sudo apt-get install build-essential

# macOS
brew install gcc

# Windows: Install MinGW or use WSL
```

### Error: `cannot find -lstdc++fs`
**Solution**: Cập nhật compiler hoặc sử dụng flag `-fno-strict-modules`
```bash
g++ -std=c++17 -I. main.cpp -o restaurant_app -fno-strict-modules
```

### Error: `Undefined reference to ...`
**Solution**: Đảm bảo tất cả includes đúng
```bash
# Check includes
grep -r "#include" app/

# Recompile
make clean
./build.sh
```

---

## Unit Testing

### Test HashTable (UserManager)

```cpp
// File: test_user_manager.cpp
#include <iostream>
#include "app/managers/UserManager.hpp"

int main() {
    UserManager mgr;
    
    // Test 1: Add user
    User user1("john", "pass123", "John Doe", UserRole::CUSTOMER);
    assert(mgr.addUser(user1));
    assert(!mgr.addUser(user1)); // Duplicate
    
    // Test 2: Find user
    User* found = mgr.findUser("john");
    assert(found != nullptr);
    assert(found->fullName == "John Doe");
    
    // Test 3: Authenticate
    assert(mgr.authenticate("john", "pass123"));
    assert(!mgr.authenticate("john", "wrong"));
    
    std::cout << "✓ All UserManager tests passed!\n";
    return 0;
}
```

Compile test:
```bash
g++ -std=c++17 -I. test_user_manager.cpp -o test_user
./test_user
```

### Test MenuManager (QuickSort + InsertionSort)

```cpp
// File: test_menu_manager.cpp
#include "app/managers/MenuManager.hpp"

int main() {
    MenuManager mgr;
    
    // Test 1: QuickSort on load
    mgr.addItem(MenuItem(1, "Zebra", 100, Category::MAIN_COURSE, "Z", true));
    mgr.addItem(MenuItem(2, "Apple", 50, Category::APPETIZER, "A", true));
    mgr.addItem(MenuItem(3, "Mango", 75, Category::DESSERT, "M", true));
    
    mgr.sortByName(); // QuickSort
    
    auto items = mgr.getAvailableItems();
    assert(items[0]->name == "Apple");     // A
    assert(items[1]->name == "Mango");     // M
    assert(items[2]->name == "Zebra");     // Z
    
    std::cout << "✓ All MenuManager tests passed!\n";
    return 0;
}
```

### Test OrderManager (BST)

```cpp
// File: test_order_manager.cpp
#include "app/managers/OrderManager.hpp"

int main() {
    OrderManager mgr;
    
    // Test 1: Insert and find
    Order order1(1001, "customer1");
    mgr.addOrder(order1);
    
    Order* found = mgr.findOrder(1001);
    assert(found != nullptr);
    assert(found->userId == "customer1");
    
    // Test 2: Inorder traversal
    Order order2(1002, "customer2");
    Order order3(1000, "customer3");
    mgr.addOrder(order2);
    mgr.addOrder(order3);
    
    // Inorder should give: 1000, 1001, 1002
    std::vector<Order*> inorder;
    mgr.forEachOrderInorder([&](Order& o) {
        inorder.push_back(&o);
    });
    
    assert(inorder[0]->orderId == 1000);
    assert(inorder[1]->orderId == 1001);
    assert(inorder[2]->orderId == 1002);
    
    std::cout << "✓ All OrderManager tests passed!\n";
    return 0;
}
```

### Test KitchenManager (PriorityQueue)

```cpp
// File: test_kitchen_manager.cpp
#include "app/managers/KitchenManager.hpp"

int main() {
    KitchenManager mgr;
    
    // Test 1: Priority ordering (VIP first)
    KitchenTask task1(101, 2, std::time(nullptr), false, 1); // Not VIP
    KitchenTask task2(102, 1, std::time(nullptr), true, 1);  // VIP
    
    mgr.addTask(task1);
    mgr.addTask(task2);
    
    // Top should be VIP (task2)
    KitchenTask top = mgr.getTopTask();
    assert(top.orderId == 102);
    assert(top.isVIP == true);
    
    std::cout << "✓ All KitchenManager tests passed!\n";
    return 0;
}
```

---

## Integration Testing

### Scenario 1: Full Order Flow

```bash
# Run app
./restaurant_app

# Steps:
1. Login: customer1 / cust123
2. View Menu
3. Create Order
   - Add Spring Rolls (qty 2)
   - Add Pho Bo (qty 1)
4. View My Orders (should show new order)
5. Logout

# Expected:
- Order created with LinkedList items
- Kitchen queue updated with PriorityQueue
```

### Scenario 2: Admin Operations

```bash
./restaurant_app

# Steps:
1. Login: admin / admin123
2. Menu Management
   - View Menu (QuickSort applied on load)
   - View Menu by Price (HeapSort applied)
   - Add Multiple Items (InsertionSort applied)
3. Statistics
   - Order Statistics (direct count)
   - Orders History (MergeSort applied)
   - Top Price Items (SelectionSort applied)
4. Logout

# Expected:
- Menu sorted alphabetically
- Prices displayed high to low
- History shows newest first
- Top 5 items by price
```

### Scenario 3: Staff Operations

```bash
./restaurant_app

# Prerequisites: Create order as customer first

# Steps:
1. Login: staff1 / staff123
2. View Kitchen Queue
   - See pending orders (PriorityQueue)
3. Manage Cooking Tasks
   - Mark order as Ready (pop from queue)
4. Logout

# Expected:
- Queue shows VIP orders first
- Tasks processed by priority
- Order status updated in BST
```

---

## Performance Testing

### Load Test: 1000 Menu Items

```cpp
// Create test data
MenuManager mgr;
for (int i = 0; i < 1000; ++i) {
    mgr.addItem(MenuItem(i, "Item" + std::to_string(i), rand() % 500000, 
                         static_cast<Category>(rand() % 5), "desc", true));
}

// Time QuickSort
auto start = std::chrono::high_resolution_clock::now();
mgr.sortByName();
auto end = std::chrono::high_resolution_clock::now();
auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

std::cout << "QuickSort 1000 items: " << duration.count() << "ms\n";
// Expected: < 10ms
```

### Load Test: 10000 Orders in BST

```cpp
OrderManager mgr;
auto start = std::chrono::high_resolution_clock::now();

for (int i = 0; i < 10000; ++i) {
    Order order(i, "user" + std::to_string(i % 100));
    mgr.addOrder(order);
}

auto end = std::chrono::high_resolution_clock::now();
auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

std::cout << "Insert 10000 orders in BST: " << duration.count() << "ms\n";
// Expected: < 50ms
```

### Load Test: Kitchen Queue Priority

```cpp
KitchenManager mgr;
auto start = std::chrono::high_resolution_clock::now();

for (int i = 0; i < 1000; ++i) {
    KitchenTask task(i, 1, std::time(nullptr), i % 10 == 0, 1);
    mgr.addTask(task);
}

auto end = std::chrono::high_resolution_clock::now();
auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

std::cout << "Add 1000 tasks to PriorityQueue: " << duration.count() << "ms\n";
// Expected: < 20ms
```

---

## Data Validation

### Check Data Files

```bash
# users.txt
wc -l data/users.txt
head -n 5 data/users.txt

# menu.txt
wc -l data/menu.txt
head -n 5 data/menu.txt

# orders.txt (empty at startup)
cat data/orders.txt
```

### Reset Data

```bash
# Backup current data
mkdir -p data.backup
cp data/* data.backup/

# Reset
rm data/orders.txt
touch data/orders.txt

# Or restore
cp data.backup/* data/
```

---

## Debugging

### Enable Verbose Logging

Create `DEBUG.hpp`:
```cpp
#define DEBUG_MODE 1

#if DEBUG_MODE
    #define LOG_DEBUG(msg) std::cerr << "[DEBUG] " << msg << "\n"
#else
    #define LOG_DEBUG(msg)
#endif
```

Use in code:
```cpp
// In managers
LOG_DEBUG("Adding item: " << item.name);
LOG_DEBUG("BST size: " << orders.size());
```

### Run with GDB

```bash
# Compile with debug symbols
g++ -std=c++17 -g -I. main.cpp -o restaurant_app

# Run under gdb
gdb ./restaurant_app

# In gdb:
(gdb) run
(gdb) break app/managers/OrderManager.hpp:50
(gdb) continue
(gdb) print order
(gdb) step
```

---

## Checklist Sebelum Release

- [ ] Compile tanpa warnings: `g++ -Wall -Wextra`
- [ ] Semua tests pass
- [ ] Data files exist: `users.txt`, `menu.txt`, `orders.txt`
- [ ] README.md updated
- [ ] ARCHITECTURE.md current
- [ ] ALGORITHM_USAGE.md documented
- [ ] Source code formatted
- [ ] No memory leaks (valgrind)

```bash
# Check for memory leaks
valgrind --leak-check=full ./restaurant_app << EOF
admin
admin123
5
EOF

# Check compiler warnings
g++ -Wall -Wextra -std=c++17 -I. main.cpp -o restaurant_app
```

---

## Next Steps for Enhancement

1. **Add Unit Test Framework**
   ```
   apt-get install libgtest-dev
   Create test suite with Google Test
   ```

2. **Add Continuous Integration**
   ```
   GitHub Actions / GitLab CI
   Auto-run tests on push
   ```

3. **Performance Profiling**
   ```
   perf record ./restaurant_app
   perf report
   ```

4. **Memory Profiling**
   ```
   valgrind --tool=massif ./restaurant_app
   ms_print output
   ```

5. **Code Coverage**
   ```
   g++ --coverage
   gcov
   ```

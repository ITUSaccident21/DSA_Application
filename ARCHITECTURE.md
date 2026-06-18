# ARCHITECTURE.md - Chi Tiết Thiết Kế Hệ Thống

## Tổng Quan

Restaurant Management System được xây dựng với kiến trúc phân tầng (Layered Architecture):

```
┌─────────────────────────────┐
│   UI Layer (ConsoleUI)      │  ← Giao diện console
├─────────────────────────────┤
│   Business Logic Layer      │  ← Managers (User, Menu, Order, Kitchen)
│   (Managers)                │
├─────────────────────────────┤
│   Data Models Layer         │  ← Models (User, MenuItem, Order, etc.)
│   (Models)                  │
├─────────────────────────────┤
│   Utilities Layer           │  ← FileIO, StringUtils, AlgorithmHelper
│   (Utils)                   │
├─────────────────────────────┤
│   DSA Library               │  ← HashTable, BST, LinkedList, etc.
│   (lib/)                    │
└─────────────────────────────┘
```

---

## Layer 1: UI Layer (ConsoleUI)

### Trách Nhiệm
- Hiển thị menu cho từng role (Admin, Staff, Customer)
- Thu thập input từ người dùng
- Điều hướng giữa các screens
- Định dạng output

### Các Màn Hình Chính

#### Authentication Screen
```
┌─ showLoginScreen()
│  ├─ handleLogin()     [Authenticate via UserManager]
│  └─ handleRegister()  [Add user via UserManager]
```

#### Admin Panel
```
┌─ showAdminMenu()
│  ├─ showMenuManagement()
│  │  ├─ displayMenuItems()            [Hiển thị menu]
│  │  ├─ displayMenuByPrice()          [HeapSort]
│  │  ├─ addSingleMenuItem()           [Thêm 1 item]
│  │  ├─ addMultipleMenuItems()        [Thêm nhiều + InsertionSort]
│  │  └─ removeMenuItem()              [Xóa item]
│  │
│  ├─ showOrdersView()
│  │  ├─ displayAllOrders()            [BST inorder]
│  │  └─ displayOrdersByStatus()       [Filter từ BST]
│  │
│  ├─ showStatistics()
│  │  ├─ displayOrderStatistics()      [Count, revenue]
│  │  ├─ displayOrdersHistoryMerged()  [MergeSort]
│  │  └─ displayTopPriceItems()        [SelectionSort]
│  │
│  └─ showUserManagement()
│     └─ createStaffAccount()          [Add staff user]
```

#### Staff Panel
```
┌─ showStaffMenu()
│  ├─ viewKitchenQueue()               [Hiển thị hàng chờ]
│  └─ manageKitchenTasks()             [Xử lý task]
│     ├─ getTopTask()                  [Pop từ PriorityQueue]
│     └─ updateOrderStatus()           [Cập nhật BST]
```

#### Customer Panel
```
┌─ showCustomerMenu()
│  ├─ displayMenuItems()               [Xem menu]
│  ├─ createOrder()                    [Tạo order mới]
│  │  └─ addItem()                     [Thêm vào LinkedList]
│  └─ viewMyOrders()                   [getOrdersByUser từ BST]
```

---

## Layer 2: Business Logic Layer (Managers)

### UserManager (HashTable)

```cpp
UserManager
├─ HashTable<string, User> users
├─ insert(username, user)      [O(1) average]
├─ find(username)              [O(1) average]
├─ authenticate()              [HashTable lookup]
└─ remove(username)            [O(1) average]
```

**Lý do HashTable:**
- Username là unique key
- Lookup thường xuyên cho authentication
- Thêm/xóa không thường xuyên nhưng cần nhanh

---

### MenuManager (Dynamic Array + Sorting)

```cpp
MenuManager
├─ MenuItem* items             [Dynamic array]
├─ size_t size
├─ size_t capacity
│
├─ addItem(item)               [O(1) amortized]
├─ sortByName()                [QuickSort O(n log n)]
├─ addItemsAndSort()           [Thêm + InsertionSort O(n)]
├─ findById(id)                [O(n) linear search]
├─ findByName(name)            [O(log n) binary search nếu sorted]
└─ getAvailableItems()         [O(n) filter]
```

**Lý do Dynamic Array:**
```
Operasi        Array    BST
──────────────────────────────
Display all    O(n)     O(n) [inorder]
Add 1          O(1)     O(log n)
Remove         O(n)     O(log n)
Find by ID     O(n)     O(log n)
Find by Name   O(log n) O(log n)
Sorted access  O(1)     O(n)
```
→ Array tốt hơn vì display (O(1) per item) thường xuyên

**Khi nào sử dụng sắp xếp nào:**
```
Load từ file (10+ items):     QuickSort [O(n log n)]
Thêm 1-2 items:              InsertionSort [O(n)]
View by price:               HeapSort [O(n log n)]
```

---

### OrderManager (BST)

```cpp
OrderManager
├─ BST<Order, ..., OrderKey> orders
│  └─ Key = orderId
│
├─ insert(order)              [O(log n)]
├─ find(orderId)              [O(log n)]
├─ getOrdersByUser()          [O(n) inorder + filter]
├─ getOrdersByStatus()        [O(n) inorder + filter]
├─ forEachOrderInorder()      [O(n) traversal]
│  └─ Dùng cho: lịch sử, report
└─ remove(orderId)            [O(log n)]
```

**Lý do BST:**
```
Operasi               Array    BST
────────────────────────────────────
Find by ID            O(n)     O(log n) ✓
Insert new            O(1)     O(log n)
Delete                O(n)     O(log n)
Ordered traversal     O(n)     O(n) [inorder] ✓
Filter by status      O(n)     O(n)
Statistics            O(n)     O(n)
```
→ BST tốt vì: find (O(log n)) + ordered traversal (O(n))

**Flow tạo Order:**
```
Customer tạo order
  ↓
ConsoleUI.createOrder()
  ↓
Order với LinkedList<OrderItem>
  ↓
OrderManager.insert() → BST
  ↓
KitchenManager.addTask() → PriorityQueue
```

---

### KitchenManager (PriorityQueue)

```cpp
KitchenManager
├─ PriorityQueue<KitchenTask, Comparator> queue
│  └─ Heap structure (max-heap)
│
├─ addTask(task)               [O(log n) sift-up]
├─ popTask()                   [O(log n) sift-down]
├─ getTopTask()                [O(1) peek]
└─ getAllTasks()               [O(n) extract all]
```

**Comparator cho KitchenTask:**
```cpp
bool operator>(const KitchenTask& other) const {
    // VIP first
    if (isVIP != other.isVIP) {
        return isVIP > other.isVIP;
    }
    // Then by priority (lower = higher)
    if (priority != other.priority) {
        return priority < other.priority;
    }
    // Then by order time (earlier = higher)
    return orderTime < other.orderTime;
}
```

**Lý do PriorityQueue:**
```
Cần gì                 Array+Sort   BST      PriorityQueue
──────────────────────────────────────────────────────────
Lấy task ưu tiên cao   O(1) [0]     O(log n) O(1) [top] ✓
Thêm task mới          O(n) [sort]  O(log n) O(log n) ✓
Xóa task               O(n) [shift] O(log n) O(log n) ✓
Xem tất cả tasks       O(n)         O(n)     O(n)
```
→ PriorityQueue tốt vì: luôn cần task top (O(1)) + quick add/remove

---

## Layer 3: Data Models Layer

### Order Model - LinkedList Design

```cpp
struct Order {
    int orderId;
    string userId;
    LinkedList<OrderItem> items;    ← Key design
    OrderStatus status;
    time_t timestamp;
    double totalPrice;
    bool isVIP;
};
```

**Tại sao LinkedList cho items?**
```
Số lượng items không biết trước:
  - Customer 1 order: 1 item
  - Customer 2 order: 5 items
  - Customer 3 order: 10 items
  → Không thể dùng array[MAX_SIZE]

LinkedList advantages:
  ✓ Thêm item O(1) (push_back)
  ✓ Iterate qua items O(n)
  ✓ Không cần biết kích thước trước
  ✗ Không access ngẫu nhiên (nhưng không cần)
```

**LinkedList Methods Used:**
```cpp
// Khi tạo order
order.addItem(OrderItem)
  → LinkedList.push_back(item)

// Khi hiển thị order
for (const auto& item : order.items)
  → LinkedList iterator
```

---

## Layer 4: Utilities Layer

### FileIO (Load/Save)

```
┌─────────────────────────┐
│   FileIO::loadUsers()   │
├─────────────────────────┤
│ Đọc users.txt           │
│ Parse "u|p|n|r|a"       │
│ UserManager.insert()    │
└─────────────────────────┘

┌─────────────────────────┐
│   FileIO::loadMenu()    │
├─────────────────────────┤
│ Đọc menu.txt            │
│ Parse "id|n|p|c|d|a"    │
│ MenuManager.addItem()   │
│ MenuManager.sortByName()│ ← QuickSort
└─────────────────────────┘

┌─────────────────────────┐
│   FileIO::loadOrders()  │
├─────────────────────────┤
│ Đọc orders.txt          │
│ Parse "oid|uid|s|t|v|.."│
│ OrderManager.insert()   │ ← BST
└─────────────────────────┘
```

### AlgorithmHelper

```
sortMenuByPriceHeap()       → heapSort()
sortOrdersByTimeNewest()    → mergeSort()
getTopPriceItems()          → selectionSort()
sortMenuByNameBubble()      → bubbleSort()
```

---

## Data Flow Diagrams

### Scenario 1: App Startup

```
main.cpp
  ↓
Initialize managers
  ↓
FileIO::loadUsers()      → HashTable
FileIO::loadMenu()       → Array + QuickSort
FileIO::loadOrders()     → BST + LinkedList
  ↓
ConsoleUI.run()
```

### Scenario 2: Create Order (Customer)

```
ConsoleUI.createOrder()
  ↓
Order order(nextId, userId)
  ↓
Loop:
  - Select item
  - Input quantity
  - order.addItem()        ← LinkedList.push_back()
  ↓
order.recalculateTotal()
  ↓
OrderManager.insert()      ← BST
  ↓
KitchenManager.addTask()   ← PriorityQueue
  ↓
Save to file
```

### Scenario 3: Staff Process Queue

```
KitchenManager.getTopTask()
  ↓
task = popTask()           ← PriorityQueue
  ↓
Order* = OrderManager.find(taskId)
  ↓
order.status = READY
  ↓
Save orders.txt
```

### Scenario 4: View Reports

```
Admin → Statistics
  ↓
┌─ Order Statistics
│  OrderManager.countByStatus()  ← O(n) inorder
│  OrderManager.getTotalRevenue()
│
├─ Orders History
│  OrderManager.forEachOrderInorder()
│  AlgorithmHelper.sortOrdersByTimeNewest()  ← MergeSort
│  Display sorted
│
└─ Top Price Items
   MenuManager.getAvailableItems()
   AlgorithmHelper.getTopPriceItemsSelection()  ← SelectionSort
   Display top 5
```

---

## Memory Layout

### Stack vs Heap

```
Stack:
┌─────────────────────────────┐
│ UserManager userManager     │ ← Managers
│ MenuManager menuManager     │
│ OrderManager orderManager   │
│ KitchenManager kitchenMgr   │
└─────────────────────────────┘

Heap:
┌─────────────────────────────┐
│ HashTable buckets[]         │ ← User data
│  └─ AVL trees              │
└─────────────────────────────┘

┌─────────────────────────────┐
│ MenuItem* items             │ ← Menu array
│ (dynamic allocation)        │
└─────────────────────────────┘

┌─────────────────────────────┐
│ BST<Order> orders           │ ← Order tree
│  └─ Node pointers           │
│     └─ LinkedList items     │
└─────────────────────────────┘

┌─────────────────────────────┐
│ PriorityQueue<KitchenTask>  │ ← Kitchen queue
│  └─ vector<KitchenTask>     │
└─────────────────────────────┘
```

---

## Performance Analysis

### Load Time Complexity

```
Operation                   Complexity   Time (approx)
─────────────────────────────────────────────────────
Load users.txt             O(n)         < 10ms
Load menu.txt + QuickSort  O(n log n)   < 50ms
Load orders.txt            O(n log n)   < 100ms
Total startup              O(n log n)   < 200ms
```

### Runtime Complexity

```
Operation                          Complexity    Notes
──────────────────────────────────────────────────────
Create order                       O(log n)      BST insert
View menu                          O(n)          Display all
Search menu by name (binary)       O(log n)      If sorted
Get customer orders                O(n)          Filter from BST
Get orders by status               O(n)          Filter from BST
Add item to order                  O(1)          LinkedList push_back
Get top kitchen task               O(1)          PriorityQueue peek
Pop kitchen task                   O(log n)      PriorityQueue pop
View menu by price (HeapSort)      O(n log n)    Ad-hoc sorting
View orders history (MergeSort)    O(n log n)    Ad-hoc sorting
```

---

## Extension Points

### Thêm Feature Mới

1. **Thêm Inventory Management**
   ```
   InventoryManager:
   - HashMap<itemId, quantity>
   - Decrease on order create
   - Increase on restock
   ```

2. **Thêm Customer Loyalty**
   ```
   UserManager:
   - Add `points` field
   - PriorityQueue based on VIP + points
   ```

3. **Thêm Payment Processing**
   ```
   PaymentManager:
   - Track payments
   - BST by date + status
   ```

4. **Thêm Async Kitchen**
   ```
   Multi-threaded processing:
   - KitchenManager với thread-safe PriorityQueue
   - Consumer threads cho staff
   ```

---

## Design Principles Applied

1. **Separation of Concerns**
   - UI ↔ Business Logic ↔ Data Models ↔ Utilities

2. **Single Responsibility**
   - UserManager: chỉ quản lý users
   - MenuManager: chỉ quản lý menu
   - OrderManager: chỉ quản lý orders

3. **Appropriate Data Structure Selection**
   - HashTable: O(1) lookup cần
   - BST: O(log n) + ordered traversal cần
   - PriorityQueue: top element cần
   - LinkedList: dynamic sizing cần

4. **Algorithm Efficiency**
   - QuickSort: load lần đầu (predictable large)
   - InsertionSort: update nhỏ (already sorted)
   - Specialized sorts: for specific views

---

## Testing Strategy

### Unit Level
```
- UserManager authentication
- MenuManager sorting
- OrderManager CRUD
- KitchenManager priority
```

### Integration Level
```
- Scenario: Create order → Kitchen queue → Mark ready
- Scenario: Load data → Display → Modify → Save
```

### Performance Level
```
- Load 1000+ items
- Create 100+ orders
- Kitchen queue stress test
```

---

## Conclusion

Kiến trúc này đảm bảo:
- ✓ Clean separation of concerns
- ✓ Appropriate data structures
- ✓ Efficient algorithms
- ✓ Scalable design
- ✓ Easy to maintain and extend

# Restaurant Management System - Documentation Index

## 📚 Quick Navigation

### Getting Started
1. **[README.md](README.md)** - Overview & Quick Start
   - Kiến trúc tổng quan
   - Các cấu trúc dữ liệu sử dụng
   - Hướng dẫn cơ bản
   - Tài khoản mặc định

2. **[BUILD_TESTING.md](BUILD_TESTING.md)** - Compilation & Testing
   - Yêu cầu hệ thống
   - Cách build ứng dụng
   - Unit testing
   - Integration testing
   - Troubleshooting

### Deep Dive
3. **[ARCHITECTURE.md](ARCHITECTURE.md)** - Thiết Kế Chi Tiết
   - Kiến trúc phân tầng
   - Mô tả từng layer
   - Data flow diagrams
   - Performance analysis
   - Memory layout

4. **[ALGORITHM_USAGE.md](ALGORITHM_USAGE.md)** - Sử Dụng Thuật Toán
   - Chi tiết QuickSort
   - Chi tiết InsertionSort
   - Chi tiết HeapSort
   - Chi tiết MergeSort
   - Chi tiết SelectionSort
   - Bảng so sánh
   - Testing commands

---

## 📁 Project Structure

```
DSA_Application/
├── README.md                 ← START HERE
├── BUILD_TESTING.md         ← How to build & test
├── ARCHITECTURE.md          ← System design details
├── ALGORITHM_USAGE.md       ← Algorithm explanation
├── INDEX.md                 ← This file
├── build.sh                 ← Build script
│
├── main.cpp                 ← Entry point
│
├── lib/                     ← DSA Library (given)
│   ├── Algorithms.hpp       ← Sort algorithms
│   ├── LinkedList.hpp       ← Linked list impl
│   ├── Queue.hpp            ← Queue impl
│   ├── Stack.hpp            ← Stack impl
│   ├── PriorityQueue.hpp    ← Priority queue impl
│   ├── HashTable.hpp        ← Hash table impl
│   ├── BST.hpp              ← Binary search tree
│   ├── AVL.hpp              ← AVL tree
│   └── Detail.hpp           ← Helper templates
│
├── app/                     ← Application code
│   ├── models/              ← Data structures
│   │   ├── User.hpp         ← User model
│   │   ├── MenuItem.hpp     ← Menu item model
│   │   ├── OrderItem.hpp    ← Order item model
│   │   ├── Order.hpp        ← Order model (LinkedList)
│   │   └── KitchenTask.hpp  ← Kitchen task model
│   │
│   ├── managers/            ← Business logic
│   │   ├── UserManager.hpp  ← HashTable
│   │   ├── MenuManager.hpp  ← Dynamic array + sorting
│   │   ├── OrderManager.hpp ← BST
│   │   └── KitchenManager.hpp ← PriorityQueue
│   │
│   ├── utils/               ← Utilities
│   │   ├── FileIO.hpp       ← File operations
│   │   ├── StringUtils.hpp  ← String helpers
│   │   ├── TimeUtils.hpp    ← Time helpers
│   │   └── AlgorithmHelper.hpp ← Algorithm wrappers
│   │
│   └── ui/                  ← User interface
│       └── ConsoleUI.hpp    ← Console UI
│
├── data/                    ← Data files
│   ├── users.txt            ← User accounts
│   ├── menu.txt             ← Menu items
│   └── orders.txt           ← Order history
│
└── restaurant_app           ← Compiled executable
```

---

## 🎯 Quick Start

### 1. Build
```bash
cd /workspaces/DSA_Application
./build.sh
```

### 2. Run
```bash
./restaurant_app
```

### 3. Login
```
Username: admin
Password: admin123
```

### 4. Explore Features
- Admin: Menu Management, Statistics, User Management
- Staff: Kitchen Queue, Cooking Tasks
- Customer: View Menu, Create Orders, View My Orders

---

## 📊 Data Structures Used

| Component | Data Structure | Location | Why |
|-----------|---|---|---|
| Accounts | HashTable | UserManager | O(1) lookup |
| Menu | Array + Sort | MenuManager | O(1) access + sequential |
| Orders | BST | OrderManager | O(log n) + inorder |
| Kitchen Queue | PriorityQueue | KitchenManager | O(1) top + priority |
| Order Items | LinkedList | Order model | Dynamic sizing |

---

## 🔄 Algorithm Usage

| Algorithm | Location | When | Why |
|---|---|---|---|
| QuickSort | MenuManager | Load menu file | O(n log n) general purpose |
| InsertionSort | MenuManager | Add 1-2 items | O(n) for small additions |
| HeapSort | AlgorithmHelper | View by price | O(n log n) guaranteed |
| MergeSort | AlgorithmHelper | View history | O(n log n) stable |
| SelectionSort | AlgorithmHelper | Top K items | O(n) for selection |
| BubbleSort | AlgorithmHelper | Available for use | Simple, pedagogical |

---

## 🚀 Key Features

### Admin Panel
- **Menu Management**: View, add, remove items
  - View Menu (alphabetical, QuickSort on load)
  - View Menu by Price (HeapSort)
  - Add Multiple Items (InsertionSort)
- **Statistics**:
  - Order stats and revenue
  - Order history (MergeSort by time)
  - Top price items (SelectionSort)
- **User Management**: Create staff accounts

### Staff Panel
- **Kitchen Queue**: View tasks from PriorityQueue
- **Cooking Tasks**: Process orders with priority (VIP first)

### Customer Panel
- **View Menu**: Browse all items
- **Create Order**: Add items to LinkedList
- **View Orders**: See personal order history from BST

---

## 📖 Documentation Levels

### Level 1: Overview (README.md)
- What is this project?
- How to build and run?
- Quick feature overview

### Level 2: Building (BUILD_TESTING.md)
- System requirements
- Build instructions
- Testing procedures
- Troubleshooting

### Level 3: Architecture (ARCHITECTURE.md)
- System design
- Module responsibilities
- Data flow
- Performance analysis

### Level 4: Algorithms (ALGORITHM_USAGE.md)
- Each algorithm detailed
- Why chosen
- How implemented
- Performance characteristics

---

## 💻 Technology Stack

- **Language**: C++17
- **Build**: g++/clang++ compiler
- **Data Structures**: Custom implementation (lib/)
- **UI**: Console (text-based)
- **Storage**: Text files (users.txt, menu.txt, orders.txt)

---

## ✅ Implementation Checklist

### Models ✓
- [x] User.hpp - with roles
- [x] MenuItem.hpp - menu items
- [x] OrderItem.hpp - items in orders
- [x] Order.hpp - with LinkedList items
- [x] KitchenTask.hpp - for PriorityQueue

### Managers ✓
- [x] UserManager - HashTable
- [x] MenuManager - Array + Sorting
- [x] OrderManager - BST
- [x] KitchenManager - PriorityQueue

### Utils ✓
- [x] FileIO - Load/save data
- [x] StringUtils - String parsing
- [x] TimeUtils - Time handling
- [x] AlgorithmHelper - Sort wrappers

### UI ✓
- [x] ConsoleUI - Main interface
- [x] Admin menu
- [x] Staff menu
- [x] Customer menu

### Algorithms ✓
- [x] QuickSort - load menu
- [x] InsertionSort - add items
- [x] HeapSort - view by price
- [x] MergeSort - view history
- [x] SelectionSort - top items
- [x] BubbleSort - utility sort

### Documentation ✓
- [x] README.md
- [x] ARCHITECTURE.md
- [x] ALGORITHM_USAGE.md
- [x] BUILD_TESTING.md
- [x] INDEX.md (this file)

---

## 🔗 Important Files

| File | Purpose | Key Content |
|------|---------|---|
| main.cpp | Entry point | Initialization & main loop |
| app/ui/ConsoleUI.hpp | Main interface | All menus |
| app/managers/MenuManager.hpp | Menu logic | Array + QuickSort + InsertionSort |
| app/managers/OrderManager.hpp | Order logic | BST |
| app/managers/KitchenManager.hpp | Kitchen logic | PriorityQueue |
| app/utils/AlgorithmHelper.hpp | Sort algorithms | HeapSort, MergeSort, SelectionSort |

---

## 📝 Sample Workflows

### Workflow 1: Create Order (Customer)
```
Login as customer1
  ↓
View Menu (items from MenuManager array)
  ↓
Create Order
  ↓
Select items (added to LinkedList)
  ↓
Order inserted into OrderManager BST
  ↓
KitchenTask added to PriorityQueue
  ↓
Kitchen Queue shows task
```

### Workflow 2: View Reports (Admin)
```
Login as admin
  ↓
Statistics & Reports menu
  ↓
Orders History:
  - Get all orders from BST (inorder)
  - MergeSort by time (newest first)
  - Display
  ↓
Top Price Items:
  - Get available items from MenuManager
  - SelectionSort by price
  - Display top 5
```

### Workflow 3: Process Kitchen (Staff)
```
Login as staff1
  ↓
View Kitchen Queue (from PriorityQueue)
  ↓
Manage Cooking Tasks
  ↓
Get top task (O(1) from PriorityQueue)
  ↓
Mark Ready:
  - Pop from PriorityQueue
  - Update Order status in BST
```

---

## 🧪 Testing Tips

### Manual Testing
1. Start fresh: Remove orders.txt
2. Login with test account
3. Try each feature
4. Check data files after operations

### Automated Testing
1. Use unit test examples in BUILD_TESTING.md
2. Create test scenarios
3. Verify output matches expectations

### Performance Testing
1. Load 1000+ items
2. Create 100+ orders
3. Run all statistics
4. Check execution time

---

## 🔧 Maintenance

### Adding New Feature
1. Create model (app/models/)
2. Create manager (app/managers/)
3. Add UI menu (app/ui/ConsoleUI.hpp)
4. Update FileIO if needed (app/utils/FileIO.hpp)
5. Update documentation

### Fixing Bugs
1. Identify which layer (UI/Manager/Model/Utils)
2. Add debug output
3. Fix in appropriate file
4. Recompile and test
5. Update documentation

### Performance Optimization
1. Profile with perf or valgrind
2. Identify bottleneck
3. Consider data structure/algorithm change
4. Benchmark improvement
5. Document rationale

---

## 📞 Support & Questions

### Common Questions

**Q: How do I reset all data?**
```bash
rm data/orders.txt
touch data/orders.txt
```

**Q: How do I add a new menu item?**
```
Login as admin
Menu Management → Add Menu Item
Enter name, price, category, description
```

**Q: Why BST instead of array for orders?**
- Need O(log n) search by ID
- Need inorder traversal for history
- More efficient than array for dynamic operations

**Q: Can I extend this for multiple restaurants?**
- Add RestaurantID field to models
- Create RestaurantManager
- Group data by restaurant

---

## 📅 Version History

| Version | Date | Changes |
|---------|------|---------|
| 1.0 | 2024 | Initial release |
| | | All 6 algorithms implemented |
| | | Full CRUD for users, menu, orders |
| | | Kitchen queue with priority |
| | | Comprehensive documentation |

---

## 📄 License

Educational project for DSA learning.

---

## 🎓 Learning Objectives

By studying this project, you'll understand:

1. **Data Structures**
   - When to use HashTable vs BST vs Array
   - LinkedList for dynamic collections
   - PriorityQueue for priority-based ordering

2. **Algorithms**
   - Different sorting algorithms
   - Time/space complexity tradeoffs
   - When each algorithm is optimal

3. **System Design**
   - Layered architecture
   - Separation of concerns
   - Data flow design

4. **C++ Programming**
   - Template metaprogramming
   - Memory management
   - Standard library usage

5. **Software Engineering**
   - Code organization
   - Documentation
   - Testing strategies

---

## 🚦 Next Steps

1. **Read README.md** - Get overview
2. **Run BUILD_TESTING.md** - Build & test
3. **Study ARCHITECTURE.md** - Understand design
4. **Read ALGORITHM_USAGE.md** - Learn algorithms
5. **Explore code** - Review implementation
6. **Extend project** - Add features

---

**Last Updated**: 2024
**Status**: Complete & Documented ✓

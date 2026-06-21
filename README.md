# Restaurant Management System

Ứng dụng quản lý nhà hàng toàn bộ sử dụng các cấu trúc dữ liệu nâng cao (DSA) được tự implement.

## Kiến Trúc Tổng Quan

### Cấu Trúc Dữ Liệu Sử Dụng

#### 1. **HashTable** - Quản lý Tài khoản
- **Vị trí**: `app/managers/UserManager.hpp`
- **Tại sao HashTable?**
  - Cần tra cứu username O(1) trung bình
  - Thêm/xóa tài khoản thường xuyên
  - Không cần duyệt có thứ tự
  - Hiệu suất tối ưu cho authentication
- **Chức năng**: 
  - `addUser()` - Thêm người dùng
  - `findUser()` - Tìm theo username
  - `authenticate()` - Xác thực login
  - `updatePassword()` - Cập nhật mật khẩu
  - `removeUser()` - Xóa tài khoản

#### 2. **Mảng Động + Sắp xếp** - Quản lý Menu
- **Vị trí**: `app/managers/MenuManager.hpp`
- **Tại sao Mảng Động?**
  - Menu được duyệt từ đầu đến cuối (hiển thị cho khách)
  - Ít thay đổi nhưng thường xuyên đọc
  - Truy cập O(1) và duyệt O(n) hiệu quả
- **Thuật toán Sắp xếp**:
  - **QuickSort**: Khi load menu từ file lần đầu (hiệu suất tổng thể tốt)
  - **InsertionSort**: Khi thêm 1-2 items mới vào menu đã sắp xếp
- **Chức năng**:
  - `addItem()` - Thêm item
  - `sortByName()` - QuickSort theo tên
  - `addItemsAndSort()` - Thêm nhiều items + InsertionSort
  - `findById()` / `findByName()` - Tìm kiếm
  - `getAvailableItems()` - Lấy danh sách items có sẵn

#### 3. **BST (Binary Search Tree)** - Quản lý Đơn Hàng
- **Vị trí**: `app/managers/OrderManager.hpp`
- **Tại sao BST?**
  - Tra cứu nhanh theo orderId (O(log n))
  - Duyệt inorder để xem lịch sử đơn hàng (O(n))
  - Thống kê và phân tích dữ liệu
  - Cân bằng giữa tìm kiếm và duyệt
- **Chức năng**:
  - `addOrder()` - Thêm đơn hàng mới
  - `findOrder()` - Tìm theo orderId
  - `getOrdersByUser()` - Danh sách order của user
  - `getOrdersByStatus()` - Lọc theo trạng thái
  - `forEachOrderInorder()` - Duyệt lịch sử
  - `getTotalRevenue()` - Tính doanh thu

#### 4. **PriorityQueue** - Quản lý Hàng Chờ Bếp
- **Vị trí**: `app/managers/KitchenManager.hpp`
- **Tại sao PriorityQueue?**
  - Lấy task có ưu tiên cao nhất O(1)
  - Thêm task mới O(log n)
  - Không cần tìm kiếm - chỉ cần top element
  - Ưu tiên: VIP → priority → order time
- **Chức năng**:
  - `addTask()` - Thêm công việc
  - `popTask()` - Lấy task ưu tiên cao nhất
  - `getTopTask()` - Xem task tiếp theo
  - `getAllTasks()` - Xem hàng chờ

#### 5. **LinkedList** - Lưu Items trong Order
- **Vị trí**: `app/models/Order.hpp`
- **Tại sao LinkedList?**
  - Số lượng items không biết trước
  - Thêm items tuần tự
  - Không cần truy cập ngẫu nhiên
  - Tiết kiệm bộ nhớ
- **Chức năng**:
  - `push_back()` - Thêm item vào order
  - Iterate qua tất cả items trong order

### Các Thuật Toán Sắp Xếp

Tất cả các thuật toán được sử dụng ít nhất một lần trong ứng dụng:

| Thuật toán | Vị trí Sử dụng | Trường Hợp |
|-----------|--------------|-----------|
| **QuickSort** | MenuManager::sortByName() | Load menu từ file |
| **InsertionSort** | MenuManager::addItemsAndSort() | Thêm 1-2 items mới |
| **HeapSort** | AlgorithmHelper::sortMenuByPriceHeap() | View Menu by Price (Admin) |
| **MergeSort** | AlgorithmHelper::sortOrdersByTimeNewest() | View Orders History (Admin) |
| **SelectionSort** | AlgorithmHelper::getTopPriceItemsSelection() | Top Price Items (Admin) |
| **BubbleSort** | AlgorithmHelper::sortMenuByNameBubble() | Sắp xếp menu nhỏ |

### Cấu Trúc Thư Mục

```
DSA_Application/
├── lib/                          # Thư viện cấu trúc dữ liệu
│   ├── Algorithms.hpp            # Các thuật toán sắp xếp
│   ├── LinkedList.hpp            # Danh sách liên kết
│   ├── Queue.hpp                 # Queue cơ bản
│   ├── Stack.hpp                 # Stack
│   ├── PriorityQueue.hpp         # Priority Queue
│   ├── HashTable.hpp             # Hash Table
│   ├── BST.hpp                   # Binary Search Tree
│   ├── AVL.hpp                   # AVL Tree (dùng trong HashTable)
│   └── Detail.hpp                # Helper classes
│
├── app/                          # Ứng dụng chính
│   ├── models/                   # Định nghĩa dữ liệu
│   │   ├── User.hpp              # User với role
│   │   ├── MenuItem.hpp          # Menu item
│   │   ├── OrderItem.hpp         # Item trong order
│   │   ├── Order.hpp             # Order (chứa LinkedList<OrderItem>)
│   │   └── KitchenTask.hpp       # Task bếp (cho PriorityQueue)
│   │
│   ├── managers/                 # Quản lý dữ liệu
│   │   ├── UserManager.hpp       # HashTable<username, User>
│   │   ├── MenuManager.hpp       # MenuItem[] + Sắp xếp
│   │   ├── OrderManager.hpp      # BST<Order>
│   │   └── KitchenManager.hpp    # PriorityQueue<KitchenTask>
│   │
│   ├── utils/                    # Tiện ích
│   │   ├── FileIO.hpp            # Đọc/ghi file
│   │   ├── StringUtils.hpp       # Xử lý string
│   │   ├── TimeUtils.hpp         # Xử lý thời gian
│   │   └── AlgorithmHelper.hpp   # Wrapper cho các thuật toán
│   │
│   └── ui/                       # Giao diện console
│       └── ConsoleUI.hpp         # Main UI class
│
├── data/                         # Dữ liệu
│   ├── users.txt                 # Danh sách tài khoản
│   ├── menu.txt                  # Danh sách menu
│   └── orders.txt                # Danh sách đơn hàng
│
├── main.cpp                      # Entry point
└── CMakeLists.txt / Makefile     # Build configuration
```

## Hướng Dẫn Sử Dụng

### Biên Dịch

```bash
cd /workspaces/DSA_Application
g++ -std=c++17 -I. main.cpp -o restaurant_app
```

### Chạy Ứng Dụng

```bash
./restaurant_app
```

### Tài Khoản Mặc Định

- **Admin**: username=`admin`, password=`admin123`
- **Staff**: username=`staff1`, password=`staff123`
- **Customer**: username=`customer1`, password=`cust123`

### Các Chức Năng Chính

#### Admin Panel
1. **Menu Management**
   - View Menu (danh sách menu theo tên)
   - View Menu by Price (HeapSort - từ cao đến thấp)
   - Add Menu Item
   - Add Multiple Items (với InsertionSort)
   - Remove Item
2. **View Orders**
   - View All Orders (duyệt BST inorder)
   - View Orders by Status
3. **Statistics & Reports**
   - Order Statistics
   - Orders History (MergeSort - mới nhất trước)
   - Top Price Items (SelectionSort)
4. **Manage Users**
   - Create Staff Account

#### Staff Panel
1. **View Kitchen Queue** (từ PriorityQueue)
2. **Manage Cooking Tasks**
   - Xem task tiếp theo
   - Mark as Ready (xóa từ queue)

#### Customer Panel
1. **View Menu**
2. **Create Order** (thêm vào LinkedList)
3. **View My Orders** (BST getOrdersByUser)

## Lưu Trữ Dữ Liệu

### Format File

#### users.txt
```
username|password|fullName|role|active
admin|admin123|Administrator|ADMIN|1
```

#### menu.txt
```
id|name|price|category|description|available
1001|Spring Rolls|50000|APPETIZER|Cuộn không lò|1
```

#### orders.txt
```
orderId|userId|status|timestamp|VIP|itemId:qty|itemId:qty|...
1001|customer1|DELIVERED|1718627400|0|1001:2|1002:1
```

## Sơ Đồ Dữ Liệu

### Luồng Tạo Order

```
Customer tạo Order
  ↓
Order chứa LinkedList<OrderItem>
  ↓
Thêm vào OrderManager (BST)
  ↓
Tạo KitchenTask, thêm vào KitchenManager (PriorityQueue)
  ↓
Staff lấy task từ top PriorityQueue
  ↓
Đánh dấu Ready → cập nhật status Order trong BST
```

### Hiển Thị Dữ Liệu

```
Admin xem lịch sử:
  ↓
Lấy tất cả Orders từ BST (inorder)
  ↓
Sắp xếp bằng MergeSort (mới nhất trước)
  ↓
Hiển thị
```

## Yêu Cầu Cài Đặt

- C++17 trở lên
- g++ hoặc clang++
- Linux / macOS / Windows (WSL)

## Tổng Kết Sử Dụng DSA

| DSA Component | Lý do Chọn | Lợi Ích |
|---|---|---|
| HashTable | Tra cứu username nhanh | O(1) authentication |
| Mảng Động | Menu thường duyệt từ đầu | O(1) access, O(n) traversal |
| QuickSort | Load menu toàn bộ | Hiệu suất tốt O(n log n) |
| InsertionSort | Thêm vài items | Không cần sort toàn bộ O(n) |
| BST | Lưu Orders | O(log n) search, O(n) ordered traversal |
| PriorityQueue | Hàng chờ bếp | O(1) getMax, O(log n) insert |
| LinkedList | Items trong Order | Linh hoạt, tiết kiệm bộ nhớ |
| HeapSort | Sắp xếp menu theo giá | Hiệu suất O(n log n) |
| MergeSort | Lịch sử orders | Ổn định, O(n log n) |
| SelectionSort | Top items | Đơn giản, tìm top-k |

## Lưu Ý Về Hiệu Suất

1. **QuickSort vs InsertionSort**: 
   - Load 10 items từ file → QuickSort
   - Thêm 1-2 items vào 10 items đã sort → InsertionSort

2. **BST Duyệt Inorder**: 
   - Khi cần lịch sử orders với thứ tự, duyệt BST inorder rồi sort thêm với MergeSort

3. **PriorityQueue vs sort toàn bộ**: 
   - Kitchen queue luôn cần task ưu tiên cao nhất → PriorityQueue tốt hơn

## Phát Triển Tiếp Theo

- Thêm transaction history cho từng user
- Thêm inventory management
- Thêm report generation
- Thêm multi-threaded kitchen processing
- Thêm database backend thay vì file

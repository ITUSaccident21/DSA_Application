# Tài Liệu Chi Tiết Sử Dụng Các Thuật Toán

## 1. QuickSort - Load Menu từ File

### Vị Trí
- **File**: `app/managers/MenuManager.hpp`
- **Hàm**: `sortByName()`

### Tại Sao?
- Khi ứng dụng khởi động, cần sắp xếp 10+ items từ file
- QuickSort có hiệu suất trung bình O(n log n) tốt nhất
- Worst case O(n²) hiếm gặp với dữ liệu random

### Cách Sử Dụng
```cpp
// Trong FileIO::loadMenu()
FileIO::loadMenu(menuFile, menuManager);
// → Tự động gọi quickSort trong loadMenu
```

### Luồng Thực Thi
```
Load 10 items từ file
  ↓
quickSort(items.begin(), items.end())
  ↓
Partition + recursion
  ↓
Menu sắp xếp theo tên A-Z
```

---

## 2. InsertionSort - Thêm Items Mới

### Vị Trí
- **File**: `app/managers/MenuManager.hpp`
- **Hàm**: `addItemsAndSort()`

### Tại Sao?
- Khi thêm 1-2 items mới vào menu đã có 10+ items
- InsertionSort O(n) nếu item được chèn vào đúng vị trí
- Tốt hơn QuickSort O(n log n) cho danh sách nhỏ

### Cách Sử Dụng
```cpp
// Admin menu → Add Multiple Items
MenuItem newItems[2] = {...};
menuManager.addItemsAndSort(newItems, 2);
```

### Luồng Thực Thi
```
Có 10 items sắp xếp: [A, B, C, ...]
  ↓
Thêm 2 items mới
  ↓
insertionSort() chèn từng item vào vị trí đúng
  ↓
Menu vẫn sắp xếp O(10+2) << O(12 log 12)
```

### Benchmark
```
- QuickSort 10+2 items: ~60 phép toán
- InsertionSort 10+2 items: ~25 phép toán (chèn ngay)
```

---

## 3. HeapSort - Xem Menu Theo Giá (Cao đến Thấp)

### Vị Trí
- **File**: `app/utils/AlgorithmHelper.hpp`
- **Hàm**: `sortMenuByPriceHeap()`

### Tại Sao?
- Hiển thị menu theo giá (cao đến thấp)
- HeapSort O(n log n) guaranteed, không worst case
- Không cần bộ nhớ phụ như MergeSort

### Cách Sử Dụng
```cpp
// Admin → Menu Management → View Menu by Price
auto sortedByPrice = AlgorithmHelper::sortMenuByPriceHeap(items);
// → Hiển thị items từ giá cao đến thấp
```

### Luồng Thực Thi
```
Items: [Spring Rolls(50k), Pho(120k), Banh Mi(80k), ...]
  ↓
heapSort() theo price descending
  ↓
Heap structure: Max heap
  ↓
Output: [Pho(120k), Banh Mi(80k), Spring Rolls(50k), ...]
```

---

## 4. MergeSort - Lịch Sử Orders (Mới Nhất Trước)

### Vị Trí
- **File**: `app/utils/AlgorithmHelper.hpp`
- **Hàm**: `sortOrdersByTimeNewest()`

### Tại Sao?
- Duyệt BST inorder cho tất cả orders (không đảm bảo thứ tự thời gian)
- MergeSort O(n log n) stable, duy trì thứ tự ban đầu cho items cùng thời gian
- Hiệu suất predictable

### Cách Sử Dụng
```cpp
// Admin → Statistics → Orders History
std::vector<Order*> allOrders = // từ OrderManager inorder
auto sorted = AlgorithmHelper::sortOrdersByTimeNewest(allOrders);
// → Hiển thị mới nhất trước
```

### Luồng Thực Thi
```
Orders từ BST inorder: [#1001(2h ago), #1002(3h ago), #1003(1h ago)]
  ↓
mergeSort() theo timestamp descending
  ↓
Merge: so sánh timestamp
  ↓
Output: [#1003(1h ago), #1001(2h ago), #1002(3h ago)]
```

### Tại Sao MergeSort Thay Vì QuickSort?
```
Stable: Nếu 2 orders cùng thời gian, vẫn giữ thứ tự ban đầu
O(n log n) guaranteed: Không worry về worst case
```

---

## 5. SelectionSort - Top Price Items

### Vị Trí
- **File**: `app/utils/AlgorithmHelper.hpp`
- **Hàm**: `getTopPriceItemsSelection()`

### Tại Sao?
- Tìm top 5 items có giá cao nhất
- SelectionSort O(n) để tìm top-k (so sánh n+k lần)
- Đơn giản, không cần heapify

### Cách Sử Dụng
```cpp
// Admin → Statistics → Top Price Items
auto topItems = AlgorithmHelper::getTopPriceItemsSelection(items, 5);
// → Hiển thị 5 items đắt nhất
```

### Luồng Thực Thi
```
Items: [50k, 120k, 80k, 150k, 160k, 70k, 180k, ...]
  ↓
selectionSort() theo price descending
  ↓
Selection: Tìm max, đưa lên đầu
Iteration 1: max = 180k → pos 0
Iteration 2: max = 160k → pos 1
Iteration 3: max = 150k → pos 2
Iteration 4: max = 120k → pos 3
Iteration 5: max = 80k → pos 4
  ↓
Return top 5: [180k, 160k, 150k, 120k, 80k]
```

### So Sánh Với HeapSort cho TopK
```
SelectionSort: Đơn giản, dễ hiểu, O(n) cho toàn bộ
HeapSort: O(n log n), tốt khi k << n
QuickSelect: O(n) average cho top-k, nhưng phức tạp
```

---

## 6. BubbleSort - Sắp Xếp Danh Sách Nhỏ (Future Use)

### Vị Trí
- **File**: `app/utils/AlgorithmHelper.hpp`
- **Hàm**: `sortMenuByNameBubble()`

### Tại Sao?
- Dùng cho danh sách rất nhỏ (< 10 items)
- Đơn giản để debug
- Dễ thêm custom comparators

### Khi Sử Dụng
```cpp
// Sắp xếp danh sách nhỏ (< 10 items)
auto sorted = AlgorithmHelper::sortMenuByNameBubble(smallList);
```

---

## Bảng So Sánh Thuật Toán

| Thuật toán | Best | Average | Worst | Space | Stable | Khi Dùng |
|-----------|------|---------|-------|-------|--------|----------|
| QuickSort | O(n log n) | O(n log n) | O(n²) | O(log n) | Không | Load lớn |
| InsertionSort | O(n) | O(n²) | O(n²) | O(1) | Có | Thêm nhỏ |
| HeapSort | O(n log n) | O(n log n) | O(n log n) | O(1) | Không | Yêu cầu guaranteed |
| MergeSort | O(n log n) | O(n log n) | O(n log n) | O(n) | Có | Cần stable |
| SelectionSort | O(n²) | O(n²) | O(n²) | O(1) | Không | Top-k |
| BubbleSort | O(n) | O(n²) | O(n²) | O(1) | Có | Debug nhỏ |

---

## Các Scenario Trong Ứng Dụng

### Scenario 1: App Startup
```
main.cpp
  ↓
FileIO::loadMenu(menuFile, menuManager)
  ↓
MenuManager::sortByName() [QuickSort]
  ↓
Menu ready, sorted A-Z
```

### Scenario 2: Admin Add Items
```
Admin → Menu Management → Add Multiple Items
  ↓
Input 3 new items
  ↓
MenuManager::addItemsAndSort()
  ↓
insertionSort() [Chèn từng item]
  ↓
Menu updated, still sorted
```

### Scenario 3: Admin View Reports
```
Admin → Statistics
  ↓
1. Order Statistics [Direct count, no sort]
2. Orders History → AlgorithmHelper::sortOrdersByTimeNewest()
   ↓
   mergeSort() [Mới nhất trước]
3. Top Price Items → AlgorithmHelper::getTopPriceItemsSelection()
   ↓
   selectionSort() [Top 5]
```

### Scenario 4: Staff Manages Queue
```
OrderManager → KitchenManager (PriorityQueue)
  ↓
KitchenTask so sánh: VIP > priority > time
  ↓
Staff lấy top task từ PriorityQueue (Max-Heap)
  ↓
Task được remove từ queue
```

---

## Testing Commands

### Build
```bash
./build.sh
```

### Run
```bash
./restaurant_app
```

### Test QuickSort + InsertionSort
```
1. Login as admin/admin123
2. Goto Menu Management
3. View Menu [QuickSort applied on load]
4. Add Multiple Items [InsertionSort applied]
```

### Test HeapSort
```
1. Admin → Menu Management
2. Select "View Menu by Price" [HeapSort applied]
```

### Test MergeSort
```
1. Admin → Statistics & Reports
2. Select "Orders History" [MergeSort applied]
```

### Test SelectionSort
```
1. Admin → Statistics & Reports
2. Select "Top Price Items" [SelectionSort applied]
```

---

## Kết Luận

Tất cả 6 thuật toán sắp xếp được sử dụng trong các scenario thực tế:
- **QuickSort**: Load data khởi tạo
- **InsertionSort**: Update dữ liệu đã sắp xếp
- **HeapSort**: View alternative ordering
- **MergeSort**: Stable sort cho lịch sử
- **SelectionSort**: Top-K selection
- **BubbleSort**: Simple sorting cho testing

Mỗi lựa chọn được dự định dựa trên:
1. Kích thước dữ liệu
2. Tính chất dữ liệu (đã sort hay không)
3. Yêu cầu (stable, guaranteed O(n log n), etc.)
4. Trade-off memory vs time

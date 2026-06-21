// ============================================================
//  test_lib.cpp  --  DSA Library Test Suite
//  Restaurant Management System  |  CSC10004
//
//  Compile:
//    g++ -std=c++17 -I. test_lib.cpp -o test_lib && ./test_lib
// ============================================================

#include <iostream>
#include <string>
#include <vector>
#include <ctime>

#include "lib/HashTable.hpp"
#include "lib/BST.hpp"
#include "lib/AVL.hpp"
#include "lib/PriorityQueue.hpp"
#include "lib/Algorithms.hpp"
#include "app/models/KitchenTask.hpp"

// ── Minimal test framework ────────────────────────────────────
static int g_pass = 0, g_fail = 0;

void check(bool ok, const std::string& name) {
    if (ok) { std::cout << "  [PASS] " << name << "\n"; ++g_pass; }
    else     { std::cout << "  [FAIL] " << name << "\n"; ++g_fail; }
}

void section(const std::string& title) {
    std::cout << "\n-- " << title << "\n";
}

void header(const std::string& title) {
    std::cout << "\n";
    std::cout << "============================================================\n";
    std::cout << "  " << title << "\n";
    std::cout << "============================================================\n";
}

// ── Helper: check array is ascending ─────────────────────────
template<typename T>
bool ascending(const T* a, std::size_t n) {
    for (std::size_t i = 1; i < n; ++i)
        if (a[i] < a[i-1]) return false;
    return true;
}

template<typename T>
bool descending(const T* a, std::size_t n) {
    for (std::size_t i = 1; i < n; ++i)
        if (a[i] > a[i-1]) return false;
    return true;
}

// ============================================================
//  1. HASHTABLE
// ============================================================
void test_hashtable() {
    header("1. HashTable<string, int>  (lib/HashTable.hpp)");

    section("Basic insert / find / contains");
    ds::HashTable<std::string, int> ht;
    ht.insert("alice", 10);
    ht.insert("bob",   20);
    ht.insert("carol", 30);

    check(ht.size() == 3,
          "size() == 3 after 3 inserts");
    check(ht.find("alice") != nullptr,
          "find('alice') != nullptr  [exists]");
    check(*ht.find("alice") == 10,
          "find('alice') value == 10");
    check(ht.find("nobody") == nullptr,
          "find('nobody') == nullptr  [non-existent]");
    check(ht.contains("bob"),
          "contains('bob') == true");
    check(!ht.contains("ghost"),
          "contains('ghost') == false  [non-existent]");

    section("Duplicate insert");
    ht.insert("alice", 99);
    check(ht.size() == 3,
          "size unchanged after duplicate insert");
    check(*ht.find("alice") == 99,
          "duplicate insert updates value  (alice: 10 -> 99)");

    section("erase");
    bool ok1 = ht.erase("bob");
    check(ok1,
          "erase('bob') returns true");
    check(ht.size() == 2,
          "size() == 2 after erase");
    check(ht.find("bob") == nullptr,
          "find('bob') == nullptr after erase");

    bool ok2 = ht.erase("ghost");
    check(!ok2,
          "erase('ghost') returns false  [non-existent]");
    check(ht.size() == 2,
          "size unchanged after erasing non-existent");

    section("Boundary: erase non-existent / update password pattern");
    ds::HashTable<std::string, std::string> users;
    users.insert("admin", "admin123");

    auto* p = users.find("admin");
    check(p != nullptr && *p == "admin123",
          "find('admin') returns correct password");

    // Simulate updatePassword: find + mutate in-place
    if (p) *p = "newpass456";
    check(*users.find("admin") == "newpass456",
          "in-place password update via find() pointer works");

    // Simulate login for non-existent user
    check(users.find("hacker") == nullptr,
          "find non-existent user returns nullptr  [login guard]");

    section("clear");
    ht.clear();
    check(ht.size() == 0,
          "size() == 0 after clear()");
    check(ht.find("alice") == nullptr,
          "find after clear() returns nullptr");
}

// ============================================================
//  2. BST
// ============================================================

// Simple stand-in for Order
struct Order { int id; double total; };
struct OrderKey { int operator()(const Order& o) const { return o.id; } };

using OrderBST = ds::BST<Order, std::less<int>, OrderKey>;

void test_bst() {
    header("2. BST<Order>  (lib/BST.hpp)  --  key = orderId");

    section("Insert and find  [non-sequential IDs]");
    OrderBST bst;
    bst.insert({1003, 300.0});
    bst.insert({1001, 100.0});
    bst.insert({1005, 500.0});
    bst.insert({1002, 200.0});
    bst.insert({1004, 400.0});

    check(bst.size() == 5,
          "size() == 5 after 5 inserts");
    check(bst.find(1003) != nullptr,
          "find(1003) != nullptr  [exists]");
    check(bst.find(1003)->total == 300.0,
          "find(1003)->total == 300.0");
    check(bst.find(9999) == nullptr,
          "find(9999) == nullptr  [non-existent]");

    section("Inorder traversal  [must be ascending by orderId]");
    std::vector<int> ids;
    bst.inorder([&](Order& o){ ids.push_back(o.id); });
    bool asc = (ids == std::vector<int>{1001, 1002, 1003, 1004, 1005});
    check(asc, "inorder ids = {1001, 1002, 1003, 1004, 1005}");
    std::cout << "    actual: ";
    for (int x : ids) std::cout << x << " ";
    std::cout << "\n";

    section("Revenue via inorder lambda");
    double rev = 0;
    bst.inorder([&](Order& o){ rev += o.total; });
    check(rev == 1500.0, "inorder sum of totals == 1500.0");
    std::cout << "    actual sum: " << rev << "\n";

    section("remove leaf  [1001 has no children after above inserts]");
    bool r1 = bst.remove(1001);
    check(r1,                          "remove(1001) returns true");
    check(bst.find(1001) == nullptr,   "find(1001) == nullptr after remove");
    check(bst.size() == 4,             "size() == 4 after remove leaf");

    section("remove node with one child  [1002]");
    bool r2 = bst.remove(1002);
    check(r2,                          "remove(1002) returns true");
    check(bst.find(1002) == nullptr,   "find(1002) == nullptr after remove");
    check(bst.size() == 3,             "size() == 3");

    section("remove node with two children  [1003 = original root]");
    bool r3 = bst.remove(1003);
    check(r3,                          "remove(1003) returns true  [successor replacement]");
    check(bst.find(1003) == nullptr,   "find(1003) == nullptr after remove");
    check(bst.size() == 2,             "size() == 2");

    std::vector<int> after;
    bst.inorder([&](Order& o){ after.push_back(o.id); });
    check(after == std::vector<int>{1004, 1005},
          "inorder after removes = {1004, 1005}");

    section("remove non-existent");
    bool rn = bst.remove(9999);
    check(!rn,          "remove(9999) returns false");
    check(bst.size() == 2, "size unchanged");

    section("Sequential orderId insert  [degeneration risk]");
    // App creates orderId 1001, 1002, 1003... monotonically
    ds::BST<int> bst2;
    for (int i = 1001; i <= 1007; ++i) bst2.insert(i);

    check(bst2.size() == 7, "sequential insert: size() == 7  [functional]");

    std::vector<int> seq;
    bst2.inorder([&](int x){ seq.push_back(x); });
    check(seq == std::vector<int>{1001,1002,1003,1004,1005,1006,1007},
          "inorder still correct despite monotone keys");

    // Demonstrate depth: find all nodes via repeated find
    // (BST has no height() API, so we verify path length indirectly via find)
    bool depth_ok = true;
    for (int i = 1001; i <= 1007; ++i)
        if (!bst2.find(i)) { depth_ok = false; break; }
    check(depth_ok, "all 7 nodes reachable via find()");

    std::cout << "  [NOTE] Monotone orderId 1001..1007 creates right-skewed tree.\n";
    std::cout << "         Worst-case path length = 7 (O(n)), not O(log n).\n";
    std::cout << "         Replace BST with AVL in OrderManager to fix this.\n";
}

// ============================================================
//  3. PRIORITYQUEUE
// ============================================================

// Actual comparator from KitchenManager
struct KTCmp {
    bool operator()(const KitchenTask& a, const KitchenTask& b) const {
        return a > b;
    }
};

// Fixed comparator (reversed argument order)
struct KTCmpFixed {
    bool operator()(const KitchenTask& a, const KitchenTask& b) const {
        return b > a;
    }
};

void test_priorityqueue() {
    header("3. PriorityQueue  (lib/PriorityQueue.hpp)");

    section("3a. Basic max-heap with std::less<int>  [sanity check]");
    {
        ds::PriorityQueue<int> pq;
        pq.push(3); pq.push(1); pq.push(4); pq.push(1); pq.push(5);

        check(pq.size() == 5, "size() == 5 after 5 pushes");
        check(pq.top()  == 5, "top() == 5  [max element]");

        std::vector<int> out;
        while (!pq.empty()) { out.push_back(pq.top()); pq.pop(); }
        check(out == std::vector<int>{5,4,3,1,1},
              "pop order = {5,4,3,1,1}  [descending = max-heap confirmed]");
        std::cout << "    actual: ";
        for (int x : out) std::cout << x << " "; std::cout << "\n";
    }

    section("3b. pop() on empty queue  [must not crash]");
    {
        ds::PriorityQueue<int> pq;
        pq.pop();
        check(pq.empty(), "pop() on empty queue: still empty, no crash");
    }

    // Build reference tasks
    std::time_t T0 = 1000, T1 = 1005, T2 = 1010;
    KitchenTask nonvip_early (1001, 2, T0, false, 0);
    KitchenTask nonvip_late  (1002, 2, T1, false, 0);
    KitchenTask vip_early    (1003, 2, T0, true,  0);
    KitchenTask vip_late     (1004, 2, T2, true,  0);
    KitchenTask vip_hipri    (1005, 2, T0, true,  1);

    // ── Historical bug (for reference) ───────────────────────
    // KTCmp { return a > b } WAS the original comparator in KitchenManager.
    // Bug: compare(a,b)=true causes a to sink DOWN, so high-priority tasks sink.
    // Fixed by changing to { return b > a } -- see KTCmpFixed section below.
    section("3c. KTCmp (a > b)  --  HISTORICAL BUG [no longer in production]");
    std::cout << "    Semantics of sift_up: swap(parent,child) when compare_(parent,child)=true.\n";
    std::cout << "    With KTCmp(a>b): high-priority task a causes swap => a sinks, low-prio rises.\n\n";

    // Những dòng dưới đây KHÔNG dùng check() vì đây là bug đã biết.
    // Mục đích: ghi lại hành vi sai để đối chiếu với KTCmpFixed.
    {
        ds::PriorityQueue<KitchenTask, KTCmp> q;
        q.push(nonvip_early); q.push(vip_early);
        std::cout << "  [BUG] VIP on top over non-VIP? ";
        std::cout << (q.top().isVIP ? "YES (correct)" : "NO -- non-VIP rose to top!") << "\n";
        std::cout << "        top(): orderId=" << q.top().orderId
                  << "  isVIP=" << q.top().isVIP << "\n";
    }
    {
        ds::PriorityQueue<KitchenTask, KTCmp> q;
        KitchenTask a(2001,1,T0,true,0), b(2002,1,T0,true,1);
        q.push(b); q.push(a);
        std::cout << "  [BUG] VIP pri=0 on top over pri=1? ";
        std::cout << (q.top().priority == 0 ? "YES (correct)" : "NO -- wrong priority rose!") << "\n";
        std::cout << "        top(): orderId=" << q.top().orderId
                  << "  priority=" << q.top().priority << "\n";
    }
    {
        ds::PriorityQueue<KitchenTask, KTCmp> q;
        q.push(nonvip_late); q.push(vip_late); q.push(nonvip_early);
        q.push(vip_early);   q.push(vip_hipri);
        std::cout << "  [BUG] Full pop order (KTCmp -- wrong):\n        ";
        while (!q.empty()) {
            const auto& t = q.top();
            std::cout << "#" << t.orderId
                      << "(VIP=" << t.isVIP << ",pri=" << t.priority
                      << ",t=" << t.orderTime << ") ";
            q.pop();
        }
        std::cout << "\n";
        std::cout << "        Correct order: 1003 > 1004 > 1005 > 1001 > 1002\n";
        std::cout << "        Root cause: compare(a,b)=true sinks a; (a>b) sinks high-prio.\n";
        std::cout << "        Fix: use (b>a) so high-prio task b sinks low-prio task a.\n";
    }

    // ── Production comparator (fixed) ────────────────────────
    section("3d. KTCmpFixed (b > a)  --  PRODUCTION comparator [KitchenManager.hpp]");

    {
        ds::PriorityQueue<KitchenTask, KTCmpFixed> q;
        q.push(nonvip_early);
        q.push(vip_early);
        bool vip_on_top = q.top().isVIP;
        check(vip_on_top,
              "[Fixed] VIP on top over non-VIP");
        std::cout << "    top(): orderId=" << q.top().orderId
                  << "  isVIP=" << q.top().isVIP << "\n";
    }

    {
        ds::PriorityQueue<KitchenTask, KTCmpFixed> q;
        KitchenTask a(2001, 1, T0, true, 0);
        KitchenTask b(2002, 1, T0, true, 1);
        q.push(b); q.push(a);
        check(q.top().priority == 0,
              "[Fixed] VIP pri=0 on top over VIP pri=1");
        std::cout << "    top(): orderId=" << q.top().orderId
                  << "  priority=" << q.top().priority << "\n";
    }

    {
        ds::PriorityQueue<KitchenTask, KTCmpFixed> q;
        KitchenTask a(3001, 1, T0, true, 0);
        KitchenTask b(3002, 1, T2, true, 0);
        q.push(b); q.push(a);
        check(q.top().orderTime == T0,
              "[Fixed] Earlier orderTime (T0) on top");
        std::cout << "    top(): orderId=" << q.top().orderId
                  << "  orderTime=" << q.top().orderTime << "\n";
    }

    {
        ds::PriorityQueue<KitchenTask, KTCmpFixed> q;
        q.push(nonvip_late);
        q.push(vip_late);
        q.push(nonvip_early);
        q.push(vip_early);
        q.push(vip_hipri);

        std::cout << "    Full pop order (KTCmpFixed):\n";
        std::cout << "    ";
        while (!q.empty()) {
            const auto& t = q.top();
            std::cout << "#" << t.orderId
                      << "(VIP=" << t.isVIP
                      << ",pri=" << t.priority
                      << ",t="   << t.orderTime << ") ";
            q.pop();
        }
        std::cout << "\n";
        std::cout << "    Expected: vip_early(1003) > vip_late(1004) >"
                  << " vip_hipri(1005) > nonvip_early(1001) > nonvip_late(1002)\n";
    }
}

// ============================================================
//  4. ALGORITHMS  (lib/Algorithms.hpp)
// ============================================================
void test_algorithms() {
    header("4. Algorithms  (lib/Algorithms.hpp)");

    const int N = 10;
    int src[N] = {5, 2, 8, 1, 9, 3, 7, 4, 6, 0};
    int arr[N];
    auto reset = [&]{ for(int i=0;i<N;i++) arr[i]=src[i]; };

    section("Sorting -- ascending order");

    reset(); ds::quickSort    (arr, arr+N);
    check(ascending(arr,N), "quickSort     {5,2,8,1,9,3,7,4,6,0} -> ascending");
    std::cout << "    result: "; for(int x:arr) std::cout<<x<<" "; std::cout<<"\n";

    reset(); ds::insertionSort(arr, arr+N);
    check(ascending(arr,N), "insertionSort {5,2,8,1,9,3,7,4,6,0} -> ascending");
    std::cout << "    result: "; for(int x:arr) std::cout<<x<<" "; std::cout<<"\n";

    reset(); ds::heapSort     (arr, arr+N);
    check(ascending(arr,N), "heapSort      {5,2,8,1,9,3,7,4,6,0} -> ascending");
    std::cout << "    result: "; for(int x:arr) std::cout<<x<<" "; std::cout<<"\n";

    reset(); ds::mergeSort    (arr, arr+N);
    check(ascending(arr,N), "mergeSort     {5,2,8,1,9,3,7,4,6,0} -> ascending");
    std::cout << "    result: "; for(int x:arr) std::cout<<x<<" "; std::cout<<"\n";

    reset(); ds::selectionSort(arr, arr+N);
    check(ascending(arr,N), "selectionSort {5,2,8,1,9,3,7,4,6,0} -> ascending");
    std::cout << "    result: "; for(int x:arr) std::cout<<x<<" "; std::cout<<"\n";

    reset(); ds::bubbleSort   (arr, arr+N);
    check(ascending(arr,N), "bubbleSort    {5,2,8,1,9,3,7,4,6,0} -> ascending");
    std::cout << "    result: "; for(int x:arr) std::cout<<x<<" "; std::cout<<"\n";

    section("QuickSort with custom comparator (descending)");
    reset();
    ds::quickSort(arr, arr+N, [](int a, int b){ return a > b; });
    check(descending(arr,N), "quickSort with cmp(a>b) -> descending");
    std::cout << "    result: "; for(int x:arr) std::cout<<x<<" "; std::cout<<"\n";

    section("InsertionSort on nearly-sorted input");
    {
        int nearly[] = {1, 2, 3, 5, 4, 6, 7, 8, 9, 10};
        ds::insertionSort(nearly, nearly+10);
        check(ascending(nearly,10), "insertionSort nearly-sorted {1,2,3,5,4,...} -> ascending");
        std::cout << "    result: "; for(int x:nearly) std::cout<<x<<" "; std::cout<<"\n";
    }

    section("Sorting strings");
    {
        std::string menu[] = {"Pho Bo", "Bun Bo", "Com Tam", "Goi Cuon", "Banh Mi"};
        ds::quickSort(menu, menu+5, [](const std::string& a, const std::string& b){ return a < b; });
        bool ok = ascending(menu, 5);
        check(ok, "quickSort strings alphabetically");
        std::cout << "    result: "; for(auto& s:menu) std::cout<<"\""<<s<<"\" "; std::cout<<"\n";
    }

    section("Search algorithms");
    {
        int sorted[] = {1, 3, 5, 7, 9, 11, 13, 15};
        const int SN = 8;

        auto it = ds::linearSearch(sorted, sorted+SN, 7);
        check(it != sorted+SN && *it == 7,
              "linearSearch: finds 7 in {1,3,5,7,9,11,13,15}");

        it = ds::linearSearch(sorted, sorted+SN, 6);
        check(it == sorted+SN,
              "linearSearch: returns end for non-existent 6");

        it = ds::binarySearch(sorted, sorted+SN, 7);
        check(it != sorted+SN && *it == 7,
              "binarySearch: finds 7");

        it = ds::binarySearch(sorted, sorted+SN, 1);
        check(it == sorted && *it == 1,
              "binarySearch: finds first element 1");

        it = ds::binarySearch(sorted, sorted+SN, 15);
        check(it != sorted+SN && *it == 15,
              "binarySearch: finds last element 15");

        it = ds::binarySearch(sorted, sorted+SN, 6);
        check(it == sorted+SN,
              "binarySearch: returns original end for non-existent 6");
    }
}

// ============================================================
//  5. AVL  (lib/AVL.hpp)
// ============================================================
void test_avl() {
    header("5. AVL<int>  (lib/AVL.hpp)  --  used as HashTable bucket");

    section("Sequential inserts  [would degenerate BST, must stay balanced]");
    ds::AVL<int> avl;
    for (int i = 1; i <= 7; ++i) avl.insert(i);

    check(avl.size() == 7, "size() == 7 after sequential inserts 1..7");

    std::vector<int> avl_order;
    avl.inorder([&](int x){ avl_order.push_back(x); });
    check(avl_order == std::vector<int>{1,2,3,4,5,6,7},
          "inorder = {1,2,3,4,5,6,7}  [self-balancing keeps it correct]");

    check(avl.find(4) != nullptr && *avl.find(4) == 4, "find(4) == 4");
    check(avl.find(99) == nullptr, "find(99) == nullptr  [non-existent]");

    section("remove and re-check inorder");
    bool r = avl.remove(4);
    check(r, "remove(4) returns true");
    check(avl.find(4) == nullptr, "find(4) == nullptr after remove");
    check(avl.size() == 6, "size() == 6 after remove");

    std::vector<int> after_rm;
    avl.inorder([&](int x){ after_rm.push_back(x); });
    check(after_rm == std::vector<int>{1,2,3,5,6,7},
          "inorder after remove(4) = {1,2,3,5,6,7}");

    check(avl.remove(99) == false, "remove(99) returns false  [non-existent]");

    std::cout << "  [NOTE] AVL rebalances on every insert/remove (rotate_left/right).\n";
    std::cout << "         Sequential inserts 1..7 stay O(log n) -- contrast with BST.\n";
}

// ============================================================
//  main
// ============================================================
int main() {
    std::cout << "============================================================\n";
    std::cout << "  DSA Library Test Suite  |  Restaurant Management System\n";
    std::cout << "  CSC10004  |  CQ2025/4\n";
    std::cout << "============================================================\n";

    test_hashtable();
    test_bst();
    test_priorityqueue();
    test_algorithms();
    test_avl();

    std::cout << "\n============================================================\n";
    std::cout << "  TOTAL: " << g_pass << " passed  |  " << g_fail << " failed\n";
    std::cout << "============================================================\n";

    return g_fail > 0 ? 1 : 0;
}

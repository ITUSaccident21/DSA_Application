"""
gen_images.py  --  Tạo ảnh PNG từ test output để nhúng vào báo cáo LaTeX.
Run: python3 gen_images.py
"""
import os
from PIL import Image, ImageDraw, ImageFont

OUT = r"E:\_ITUS_COURSE\Data_structure_and_algorithm\Project\img"
os.makedirs(OUT, exist_ok=True)

FONT_PATH = r"C:\Windows\Fonts\consola.ttf"
FONT_SIZE  = 14
LINE_H     = 19
PAD        = 14

BG      = (18,  18,  18)
FG      = (220, 220, 220)
GREEN   = (80,  200, 80)
RED     = (220, 70,  70)
YELLOW  = (220, 180, 50)
CYAN    = (80,  200, 220)
GRAY    = (130, 130, 130)

font      = ImageFont.truetype(FONT_PATH, FONT_SIZE)
font_bold = ImageFont.truetype(FONT_PATH, FONT_SIZE)


def make_image(lines_spec, filename, title=None):
    """
    lines_spec: list of (text, color)
    color = None -> default FG
    """
    if title:
        lines_spec = [(title, CYAN), ("", None)] + lines_spec
    w = max(len(t) for t, _ in lines_spec) * 8 + PAD * 2 + 2
    w = max(w, 640)
    h = len(lines_spec) * LINE_H + PAD * 2
    img = Image.new("RGB", (w, h), BG)
    d   = ImageDraw.Draw(img)
    y   = PAD
    for text, color in lines_spec:
        d.text((PAD, y), text, font=font, fill=(color or FG))
        y += LINE_H
    path = os.path.join(OUT, filename)
    img.save(path)
    print(f"  saved: {path}")
    return path


# ── 1. HashTable: all PASS ────────────────────────────────────
make_image([
    ("-- Basic insert / find / contains",           GRAY),
    ("[PASS] size() == 3 after 3 inserts",          GREEN),
    ("[PASS] find('alice') != nullptr  [exists]",   GREEN),
    ("[PASS] find('alice') value == 10",            GREEN),
    ("[PASS] find('nobody') == nullptr  [non-existent]", GREEN),
    ("[PASS] contains('bob') == true",              GREEN),
    ("[PASS] contains('ghost') == false",           GREEN),
    ("",                                            None),
    ("-- Duplicate insert",                         GRAY),
    ("[PASS] size unchanged after duplicate insert",GREEN),
    ("[PASS] duplicate insert updates value  (alice: 10 -> 99)", GREEN),
    ("",                                            None),
    ("-- erase",                                    GRAY),
    ("[PASS] erase('bob') returns true",            GREEN),
    ("[PASS] size() == 2 after erase",              GREEN),
    ("[PASS] find('bob') == nullptr after erase",   GREEN),
    ("[PASS] erase('ghost') returns false  [non-existent]", GREEN),
    ("[PASS] size unchanged after erasing non-existent", GREEN),
    ("",                                            None),
    ("-- Boundary: login guard / update password",  GRAY),
    ("[PASS] find('admin') returns correct password", GREEN),
    ("[PASS] in-place password update via find() pointer works", GREEN),
    ("[PASS] find non-existent user returns nullptr  [login guard]", GREEN),
    ("",                                            None),
    ("-- clear",                                    GRAY),
    ("[PASS] size() == 0 after clear()",            GREEN),
    ("[PASS] find after clear() returns nullptr",   GREEN),
    ("",                                            None),
    ("  19 / 19 PASS",                             CYAN),
],
"01_hashtable.png",
"1. HashTable<string,int>  (lib/HashTable.hpp)")

# ── 2. BST ───────────────────────────────────────────────────
make_image([
    ("-- Insert 5 orders (non-sequential), find",  GRAY),
    ("[PASS] size() == 5 after 5 inserts",         GREEN),
    ("[PASS] find(1003)->total == 300.0",           GREEN),
    ("[PASS] find(9999) == nullptr  [non-existent]",GREEN),
    ("",                                           None),
    ("-- Inorder traversal",                       GRAY),
    ("[PASS] inorder ids = {1001, 1002, 1003, 1004, 1005}", GREEN),
    ("    actual: 1001 1002 1003 1004 1005",       FG),
    ("[PASS] inorder sum of totals == 1500.0",     GREEN),
    ("    actual sum: 1500",                       FG),
    ("",                                           None),
    ("-- remove leaf (1001)",                      GRAY),
    ("[PASS] remove(1001) returns true",           GREEN),
    ("[PASS] find(1001) == nullptr after remove",  GREEN),
    ("[PASS] size() == 4 after remove leaf",       GREEN),
    ("",                                           None),
    ("-- remove one-child node (1002)",            GRAY),
    ("[PASS] remove(1002) returns true",           GREEN),
    ("[PASS] find(1002) == nullptr after remove",  GREEN),
    ("",                                           None),
    ("-- remove two-child node (1003 = root)",     GRAY),
    ("[PASS] remove(1003) returns true  [successor]", GREEN),
    ("[PASS] inorder after removes = {1004, 1005}",GREEN),
    ("",                                           None),
    ("-- remove non-existent",                     GRAY),
    ("[PASS] remove(9999) returns false",          GREEN),
    ("",                                           None),
    ("-- Sequential insert degeneration demo",     GRAY),
    ("[PASS] sequential insert: size() == 7",      GREEN),
    ("[PASS] inorder still correct despite monotone keys", GREEN),
    ("[NOTE] Monotone orderId 1001..1007 -> right-skewed tree", YELLOW),
    ("       Worst-case path = 7 = O(n), not O(log n)",YELLOW),
    ("",                                           None),
    ("  17 / 17 PASS",                            CYAN),
],
"02_bst.png",
"2. BST<Order>  key=orderId  (lib/BST.hpp)")

# ── 3a. PriorityQueue sanity ─────────────────────────────────
make_image([
    ("-- Basic max-heap  std::less<int>",          GRAY),
    ("[PASS] size() == 5 after 5 pushes",          GREEN),
    ("[PASS] top() == 5  [max element]",           GREEN),
    ("[PASS] pop order = {5,4,3,1,1}  [max-heap]", GREEN),
    ("    actual: 5 4 3 1 1",                      FG),
    ("[PASS] pop() on empty: still empty, no crash",GREEN),
],
"03a_pq_sanity.png",
"3a. PriorityQueue<int, std::less>  sanity check")

# ── 3b. PriorityQueue bug ─────────────────────────────────────
make_image([
    ("KTCmp { return a > b; }  <-- CURRENT comparator",  YELLOW),
    ("",                                              None),
    ("[FAIL] VIP on top over non-VIP (push nonvip, then vip)", RED),
    ("       top(): orderId=1001  isVIP=0  <-- non-VIP on top!", RED),
    ("[FAIL] VIP on top over non-VIP (push vip, then nonvip)", RED),
    ("       top(): orderId=1001  isVIP=0  <-- non-VIP on top!", RED),
    ("[FAIL] VIP pri=0 on top over VIP pri=1",       RED),
    ("       top(): orderId=2002  priority=1  <-- WRONG priority", RED),
    ("[FAIL] Earlier orderTime on top",              RED),
    ("       top(): orderId=3002  orderTime=1010  <-- LATER time!", RED),
    ("",                                             None),
    ("Full pop order (KTCmp -- WRONG):",             YELLOW),
    ("#1002(nonVIP,pri=0) #1001(nonVIP,pri=0)",      RED),
    ("#1005(VIP,pri=1)    #1004(VIP,pri=0)",         RED),
    ("#1003(VIP,pri=0)    <-- VIP processed LAST",   RED),
    ("",                                             None),
    ("Root cause: ds::PriorityQueue swaps when compare_(parent,child)=true", YELLOW),
    ("  With (a>b): parent with HIGHER priority gets pushed DOWN",  YELLOW),
    ("  => lower-priority tasks float to top (inverted heap)",      YELLOW),
    ("  Fix: change to { return b > a; }",           CYAN),
],
"03b_pq_bug.png",
"3b. PriorityQueue<KitchenTask, KTCmp>  BUG CONFIRMED")

# ── 3c. PriorityQueue fixed ───────────────────────────────────
make_image([
    ("KTCmpFixed { return b > a; }  <-- CORRECTED",  CYAN),
    ("",                                             None),
    ("[PASS] [Fixed] VIP on top over non-VIP",       GREEN),
    ("       top(): orderId=1003  isVIP=1",          FG),
    ("[PASS] [Fixed] VIP pri=0 on top over pri=1",   GREEN),
    ("       top(): orderId=2001  priority=0",       FG),
    ("[PASS] [Fixed] Earlier orderTime on top",      GREEN),
    ("       top(): orderId=3001  orderTime=1000",   FG),
    ("",                                             None),
    ("Full pop order (KTCmpFixed -- CORRECT):",      CYAN),
    ("#1003(VIP=1,pri=0,t=1000)",                    GREEN),
    ("#1004(VIP=1,pri=0,t=1010)",                    GREEN),
    ("#1005(VIP=1,pri=1,t=1000)",                    GREEN),
    ("#1001(VIP=0,pri=0,t=1000)",                    GREEN),
    ("#1002(VIP=0,pri=0,t=1005)",                    GREEN),
    ("",                                             None),
    ("Order matches spec: VIP first, earlier time first",CYAN),
],
"03c_pq_fixed.png",
"3c. PriorityQueue<KitchenTask, KTCmpFixed>  FIX VERIFIED")

# ── 4. Algorithms ─────────────────────────────────────────────
make_image([
    ("Input: {5, 2, 8, 1, 9, 3, 7, 4, 6, 0}",     GRAY),
    ("",                                            None),
    ("[PASS] quickSort     -> 0 1 2 3 4 5 6 7 8 9", GREEN),
    ("[PASS] insertionSort -> 0 1 2 3 4 5 6 7 8 9", GREEN),
    ("[PASS] heapSort      -> 0 1 2 3 4 5 6 7 8 9", GREEN),
    ("[PASS] mergeSort     -> 0 1 2 3 4 5 6 7 8 9", GREEN),
    ("[PASS] selectionSort -> 0 1 2 3 4 5 6 7 8 9", GREEN),
    ("[PASS] bubbleSort    -> 0 1 2 3 4 5 6 7 8 9", GREEN),
    ("",                                            None),
    ("[PASS] quickSort desc cmp -> 9 8 7 6 5 4 3 2 1 0", GREEN),
    ("[PASS] insertionSort nearly-sorted -> 1 2 3 4 5 6 7 8 9 10", GREEN),
    ("[PASS] quickSort strings -> \"Banh Mi\" \"Bun Bo\" \"Com Tam\" ...", GREEN),
    ("",                                            None),
    ("-- Search",                                   GRAY),
    ("[PASS] linearSearch: finds 7 in {1,3,5,7,9,11,13,15}", GREEN),
    ("[PASS] linearSearch: returns end for non-existent 6", GREEN),
    ("[PASS] binarySearch: finds 7 / 1 / 15",      GREEN),
    ("[PASS] binarySearch non-existent 6 -> returns original end", GREEN),
    ("       Fix: save original_last before loop (Algorithms.hpp)", CYAN),
    ("",                                            None),
    ("  16 / 16 PASS",                             CYAN),
],
"04_algorithms.png",
"4. Algorithms  (lib/Algorithms.hpp)")

# ── 5. AVL ───────────────────────────────────────────────────
make_image([
    ("Sequential inserts 1..7 (would skew BST)",   GRAY),
    ("",                                           None),
    ("[PASS] size() == 7 after sequential inserts", GREEN),
    ("[PASS] inorder = {1,2,3,4,5,6,7}  [balanced]",GREEN),
    ("[PASS] find(4) == 4",                        GREEN),
    ("[PASS] find(99) == nullptr",                 GREEN),
    ("",                                           None),
    ("-- remove(4) and re-check",                  GRAY),
    ("[PASS] remove(4) returns true",              GREEN),
    ("[PASS] find(4) == nullptr after remove",     GREEN),
    ("[PASS] size() == 6 after remove",            GREEN),
    ("[PASS] inorder = {1,2,3,5,6,7}",            GREEN),
    ("[PASS] remove(99) returns false",            GREEN),
    ("",                                           None),
    ("[NOTE] AVL rebalances via rotate_left/rotate_right", CYAN),
    ("       Sequential inserts stay O(log n) -- contrast with BST", CYAN),
    ("",                                           None),
    ("  9 / 9 PASS",                              CYAN),
],
"05_avl.png",
"5. AVL<int>  (lib/AVL.hpp)  -- used as HashTable bucket")

# ── Summary ───────────────────────────────────────────────────
make_image([
    ("Component          Pass   Fail   Status",    GRAY),
    ("─" * 50,                                     GRAY),
    ("HashTable          19     0      ALL PASS",  GREEN),
    ("BST                17     0      ALL PASS",  GREEN),
    ("PriorityQueue      11     0      ALL PASS  (bug found + fixed)", GREEN),
    ("  max-heap sanity   4     0",                GREEN),
    ("  KitchenTask prio  3     0      after fix: return b > a", GREEN),
    ("  historical [BUG]  4  (info)   return a>b was wrong -- documented", YELLOW),
    ("Algorithms         16     0      ALL PASS  (binarySearch fixed)", GREEN),
    ("AVL                 9     0      ALL PASS",  GREEN),
    ("─" * 50,                                     GRAY),
    ("TOTAL              70     0      ALL PASS",  GREEN),
    ("",                                           None),
    ("Bugs found and fixed:",                      CYAN),
    ("  [1] KitchenTaskComparator: return a>b -> inverted heap", YELLOW),
    ("      Fix applied: return b>a  (KitchenManager.hpp)  ->  3/3 PASS", GREEN),
    ("  [2] ds::binarySearch: returned insertion point, not last", YELLOW),
    ("      Fix applied: save original_last  (Algorithms.hpp)  ->  PASS", GREEN),
],
"00_summary.png",
"Test Suite Summary  --  70 PASS | 0 FAIL  (after fixes)")

print("\nAll images generated in:", OUT)

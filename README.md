# Riff House: Brand-Affiliated Musical Instruments Inventory & Search Engine

**Course:** Data Structure and Algorithm Analysis | **Program:** BS in Computer Engineering
**Output:** Final Laboratory Capstone | **Language:** C++11, console based, **modular** (13 headers/sources pairs + `main.cpp`)

Companion documents: [`TEST_CASES.md`](TEST_CASES.md) (13 test cases with real output) and
[`COMPLEXITY.md`](COMPLEXITY.md) (best / average / worst time and space for every algorithm).

---

## 1. Project description

Riff House is the console system of a musical instruments shop with four official partner brands:
**Yamaha, Fender, Roland, Casio** (categories: Guitar, Keyboard, Accessories). Customers fill a
shopping cart and check out; orders wait in a queue, are served first come first served, update the
stock, and are recorded in a transaction history. The shop owner can add, delete and restock products.
The program starts with 14 sample products so everything can be demonstrated immediately.

## 2. Features (project requirements 1-12)

| # | Requirement | Menu | Module |
| --- | --- | --- | --- |
| 1 | Product management (add, delete, restock) | 4 | `CatalogMenu`, `Inventory` |
| 2 | Multiple official partner brands (4) | everywhere | `Brand` |
| 3 | Product display (inventory order, or by ID through the BST) | 1 | `CatalogMenu`, `Inventory`, `BST` |
| 4 | Brand-based filtering | 1 > 3 | `Search` (`filterByBrand`) |
| 5 | Product searching (keyword, ID: linear / binary / BST, and a comparison screen) | 2 | `Search`, `Inventory` |
| 6 | Product sorting (5 algorithms, 4 sort keys, ascending / descending) | 3 | `Sorting` |
| 7 | Shopping cart (add, remove, view, empty) | 5 | `Cart`, `Checkout` |
| 8 | Checkout processing (cart becomes a queued order) | 5 > 5 | `Checkout` |
| 9 | Order queue processing (first come, first served) | 6 | `OrderQueue`, `Checkout` |
| 10 | Inventory stock updates (sale, restock, refund) | 4, 6, 7 | `Inventory::adjustStock` |
| 11 | Transaction history (newest first) and refund of the last transaction | 7 | `TransactionStack` |
| 12 | Input validation and error handling | everywhere | `Input` and every module |

The required record is used exactly as specified (`Product.h`):

```cpp
struct Product {
    int id;               // Unique Identifier
    string name;          // Product Title
    string category;      // Category (Guitar, Keyboard, Accessories)
    string brand;         // Official Partner Brand (Yamaha, Fender, Roland, Casio)
    double price;         // Unit Price (PHP)
    int stockQuantity;    // Available Inventory Level
};
```

## 3. Folder structure

```
RiffHouse/
├── main.cpp                  start-up, main menu, coordination
├── Makefile                  optional build shortcut
├── README.md   TEST_CASES.md   COMPLEXITY.md
├── include/                  declarations only (every header has include guards)
│   ├── Product.h  Input.h  Brand.h  BST.h  Inventory.h  Search.h  Sorting.h
│   ├── Cart.h  OrderQueue.h  TransactionStack.h  Checkout.h
│   └── CatalogMenu.h  SalesMenu.h
└── src/                      one implementation file per header
    ├── Product.cpp  Input.cpp  Brand.cpp  BST.cpp  Inventory.cpp  Search.cpp  Sorting.cpp
    ├── Cart.cpp  OrderQueue.cpp  TransactionStack.cpp  Checkout.cpp
    └── CatalogMenu.cpp  SalesMenu.cpp
```

Changes from the suggested structure (and why): `Product.cpp` and `Input.cpp` were added because the
product display and the safe-input functions need a home that every module can share;
`Checkout.h/.cpp` is the "checkout processing" section of the prompt; `CatalogMenu` and `SalesMenu`
hold the sub-menu screens so that `main.cpp` stays limited to start-up, the main menu and coordination.

### Responsibility of each file

| File | Responsibility | Connects to |
| --- | --- | --- |
| `main.cpp` | Creates the objects, loads sample data, runs the main menu, frees memory on exit | all menu modules, `Inventory`, `Cart`, `OrderQueue`, `TransactionStack` |
| `Product` | The `Product` struct, table header/row printing | used by almost everything |
| `Input` | Safe line-based input (`readInt`, `readDouble`, `readYesNo`...), `toLower`, `formatMoney` | all menus, `Search`, `Sorting`, `Product` |
| `Brand` | The partner brand and category lists, brand / category pickers | `CatalogMenu`, `main` |
| `BST` | Binary search tree of `Product*` indexed by ID (insert, search, remove, in-order, height) | `Inventory` |
| `Inventory` | Linked list that **owns** the products + a BST index; add / remove / find / adjust stock | `BST`, everything that needs products |
| `Search` | Linear search (keyword, ID), brand filter, binary search on a sorted copy | `Inventory`, `Sorting` |
| `Sorting` | Bubble, selection, insertion, merge and quick sort on a pointer array | `Search`, `CatalogMenu` |
| `Cart` | The current customer's cart (array of product ID + quantity) | `Checkout`, `OrderQueue` (uses `CartItem`) |
| `OrderQueue` | FIFO queue of placed orders, units reserved by waiting orders | `Checkout`, `CatalogMenu` |
| `TransactionStack` | LIFO stack of completed sales with receipts | `Checkout`, `SalesMenu` |
| `Checkout` | The sale rules: cart check, place order, process order, refund | the four modules above |
| `CatalogMenu` | Screens: browse / filter, search, sort, manage products | `Inventory`, `Search`, `Sorting`, `Brand` |
| `SalesMenu` | Screens: cart, order queue, history | `Checkout` and the three sales modules |

### How the modules communicate

```
                           main.cpp
              ┌──────────────┴───────────────┐
         CatalogMenu                      SalesMenu
        ┌───┼────┬────────┐                   │
     Search Sorting Brand  │                Checkout
        │     │            │        ┌──────┬───┴────┬─────────────┐
        └──┬──┘            │     Cart  OrderQueue TransactionStack│
           │               │       │       │          │           │
        Inventory ◄────────┴───────┴───────┴──────────┴───────────┘
        │       │
       BST   (list)             Product, Input: shared by all
```

Rules that keep the modules independent: the data-structure modules (`BST`, `Cart`, `OrderQueue`,
`TransactionStack`) never include each other's logic; only `Checkout` joins them. The cart stores
product **IDs**, never pointers, so deleting a product can never leave a dangling pointer.

## 4. Laboratory integration table

| Laboratory concept | System feature | Reason for using it |
| --- | --- | --- |
| Structure (`struct`) | `Product`, `Order`, `Transaction`, `CartItem` | The project requires the `Product` record; the others group related data. |
| Arrays | Cart lines, brand / category names, temporary pointer array used for sorting and binary search | Small fixed collections and random access by index (needed by sorting and binary search). |
| Singly linked list | `Inventory` (the product catalog) | Products are added and deleted often; no shifting of data and no size limit. The list is the single owner of every product. |
| Binary Search Tree | ID index inside `Inventory`, in-order display by ID | Keeps IDs ordered all the time: find, add and delete by ID without sorting. In-order traversal gives a sorted list for free. |
| Queue (linked) | `OrderQueue` | Orders must be served first come, first served. |
| Stack (linked) | `TransactionStack` | The newest transaction is on top: history is shown newest first, and a refund can only reverse the latest sale. |
| Linear search | Keyword search, search by ID, brand filter | Works on the unsorted list and on conditions an index cannot answer (text, brand). |
| Binary search | Search by ID on a sorted copy | O(log n) on sorted data; the copy is temporary so the inventory order is preserved. |
| Bubble / Selection / Insertion sort | Sort menu (algorithms 1-3) | Elementary O(n²) sorts, appropriate for small catalogs. |
| Merge / Quick sort | Sort menu (algorithms 4-5), sorted copy for binary search | Efficient O(n log n) sorts for larger catalogs. |
| Dynamic memory, destructors | Every node type; `releaseAll()`, `clear()` | No leaks: every `new` has a matching `delete`. |

No lab concept was added that the project does not need (for example there is no hash table, no heap, no graph).

## 5. Memory management and safety

- **One owner.** The linked list owns every `Product`. The BST and the sorting arrays only hold pointers into it, so nothing is duplicated.
- **Safe deletion order.** `Inventory::removeProduct` removes the product from the BST first, then frees the list node.
- **IDs instead of pointers** in the cart, orders and transactions. A deleted product cannot cause a dangling pointer; the code simply reports "no longer exists".
- **Temporary arrays free themselves immediately:** each `new[]` in `toArray` / `mergeSort` is followed by `delete[]` in the same operation.
- **Destructors** free every list, tree, queue and stack node. `main` also calls `releaseAll()`, `clear()` on exit and prints how many nodes of each kind were freed.
- **Validation everywhere:** all input is read as a whole line; letters, blanks, negative numbers and forms such as `1e3`, `0x10`, `inf` are rejected. End of input (Ctrl+D / Ctrl+Z) exits cleanly through `InputClosed`, so memory is still freed.
- **Stock rules:** a customer can never buy more than the units still free: stock minus units promised to orders waiting in the queue minus what is already in the cart. A product with waiting orders cannot be deleted.

## 6. Requirements and compilation

- A C++ compiler with **C++11** support (g++ 4.8+, clang, MinGW, MSVC 2015+). In Dev-C++ / Code::Blocks, turn on the C++11 option.
- Compile from the `RiffHouse` folder with **one command** (works the same on Linux, macOS and Windows/MinGW):

```bash
g++ -std=c++11 -Wall -Wextra -o riffhouse main.cpp src/*.cpp
```

(On Windows `cmd`, if `src/*.cpp` is not expanded, list the files: `g++ -std=c++11 -o riffhouse.exe main.cpp src/Product.cpp src/Input.cpp src/Brand.cpp src/BST.cpp src/Inventory.cpp src/Search.cpp src/Sorting.cpp src/Cart.cpp src/OrderQueue.cpp src/TransactionStack.cpp src/Checkout.cpp src/CatalogMenu.cpp src/SalesMenu.cpp`)

- Or simply `make` (then `make clean`).
- Run: `./riffhouse` (`riffhouse.exe` on Windows).
- Memory check while developing: `g++ -std=c++11 -fsanitize=address,undefined -g -o riffhouse main.cpp src/*.cpp`

## 7. Main menu guide

```
1 Browse products   (display all / display by ID with the BST / filter by brand)
2 Search products   (keyword, ID: linear, binary, BST, compare all three)
3 Sort products     (Bubble, Selection, Insertion, Merge, Quick; by price, stock, name, ID)
4 Manage products   (add / delete / restock)
5 Shopping cart     (view, add, remove, empty, checkout)
6 Order queue       (view, process next)
7 Transaction history (view, refund last)
0 Exit              (frees all memory and prints a release report)
```

A 5-minute demo: option 1 (brand filter Fender) > 2 (compare the three searches on ID 2013) > 3 (merge sort by
price) > 5 (build a cart, checkout as "Justine") > 6 (view queue, process) > 7 (history, refund) > 0 (memory report).

## 8. Binary search and scalability notes

- Binary search keeps a `low` and a `high` index; the midpoint `mid = low + (high - low) / 2` is compared with the target and half of the remaining range is discarded each step (worked example in `COMPLEXITY.md`).
- Measured with 1000 products: linear search up to **1000** steps, binary search **10** steps.
- Binary search is **not always faster**: it needs sorted data, and sorting a copy costs O(n log n). For one search on a small catalog, linear search is cheaper. The program therefore uses the BST (always ordered) for everyday ID lookups and shows binary search on a temporary sorted copy.
- To add a brand or category: change the constant and the name list in `Brand.h/.cpp`. To add a sort key: add a value in `Sorting.h` and a line in `isBefore`. To add a product field: change `Product.h`, `printProduct` and the add screen only.

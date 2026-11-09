# Riff House - Complexity Analysis

`n` = number of products, `q` = orders in the queue, `t` = transactions, `k` = items per cart / order (at most 10, so it is a small constant).

## 1. Searching

| Algorithm | Best | Average | Worst | Space | Why |
| --- | --- | --- | --- | --- | --- |
| Linear search (keyword / ID / brand) | O(1) target is first | O(n) | O(n) | O(1) | Checks products one by one; needs no order. |
| Binary search on a sorted array | O(1) target is the first midpoint | O(log n) | O(log n) | O(1) | Each step discards half of the remaining range. |
| Binary search with a temporary sorted copy (what the menu does) | O(n log n) | O(n log n) | O(n log n) | O(n) | The merge sort of the copy dominates; the search itself is O(log n). |
| BST search by ID | O(1) target is the root | O(log n) | O(n) | O(1) | Halves the search while the tree is balanced; a tree that leans like a list is O(n). |

### How binary search works (`Search.cpp`)

`low` and `high` are the first and last positions that can still contain the target; `mid = low + (high - low) / 2` is the middle one.
If `sorted[mid]` is smaller than the target, everything at or below `mid` is ruled out (`low = mid + 1`); if it is larger,
everything at or above `mid` is ruled out (`high = mid - 1`). Searching ID **2005** in the 14 sorted IDs 2001 ... 2014:

| Step | low | high | mid | ID at mid | Decision |
| --- | --- | --- | --- | --- | --- |
| 1 | 0 | 13 | 6 | 2007 | 2005 < 2007, so `high = 5` |
| 2 | 0 | 5 | 2 | 2003 | 2005 > 2003, so `low = 3` |
| 3 | 3 | 5 | 4 | 2005 | found |

Binary search must only be used on sorted data. The program never searches the linked list directly with it:
it sorts a **temporary copy of pointers** (`binarySearchByIdTemporary`), so the inventory order and the BST are untouched.

### Measured steps (real program output)

| Products | Target | Linear | Binary | BST |
| --- | --- | --- | --- | --- |
| 14 | ID 2005 | 9 | 3 | 4 |
| 14 | ID 2013 | 14 | 3 | 4 |
| 1000 | ID 3500 | 514 | 10 | 503 (see note) |
| 1000 | missing ID | 1000 | 10 | 989 (see note) |

Note: the 1000-product run added 986 products with strictly ascending IDs, the worst case for a plain BST (height 989).
With mixed IDs the BST needs about log2(1000) = 10 steps like binary search.

### When binary search is NOT the better choice

Linear search is O(n) and binary search is O(log n), but binary search first needs sorted data. Sorting costs O(n log n),
which is more than one linear pass. So: for a **single** search, or a very small catalog, linear search is cheaper; binary
search pays off when the sorted data is **reused** for many searches. When products are added or removed often, a sorted
array must be re-sorted or shifted (O(n)), which is why the inventory keeps a BST index instead.

## 2. Sorting (`Sorting.cpp`)

| Algorithm | Best | Average | Worst | Space | Why |
| --- | --- | --- | --- | --- | --- |
| Bubble sort | O(n) already sorted (early exit) | O(n²) | O(n²) | O(1) | Repeated neighbour swaps. |
| Selection sort | O(n²) | O(n²) | O(n²) | O(1) | Always scans the rest to find the smallest. |
| Insertion sort | O(n) already sorted | O(n²) | O(n²) | O(1) | Shifts items inside a sorted prefix; fast on nearly sorted data. |
| Merge sort | O(n log n) | O(n log n) | O(n log n) | O(n) | Halves the array log n times and merges in O(n); needs one helper array. |
| Quick sort | O(n log n) | O(n log n) | O(n²) | O(log n) | Partitioning around a pivot. The middle item is the pivot, so sorted input is not a worst case; some inputs still can be. |

All five sort an array of `Product*` pointers (copying pointers, not products). The ID tie-breaker makes every sort give the same result for equal values.

## 3. Data structure operations

| Operation | Best | Average | Worst | Space | Notes |
| --- | --- | --- | --- | --- | --- |
| Inventory add product | O(1) | O(log n) | O(n) | O(1) | Duplicate check by BST, append at the list tail in O(1), BST insert. |
| Inventory remove product | O(1) | O(n) | O(n) | O(1) | Finding the node in a singly linked list is O(n); BST removal is O(log n) average. |
| Find product by ID (`findById`) | O(1) | O(log n) | O(n) | O(1) | BST search. |
| Display by ID (BST in-order) | O(n) | O(n) | O(n) | O(h) | `h` = tree height (recursion depth). |
| `toArray` (temporary pointer array) | O(n) | O(n) | O(n) | O(n) | Freed right after use. |
| Queue enqueue / dequeue | O(1) | O(1) | O(1) | O(1) | Linked queue with front and rear pointers. |
| Queue `reservedQuantity` | O(q·k) | O(q·k) | O(q·k) | O(1) | Scans waiting orders. |
| Stack push / pop | O(1) | O(1) | O(1) | O(1) | Only the top changes. |
| Cart add / remove / find | O(k) | O(k) | O(k) | O(1) | k <= 10, effectively constant. |
| Add to cart (`addToCart`) | O(log n + q·k) | same | O(n + q·k) | O(1) | BST lookup + reserved check. |
| Process next order | O(k log n) | same | O(k·n) | O(k) | k BST lookups twice (check, then update). |
| Transaction history display | O(t·k) | O(t·k) | O(t·k) | O(1) | Walks the stack. |

## 4. Scalability notes

- Everyday operations (find, add, queue, stack, cart) are O(1) or O(log n) while the BST is balanced; only sorting, listing and temporary copies are O(n) or more.
- No repeated work inside loops: availability (`getAvailableStock`) is computed once per line when needed.
- No unnecessary allocation: products are never copied into the tree, cart or sort arrays; temporary arrays are freed immediately.
- Known limit: the plain BST can become unbalanced when IDs are added in ascending order (see the measurements above). A self-balancing tree was left out on purpose because it goes beyond the required lab concepts.

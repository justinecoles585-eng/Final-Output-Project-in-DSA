// CatalogMenu.cpp - product browsing, searching, sorting and management screens.

#include "../include/CatalogMenu.h"
#include "../include/OrderQueue.h"
#include "../include/Search.h"
#include "../include/Sorting.h"
#include "../include/Brand.h"
#include "../include/Input.h"
#include <iostream>

// ---------------------------------------------------------------- Browse / filter
static void showAllProducts(const Inventory& inventory) {
    if (inventory.isEmpty()) {
        cout << "No products in the inventory.\n";
        return;
    }
    printProductHeader();
    for (ProductNode* current = inventory.getHead(); current != nullptr; current = current->next) {
        printProduct(current->data);
    }
    cout << "Total products: " << inventory.getCount() << "\n";
}

static void showProductsByBrand(const Inventory& inventory) {
    string brand = getBrandName(chooseBrand());
    int found = filterByBrand(inventory, brand);
    if (found == 0) cout << "No " << brand << " products found.\n";
    else cout << found << " product(s) from " << brand << ".\n";
}

void browseMenu(const Inventory& inventory) {
    cout << "\n--- Browse Products ---\n"
            "  1. Display all products (inventory order)\n"
            "  2. Display products by ID (BST in-order)\n"
            "  3. Filter by partner brand\n"
            "  0. Back\n";
    int choice = readInt("Choose: ", 0, 3);
    if (choice == 1) {
        showAllProducts(inventory);
    } else if (choice == 2) {
        if (inventory.isEmpty()) cout << "No products in the inventory.\n";
        else {
            cout << "BST in-order traversal, tree height = " << inventory.getTreeHeight() << "\n";
            inventory.displayInIdOrder();
        }
    } else if (choice == 3) {
        showProductsByBrand(inventory);
    }
}

// ---------------------------------------------------------------- Search
static void showSearchResult(const string& method, Product* product, int steps) {
    cout << "[" << method << "] " << steps << " step(s)\n";
    if (product == nullptr) {
        cout << "  Product not found.\n";
        return;
    }
    printProductHeader();
    printProduct(*product);
}

static void compareSearchMethods(const Inventory& inventory, int id) {
    int steps = 0;
    Product* product = linearSearchById(inventory, id, steps);
    showSearchResult("Linear search O(n)    ", product, steps);
    product = binarySearchByIdTemporary(inventory, id, steps);
    showSearchResult("Binary search O(log n)", product, steps);
    product = inventory.findById(id, steps);
    showSearchResult("BST search            ", product, steps);
    cout << "Note: binary search needs sorted data, so it first sorts a temporary copy (O(n log n)).\n"
            "It only pays off when the sorted copy is reused for many searches; the BST keeps the\n"
            "data ordered all the time, so each lookup is O(log n) on a balanced tree.\n";
}

void searchMenu(const Inventory& inventory) {
    if (inventory.isEmpty()) {
        cout << "No products to search.\n";
        return;
    }
    cout << "\n--- Search Products ---\n"
            "  1. Keyword (name / category / brand)   [linear search]\n"
            "  2. Product ID                          [linear search]\n"
            "  3. Product ID                          [binary search on a sorted copy]\n"
            "  4. Product ID                          [BST search]\n"
            "  5. Compare all three on one ID\n"
            "  0. Back\n";
    int choice = readInt("Choose: ", 0, 5);
    if (choice == 0) return;

    if (choice == 1) {
        string keyword = readNonEmpty("Keyword: ");
        int found = searchByKeyword(inventory, keyword);
        if (found == 0) cout << "No products match \"" << keyword << "\".\n";
        else cout << found << " match(es).\n";
        return;
    }

    int id = readInt("Product ID: ", 1, 99999);
    if (choice == 5) {
        compareSearchMethods(inventory, id);
        return;
    }
    // Run the search first: 'steps' must be filled in BEFORE it is passed to the display function.
    int steps = 0;
    Product* product = nullptr;
    string method;
    if (choice == 2) {
        method = "Linear search";
        product = linearSearchById(inventory, id, steps);
    } else if (choice == 3) {
        method = "Binary search";
        product = binarySearchByIdTemporary(inventory, id, steps);
    } else {
        method = "BST search";
        product = inventory.findById(id, steps);
    }
    showSearchResult(method, product, steps);
}

// ---------------------------------------------------------------- Sort
void sortMenu(const Inventory& inventory) {
    if (inventory.isEmpty()) {
        cout << "No products to sort.\n";
        return;
    }
    const string algorithmNames[5] = {"Bubble", "Selection", "Insertion", "Merge", "Quick"};
    const string keyNames[4] = {"price", "stock", "name", "ID"};     // same order as the menu below

    cout << "\n--- Sort Products ---\nAlgorithm:\n";
    for (int i = 0; i < 5; i++) cout << "  " << (i + 1) << ". " << algorithmNames[i] << " sort\n";
    cout << "  0. Back\n";
    int algorithm = readInt("Choose: ", 0, 5);
    if (algorithm == 0) return;

    cout << "Sort by:\n  1. Price\n  2. Stock quantity\n  3. Name\n  4. ID\n";
    int keyChoice = readInt("Choose: ", 1, 4);
    SortKey key = SORT_BY_ID;
    if (keyChoice == 1) key = SORT_BY_PRICE;
    else if (keyChoice == 2) key = SORT_BY_STOCK;
    else if (keyChoice == 3) key = SORT_BY_NAME;

    cout << "Order:\n  1. Ascending\n  2. Descending\n";
    bool descending = (readInt("Choose: ", 1, 2) == 2);

    int n = 0;
    Product** items = inventory.toArray(n);      // temporary copy of pointers: the inventory order is untouched
    if (algorithm == 1) bubbleSort(items, n, key);
    else if (algorithm == 2) selectionSort(items, n, key);
    else if (algorithm == 3) insertionSort(items, n, key);
    else if (algorithm == 4) mergeSort(items, n, key);
    else quickSort(items, n, key);

    cout << algorithmNames[algorithm - 1] << " sort by " << keyNames[keyChoice - 1]
         << (descending ? " (descending):\n" : " (ascending):\n");
    printProductHeader();
    if (descending) {
        for (int i = n - 1; i >= 0; i--) printProduct(*items[i]);
    } else {
        for (int i = 0; i < n; i++) printProduct(*items[i]);
    }
    delete[] items;
}

// ---------------------------------------------------------------- Manage products
static void addNewProduct(Inventory& inventory) {
    int id = 0;
    while (true) {
        id = readInt("New product ID (1-99999, 0 to cancel): ", 0, 99999);
        if (id == 0) {
            cout << "Cancelled.\n";
            return;
        }
        if (inventory.findById(id) == nullptr) break;
        cout << "  That ID already exists. Choose another.\n";
    }
    Product product;
    product.id = id;
    product.name = readNonEmpty("Product name: ");
    product.category = getCategoryName(chooseCategory());
    product.brand = getBrandName(chooseBrand());
    product.price = readDouble("Unit price in PHP (0.01 - 1000000): ", 0.01, 1000000);
    product.stockQuantity = readInt("Stock quantity (0 - 100000): ", 0, 100000);

    if (inventory.addProduct(product)) cout << "Added product " << id << ".\n";
    else cout << "Could not add the product.\n";
}

static void deleteProduct(Inventory& inventory, const OrderQueue& orders) {
    int id = readInt("Product ID to delete (0 to cancel): ", 0, 99999);
    if (id == 0) return;
    Product* product = inventory.findById(id);
    if (product == nullptr) {
        cout << "Product ID " << id << " not found.\n";
        return;
    }
    int reserved = orders.reservedQuantity(id);
    if (reserved > 0) {
        cout << "Cannot delete: " << reserved << " unit(s) are waiting in the order queue.\n";
        return;
    }
    printProductHeader();
    printProduct(*product);
    if (!readYesNo("Delete this product? (y/n): ")) {
        cout << "Cancelled.\n";
        return;
    }
    inventory.removeProduct(id);
    cout << "Product deleted.\n";
}

static void restockProduct(Inventory& inventory) {
    int id = readInt("Product ID to restock (0 to cancel): ", 0, 99999);
    if (id == 0) return;
    Product* product = inventory.findById(id);
    if (product == nullptr) {
        cout << "Product ID " << id << " not found.\n";
        return;
    }
    int amount = readInt("Units to add (1 - 100000): ", 1, 100000);
    if (inventory.adjustStock(id, amount)) {
        cout << product->name << " now has " << product->stockQuantity << " in stock.\n";
    } else {
        cout << "Stock limit (" << MAX_STOCK << ") would be exceeded.\n";
    }
}

void manageProductsMenu(Inventory& inventory, const OrderQueue& orders) {
    cout << "\n--- Manage Products ---\n"
            "  1. Add product\n"
            "  2. Delete product\n"
            "  3. Restock product\n"
            "  0. Back\n";
    int choice = readInt("Choose: ", 0, 3);
    if (choice == 1) addNewProduct(inventory);
    else if (choice == 2) deleteProduct(inventory, orders);
    else if (choice == 3) restockProduct(inventory);
}

// Search.cpp - linear search, binary search and brand filtering.

#include "../include/Search.h"
#include "../include/Sorting.h"
#include "../include/Input.h"
#include <iostream>

int searchByKeyword(const Inventory& inventory, const string& keyword) {
    string wanted = toLower(keyword);
    int found = 0;
    for (ProductNode* current = inventory.getHead(); current != nullptr; current = current->next) {
        const Product& product = current->data;
        string text = toLower(product.name + " " + product.category + " " + product.brand);
        if (text.find(wanted) != string::npos) {
            if (found == 0) printProductHeader();
            printProduct(product);
            found++;
        }
    }
    return found;
}

int filterByBrand(const Inventory& inventory, const string& brand) {
    int found = 0;
    for (ProductNode* current = inventory.getHead(); current != nullptr; current = current->next) {
        if (current->data.brand == brand) {
            if (found == 0) printProductHeader();
            printProduct(current->data);
            found++;
        }
    }
    return found;
}

Product* linearSearchById(const Inventory& inventory, int id, int& comparisons) {
    comparisons = 0;
    for (ProductNode* current = inventory.getHead(); current != nullptr; current = current->next) {
        comparisons++;
        if (current->data.id == id) return &current->data;
    }
    return nullptr;
}

Product* binarySearchById(Product* sorted[], int n, int id, int& comparisons) {
    comparisons = 0;
    int low = 0;                                // first index still possible
    int high = n - 1;                           // last index still possible
    while (low <= high) {
        int mid = low + (high - low) / 2;       // midpoint (written this way to avoid overflow)
        comparisons++;
        if (sorted[mid]->id == id) return sorted[mid];
        if (sorted[mid]->id < id) low = mid + 1;    // target is in the upper half
        else high = mid - 1;                        // target is in the lower half
    }
    return nullptr;
}

Product* binarySearchByIdTemporary(const Inventory& inventory, int id, int& comparisons) {
    int n = 0;
    Product** copy = inventory.toArray(n);      // pointers only, the list is not changed
    mergeSort(copy, n, SORT_BY_ID);             // binary search needs sorted data
    Product* result = binarySearchById(copy, n, id, comparisons);
    delete[] copy;                              // result still points into the inventory list
    return result;
}

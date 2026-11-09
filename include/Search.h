// Search.h - searching and brand filtering.
//   Linear search : keyword, brand, ID      -> works on the unsorted linked list, O(n)
//   Binary search : ID                      -> works ONLY on data sorted by ID,   O(log n)
//   (BST search by ID lives in Inventory::findById.)
// Used by: CatalogMenu.

#ifndef SEARCH_H
#define SEARCH_H

#include <string>
#include "Inventory.h"
using namespace std;

// Linear searches: print matching rows (header included) and return how many matched.
int searchByKeyword(const Inventory& inventory, const string& keyword);   // name / category / brand text
int filterByBrand(const Inventory& inventory, const string& brand);       // exact partner brand

// Linear search by ID. 'comparisons' reports how many products were checked.
Product* linearSearchById(const Inventory& inventory, int id, int& comparisons);

// Binary search on an array that is ALREADY sorted by ID.
// low / high are the bounds of the part still to search, mid is its midpoint.
Product* binarySearchById(Product* sorted[], int n, int id, int& comparisons);

// Builds a temporary sorted copy (merge sort), runs binary search on it, then discards
// the copy. The linked list keeps its original order.
Product* binarySearchByIdTemporary(const Inventory& inventory, int id, int& comparisons);

#endif

// Sorting.h - five sorting algorithms that work on an ARRAY OF PRODUCT POINTERS.
// Sorting a temporary pointer array (never the linked list itself) keeps the original
// inventory order and the BST index untouched, and copies no product data.
// Used by: Search (sorted copy for binary search), CatalogMenu (sort menu).

#ifndef SORTING_H
#define SORTING_H

#include "Product.h"

enum SortKey { SORT_BY_ID, SORT_BY_PRICE, SORT_BY_STOCK, SORT_BY_NAME };

// Elementary sorts: O(n^2)
void bubbleSort(Product* items[], int n, SortKey key);
void selectionSort(Product* items[], int n, SortKey key);
void insertionSort(Product* items[], int n, SortKey key);

// Efficient sorts: O(n log n)
void mergeSort(Product* items[], int n, SortKey key);
void quickSort(Product* items[], int n, SortKey key);

#endif

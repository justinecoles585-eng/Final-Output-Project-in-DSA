// Sorting.cpp - bubble, selection, insertion, merge and quick sort.

#include "../include/Sorting.h"
#include "../include/Input.h"

// True when product a must come before product b. The ID is the tie-breaker,
// so equal prices / stock / names always end up in the same order.
static bool isBefore(const Product* a, const Product* b, SortKey key) {
    if (key == SORT_BY_PRICE && a->price != b->price) return a->price < b->price;
    if (key == SORT_BY_STOCK && a->stockQuantity != b->stockQuantity) return a->stockQuantity < b->stockQuantity;
    if (key == SORT_BY_NAME) {
        string nameA = toLower(a->name);
        string nameB = toLower(b->name);
        if (nameA != nameB) return nameA < nameB;
    }
    return a->id < b->id;
}

static void swapItems(Product*& a, Product*& b) {
    Product* temp = a;
    a = b;
    b = temp;
}

// ---- Bubble sort: repeatedly swap neighbours that are out of order.
void bubbleSort(Product* items[], int n, SortKey key) {
    for (int pass = 0; pass < n - 1; pass++) {
        bool swapped = false;
        for (int i = 0; i < n - pass - 1; i++) {
            if (isBefore(items[i + 1], items[i], key)) {
                swapItems(items[i], items[i + 1]);
                swapped = true;
            }
        }
        if (!swapped) break;                    // already sorted: stop early
    }
}

// ---- Selection sort: pick the smallest remaining item and put it in place.
void selectionSort(Product* items[], int n, SortKey key) {
    for (int i = 0; i < n - 1; i++) {
        int smallest = i;
        for (int j = i + 1; j < n; j++) {
            if (isBefore(items[j], items[smallest], key)) smallest = j;
        }
        if (smallest != i) swapItems(items[i], items[smallest]);
    }
}

// ---- Insertion sort: insert each item into the already-sorted part on its left.
void insertionSort(Product* items[], int n, SortKey key) {
    for (int i = 1; i < n; i++) {
        Product* current = items[i];
        int j = i - 1;
        while (j >= 0 && isBefore(current, items[j], key)) {
            items[j + 1] = items[j];
            j--;
        }
        items[j + 1] = current;
    }
}

// ---- Merge sort: split in halves, sort each half, merge them back together.
static void mergeHalves(Product* items[], Product* temp[], int low, int mid, int high, SortKey key) {
    int left = low, right = mid + 1, k = low;
    while (left <= mid && right <= high) {
        if (isBefore(items[right], items[left], key)) temp[k++] = items[right++];
        else temp[k++] = items[left++];
    }
    while (left <= mid) temp[k++] = items[left++];
    while (right <= high) temp[k++] = items[right++];
    for (k = low; k <= high; k++) items[k] = temp[k];
}

static void mergeSortRange(Product* items[], Product* temp[], int low, int high, SortKey key) {
    if (low >= high) return;
    int mid = low + (high - low) / 2;
    mergeSortRange(items, temp, low, mid, key);
    mergeSortRange(items, temp, mid + 1, high, key);
    mergeHalves(items, temp, low, mid, high, key);
}

void mergeSort(Product* items[], int n, SortKey key) {
    if (n < 2) return;
    Product** temp = new Product*[n];           // one helper array, allocated once
    mergeSortRange(items, temp, 0, n - 1, key);
    delete[] temp;
}

// ---- Quick sort: partition around a pivot, then sort both sides.
// The middle item is used as pivot so already-sorted data does not cause the O(n^2) worst case.
static void quickSortRange(Product* items[], int low, int high, SortKey key) {
    if (low >= high) return;
    int mid = low + (high - low) / 2;
    swapItems(items[mid], items[high]);         // move the pivot to the end
    Product* pivot = items[high];
    int boundary = low;                         // items before 'boundary' are smaller than the pivot
    for (int i = low; i < high; i++) {
        if (isBefore(items[i], pivot, key)) {
            swapItems(items[i], items[boundary]);
            boundary++;
        }
    }
    swapItems(items[boundary], items[high]);    // pivot goes to its final position
    quickSortRange(items, low, boundary - 1, key);
    quickSortRange(items, boundary + 1, high, key);
}

void quickSort(Product* items[], int n, SortKey key) {
    quickSortRange(items, 0, n - 1, key);
}

// Inventory.cpp - linked list of products + BST index.

#include "../include/Inventory.h"

Inventory::Inventory() : head(nullptr), tail(nullptr), count(0) {}

Inventory::~Inventory() {
    int treeNodes = 0;
    releaseAll(treeNodes);
}

Product* Inventory::findById(int id, int& steps) const {
    return index.search(id, steps);
}

Product* Inventory::findById(int id) const {
    int steps = 0;
    return index.search(id, steps);
}

bool Inventory::addProduct(const Product& product) {
    if (findById(product.id) != nullptr) return false;     // IDs must be unique

    ProductNode* node = new ProductNode(product);
    if (tail == nullptr) head = node;                      // empty list
    else tail->next = node;
    tail = node;
    count++;
    index.insert(&node->data);                             // the node never moves, so the pointer stays valid
    return true;
}

bool Inventory::removeProduct(int id) {
    // Step 1: remove from the index FIRST, so no tree node can point at freed memory.
    if (!index.remove(id)) return false;

    // Step 2: unlink and free the list node.
    ProductNode* previous = nullptr;
    ProductNode* current = head;
    while (current != nullptr && current->data.id != id) {
        previous = current;
        current = current->next;
    }
    if (current == nullptr) return false;                  // cannot happen while list and index agree
    if (previous == nullptr) head = current->next;
    else previous->next = current->next;
    if (current == tail) tail = previous;
    delete current;
    count--;
    return true;
}

bool Inventory::adjustStock(int id, int change) {
    Product* product = findById(id);
    if (product == nullptr) return false;
    long newStock = (long)product->stockQuantity + change;
    if (newStock < 0 || newStock > MAX_STOCK) return false;
    product->stockQuantity = (int)newStock;
    return true;
}

ProductNode* Inventory::getHead() const { return head; }

int Inventory::getCount() const { return count; }

bool Inventory::isEmpty() const { return count == 0; }

void Inventory::displayInIdOrder() const { index.displayInOrder(); }

int Inventory::getTreeHeight() const { return index.height(); }

Product** Inventory::toArray(int& size) const {
    size = count;
    Product** items = new Product*[count > 0 ? count : 1];
    int i = 0;
    for (ProductNode* current = head; current != nullptr; current = current->next) {
        items[i] = &current->data;
        i++;
    }
    return items;
}

int Inventory::releaseAll(int& treeNodesFreed) {
    treeNodesFreed = index.clear();                        // tree first: it only points into the list
    int freed = 0;
    while (head != nullptr) {
        ProductNode* next = head->next;
        delete head;
        head = next;
        freed++;
    }
    tail = nullptr;
    count = 0;
    return freed;
}

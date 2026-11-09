// Inventory.h - the product catalog.
// A singly linked list OWNS every Product (insert/remove without shifting items);
// a BST index (BST.h) lets the system find a product by ID quickly.
// Used by: Search, Cart, Checkout, CatalogMenu, SalesMenu, main.

#ifndef INVENTORY_H
#define INVENTORY_H

#include "Product.h"
#include "BST.h"

const int MAX_STOCK = 1;    // upper limit for one product's stock

struct ProductNode {
    Product data;
    ProductNode* next;
    ProductNode(const Product& product) : data(product), next(nullptr) {}
};

class Inventory {
private:
    ProductNode* head;
    ProductNode* tail;
    int count;
    ProductBST index;             // ID index, points into the list

public:
    Inventory();
    ~Inventory();
    Inventory(const Inventory&) = delete;
    Inventory& operator=(const Inventory&) = delete;

    bool addProduct(const Product& product);        // false if the ID already exists
    bool removeProduct(int id);                     // false if the ID does not exist
    Product* findById(int id) const;                // BST lookup
    Product* findById(int id, int& steps) const;    // same, also reports BST nodes visited
    bool adjustStock(int id, int change);           // + restock/refund, - sale; refuses invalid results

    ProductNode* getHead() const;                   // start of the list (for traversal)
    int getCount() const;
    bool isEmpty() const;
    void displayInIdOrder() const;                  // BST in-order traversal
    int getTreeHeight() const;

    Product** toArray(int& size) const;             // temporary array of pointers; caller must delete[]
    int releaseAll(int& treeNodesFreed);            // frees tree then list, returns list nodes freed
};

#endif

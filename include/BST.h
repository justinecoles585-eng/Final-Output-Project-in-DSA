// BST.h - Binary Search Tree that indexes products by ID.
// The tree stores POINTERS to products owned by the Inventory linked list, so product
// data is never duplicated. Deleting a tree node never deletes the product itself.
// Used by: Inventory (owns one tree as its ID index).

#ifndef BST_H
#define BST_H

#include "Product.h"

struct TreeNode {
    Product* item;       // points to a Product stored in the Inventory list
    TreeNode* left;      // IDs smaller than item->id
    TreeNode* right;     // IDs larger than item->id
    TreeNode(Product* product) : item(product), left(nullptr), right(nullptr) {}
};

class ProductBST {
private:
    TreeNode* root;
    int count;

    TreeNode* insertNode(TreeNode* node, Product* product, bool& inserted);
    TreeNode* removeNode(TreeNode* node, int id, bool& removed);
    void displayNode(const TreeNode* node) const;
    int heightOf(const TreeNode* node) const;
    int destroyNode(TreeNode* node);

public:
    ProductBST();
    ~ProductBST();
    ProductBST(const ProductBST&) = delete;              // copying would double-free the nodes
    ProductBST& operator=(const ProductBST&) = delete;

    bool insert(Product* product);                       // false if the ID already exists
    Product* search(int id, int& steps) const;           // steps = nodes visited
    bool remove(int id);                                 // false if the ID does not exist
    void displayInOrder() const;                         // prints products sorted by ID
    int height() const;
    int size() const;
    int clear();                                         // frees all tree nodes, returns how many
};

#endif

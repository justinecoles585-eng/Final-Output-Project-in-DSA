// BST.cpp - Binary Search Tree implementation (index by product ID).

#include "../include/BST.h"
#include <iostream>
using namespace std;

ProductBST::ProductBST() : root(nullptr), count(0) {}

ProductBST::~ProductBST() { clear(); }

// ---- Insert: go left for smaller IDs, right for larger IDs, attach at the first empty spot.
TreeNode* ProductBST::insertNode(TreeNode* node, Product* product, bool& inserted) {
    if (node == nullptr) {
        inserted = true;
        return new TreeNode(product);
    }
    if (product->id < node->item->id) node->left = insertNode(node->left, product, inserted);
    else if (product->id > node->item->id) node->right = insertNode(node->right, product, inserted);
    // equal ID: duplicate, nothing is inserted
    return node;
}

bool ProductBST::insert(Product* product) {
    if (product == nullptr) return false;
    bool inserted = false;
    root = insertNode(root, product, inserted);
    if (inserted) count++;
    return inserted;
}

// ---- Search: each comparison discards half of the remaining tree (when the tree is balanced).
Product* ProductBST::search(int id, int& steps) const {
    steps = 0;
    TreeNode* current = root;
    while (current != nullptr) {
        steps++;
        if (id == current->item->id) return current->item;
        if (id < current->item->id) current = current->left;
        else current = current->right;
    }
    return nullptr;
}

// ---- Remove: three cases (no child, one child, two children).
TreeNode* ProductBST::removeNode(TreeNode* node, int id, bool& removed) {
    if (node == nullptr) return nullptr;
    if (id < node->item->id) {
        node->left = removeNode(node->left, id, removed);
    } else if (id > node->item->id) {
        node->right = removeNode(node->right, id, removed);
    } else {
        removed = true;
        if (node->left == nullptr) {            // 0 or 1 child (right)
            TreeNode* child = node->right;
            delete node;                        // deletes only the tree node, not the Product
            return child;
        }
        if (node->right == nullptr) {           // 1 child (left)
            TreeNode* child = node->left;
            delete node;
            return child;
        }
        TreeNode* successor = node->right;      // 2 children: use the smallest ID of the right side
        while (successor->left != nullptr) successor = successor->left;
        node->item = successor->item;
        bool ignored = false;
        node->right = removeNode(node->right, successor->item->id, ignored);
    }
    return node;
}

bool ProductBST::remove(int id) {
    bool removed = false;
    root = removeNode(root, id, removed);
    if (removed) count--;
    return removed;
}

// ---- In-order traversal (left, node, right) visits IDs in ascending order.
void ProductBST::displayNode(const TreeNode* node) const {
    if (node == nullptr) return;
    displayNode(node->left);
    printProduct(*node->item);
    displayNode(node->right);
}

void ProductBST::displayInOrder() const {
    printProductHeader();
    displayNode(root);
}

int ProductBST::heightOf(const TreeNode* node) const {
    if (node == nullptr) return 0;
    int leftHeight = heightOf(node->left);
    int rightHeight = heightOf(node->right);
    return 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);
}

int ProductBST::height() const { return heightOf(root); }

int ProductBST::size() const { return count; }

// ---- Free every node (post-order: children first, then the node).
int ProductBST::destroyNode(TreeNode* node) {
    if (node == nullptr) return 0;
    int freed = destroyNode(node->left) + destroyNode(node->right);
    delete node;
    return freed + 1;
}

int ProductBST::clear() {
    int freed = destroyNode(root);
    root = nullptr;
    count = 0;
    return freed;
}

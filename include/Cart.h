// Cart.h - the shopping cart of the current customer.
// A small fixed-size array is enough: a cart holds only a few lines (productId + quantity),
// and no product data is copied, so prices and names are always read from the inventory.
// Used by: Checkout, SalesMenu, OrderQueue (CartItem and MAX_CART_ITEMS).

#ifndef CART_H
#define CART_H

class Inventory;                      // only the name is needed here (used for display and totals)

const int MAX_CART_ITEMS = 10;        // different products per cart / order

struct CartItem {
    int productId;
    int quantity;
    CartItem() : productId(0), quantity(0) {}
};

class Cart {
private:
    CartItem items[MAX_CART_ITEMS];
    int itemCount;

public:
    Cart();

    bool isEmpty() const;
    int getItemCount() const;
    const CartItem& getItem(int index) const;
    int findItem(int productId) const;          // linear search in the cart, -1 if absent
    int quantityOf(int productId) const;        // 0 if absent

    bool addItem(int productId, int quantity);  // adds to an existing line, false if the cart is full
    bool removeItem(int productId);             // false if the product is not in the cart
    void clear();

    double calculateTotal(const Inventory& inventory) const;
    void display(const Inventory& inventory) const;
};

#endif

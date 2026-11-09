// SalesMenu.cpp - cart, order queue and transaction history screens.

#include "../include/SalesMenu.h"
#include "../include/Checkout.h"
#include "../include/Input.h"
#include <iostream>

// ---------------------------------------------------------------- Shopping cart
static void addItemScreen(Cart& cart, const Inventory& inventory, const OrderQueue& orders) {
    if (inventory.isEmpty()) {
        cout << "No products available.\n";
        return;
    }
    int id = readInt("Product ID to add (0 to cancel): ", 0, 99999);
    if (id == 0) return;
    Product* product = inventory.findById(id);
    if (product == nullptr) {
        cout << "Product ID " << id << " not found.\n";
        return;
    }
    int available = getAvailableStock(inventory, orders, id) - cart.quantityOf(id);
    if (available < 1) {
        cout << product->name << " is not available (sold out, reserved by waiting orders, or all already in your cart).\n";
        return;
    }
    printProductHeader();
    printProduct(*product);
    int quantity = readInt("Quantity (1 - " + to_string(available) + "): ", 1, available);
    addToCart(cart, inventory, orders, id, quantity);
}

static void removeItemScreen(Cart& cart) {
    if (cart.isEmpty()) {
        cout << "The cart is empty.\n";
        return;
    }
    int id = readInt("Product ID to remove (0 to cancel): ", 0, 99999);
    if (id == 0) return;
    if (cart.removeItem(id)) cout << "Removed product " << id << " from the cart.\n";
    else cout << "Product " << id << " is not in the cart.\n";
}

static void checkoutScreen(Cart& cart, const Inventory& inventory, OrderQueue& orders) {
    if (cart.isEmpty()) {
        cout << "The cart is empty. Add products before checkout.\n";
        return;
    }
    cart.display(inventory);
    if (!readYesNo("Place this order? (y/n): ")) {
        cout << "Checkout cancelled. Your cart is kept.\n";
        return;
    }
    string customer = readNonEmpty("Customer name: ");
    placeOrder(cart, customer, inventory, orders);
}

void cartMenu(Cart& cart, Inventory& inventory, OrderQueue& orders) {
    cout << "\n--- Shopping Cart ---\n"
            "  1. View cart\n"
            "  2. Add product to cart\n"
            "  3. Remove product from cart\n"
            "  4. Empty the cart\n"
            "  5. Checkout (place order in the queue)\n"
            "  0. Back\n";
    int choice = readInt("Choose: ", 0, 5);
    if (choice == 1) {
        cart.display(inventory);
    } else if (choice == 2) {
        addItemScreen(cart, inventory, orders);
    } else if (choice == 3) {
        removeItemScreen(cart);
    } else if (choice == 4) {
        cart.clear();
        cout << "The cart is now empty.\n";
    } else if (choice == 5) {
        checkoutScreen(cart, inventory, orders);
    }
}

// ---------------------------------------------------------------- Order queue
void orderMenu(Inventory& inventory, OrderQueue& orders, TransactionStack& history) {
    cout << "\n--- Order Queue ---\n"
            "  1. View waiting orders\n"
            "  2. Process next order\n"
            "  0. Back\n";
    int choice = readInt("Choose: ", 0, 2);
    if (choice == 1) orders.display();
    else if (choice == 2) processNextOrder(inventory, orders, history);
}

// ---------------------------------------------------------------- Transaction history
void historyMenu(Inventory& inventory, TransactionStack& history) {
    cout << "\n--- Transaction History ---\n"
            "  1. View transactions (newest first)\n"
            "  2. Refund the last transaction\n"
            "  0. Back\n";
    int choice = readInt("Choose: ", 0, 2);
    if (choice == 1) history.displayAll();
    else if (choice == 2) refundLastTransaction(inventory, history);
}

// ---------------------------------------------------------------- Quick order
void quickOrderMenu(Inventory& inventory, OrderQueue& orders, TransactionStack& history) {
    if (inventory.isEmpty()) {
        cout << "No products available.\n";
        return;
    }
    cout << "--- Quick Order ---\n";
    printProductHeader();
    for (ProductNode* node = inventory.getHead(); node != nullptr; node = node->next)
        printProduct(node->data);

    int id = readInt("\nProduct ID to order (0 to cancel): ", 0, 99999);
    if (id == 0) return;
    Product* product = inventory.findById(id);
    if (product == nullptr) {
        cout << "Product ID " << id << " not found.\n";
        return;
    }
    int available = getAvailableStock(inventory, orders, id);
    if (available < 1) {
        cout << product->name << " is not available (sold out or reserved by waiting orders).\n";
        return;
    }
    int quantity = readInt("Quantity (1 - " + to_string(available) + "): ", 1, available);
    string customer = readNonEmpty("Customer name: ");

    // A temporary cart is used, so the customer's real cart is not touched.
    Cart quickCart;
    if (!addToCart(quickCart, inventory, orders, id, quantity)) return;
    if (!placeOrder(quickCart, customer, inventory, orders)) return;

    int position = orders.size();            // the new order is at the rear of the queue
    cout << "\nThe order is now in the queue:\n";
    orders.display();

    if (readYesNo("\nServe the queue now so the sale is recorded in the history? (y/n): ")) {
        // First come, first served: older orders go first, then this one.
        for (int i = 0; i < position; i++) {
            if (orders.isEmpty()) break;
            processNextOrder(inventory, orders, history);
        }
        cout << "\nCheck: Main menu 6 = order queue, Main menu 7 = transaction history.\n";
    } else {
        cout << "Order waiting in the queue. Serve it in Main menu 6 > 2 (then it appears in history).\n";
    }
}
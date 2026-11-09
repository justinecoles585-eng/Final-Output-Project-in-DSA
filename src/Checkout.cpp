// Checkout.cpp - cart, order and transaction rules.

#include "../include/Checkout.h"
#include "../include/Input.h"
#include <iostream>

int getAvailableStock(const Inventory& inventory, const OrderQueue& orders, int productId) {
    Product* product = inventory.findById(productId);
    if (product == nullptr) return 0;
    int available = product->stockQuantity - orders.reservedQuantity(productId);
    return available < 0 ? 0 : available;
}

bool addToCart(Cart& cart, const Inventory& inventory, const OrderQueue& orders, int productId, int quantity) {
    Product* product = inventory.findById(productId);
    if (product == nullptr) {
        cout << "Product ID " << productId << " not found.\n";
        return false;
    }
    int canStillAdd = getAvailableStock(inventory, orders, productId) - cart.quantityOf(productId);
    if (canStillAdd < 0) canStillAdd = 0;
    if (quantity < 1 || quantity > canStillAdd) {
        cout << "Cannot add " << quantity << " x " << product->name << ": only " << canStillAdd
             << " more can be bought (stock minus orders already waiting and items already in your cart).\n";
        return false;
    }
    if (!cart.addItem(productId, quantity)) {
        cout << "The cart is full (maximum " << MAX_CART_ITEMS << " different products).\n";
        return false;
    }
    cout << "Added " << quantity << " x " << product->name << " to the cart.\n";
    return true;
}

bool placeOrder(Cart& cart, const string& customer, const Inventory& inventory, OrderQueue& orders) {
    if (cart.isEmpty()) {
        cout << "The cart is empty. Add products before checkout.\n";
        return false;
    }
    // Check every line first, so an order is only queued when all of it can be fulfilled.
    for (int i = 0; i < cart.getItemCount(); i++) {
        const CartItem& item = cart.getItem(i);
        Product* product = inventory.findById(item.productId);
        if (product == nullptr) {
            cout << "Checkout stopped: product ID " << item.productId << " no longer exists. Remove it from the cart.\n";
            return false;
        }
        if (item.quantity > getAvailableStock(inventory, orders, item.productId)) {
            cout << "Checkout stopped: not enough stock for " << product->name
                 << ". Reduce the quantity or remove it from the cart.\n";
            return false;
        }
    }
    Order order;
    order.customer = customer;
    order.itemCount = cart.getItemCount();
    for (int i = 0; i < order.itemCount; i++) order.items[i] = cart.getItem(i);

    int orderId = orders.enqueue(order);
    cart.clear();
    cout << "Order #" << orderId << " placed for " << customer << ". Position in queue: " << orders.size() << ".\n";
    return true;
}

bool processNextOrder(Inventory& inventory, OrderQueue& orders, TransactionStack& history) {
    Order order;
    if (!orders.dequeue(order)) {
        cout << "The order queue is empty.\n";
        return false;
    }
    // Pass 1: verify every line (a product may have been removed since the order was placed).
    for (int i = 0; i < order.itemCount; i++) {
        Product* product = inventory.findById(order.items[i].productId);
        if (product == nullptr || product->stockQuantity < order.items[i].quantity) {
            cout << "Order #" << order.orderId << " for " << order.customer
                 << " was cancelled: product ID " << order.items[i].productId << " is no longer available.\n";
            return false;
        }
    }
    // Pass 2: reduce stock and build the transaction record.
    Transaction transaction;
    transaction.orderId = order.orderId;
    transaction.customer = order.customer;
    transaction.lineCount = order.itemCount;
    for (int i = 0; i < order.itemCount; i++) {
        Product* product = inventory.findById(order.items[i].productId);
        inventory.adjustStock(product->id, -order.items[i].quantity);
        transaction.lines[i].productId = product->id;
        transaction.lines[i].name = product->name;
        transaction.lines[i].unitPrice = product->price;
        transaction.lines[i].quantity = order.items[i].quantity;
        transaction.total += product->price * order.items[i].quantity;
    }
    transaction.transactionId = history.push(transaction);   // push stores its own copy
    cout << "Order #" << order.orderId << " served.\n";
    printTransaction(transaction);
    return true;
}

bool refundLastTransaction(Inventory& inventory, TransactionStack& history) {
    Transaction transaction;
    if (!history.pop(transaction)) {
        cout << "No transaction to refund.\n";
        return false;
    }
    cout << "Refunding transaction #" << transaction.transactionId << " for " << transaction.customer << ":\n";
    for (int i = 0; i < transaction.lineCount; i++) {
        const TransactionLine& line = transaction.lines[i];
        if (inventory.adjustStock(line.productId, line.quantity)) {
            cout << "  Returned " << line.quantity << " x " << line.name << " to stock.\n";
        } else {
            cout << "  Could not return " << line.quantity << " x " << line.name
                 << " to stock (product deleted or stock limit reached).\n";
        }
    }
    cout << "Refund total: PHP " << formatMoney(transaction.total) << "\n";
    return true;
}

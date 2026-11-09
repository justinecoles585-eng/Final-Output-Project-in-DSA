// Cart.cpp - shopping cart operations.

#include "../include/Cart.h"
#include "../include/Inventory.h"
#include "../include/Input.h"
#include <iostream>
#include <iomanip>

Cart::Cart() : itemCount(0) {}

bool Cart::isEmpty() const { return itemCount == 0; }

int Cart::getItemCount() const { return itemCount; }

const CartItem& Cart::getItem(int index) const { return items[index]; }

int Cart::findItem(int productId) const {
    for (int i = 0; i < itemCount; i++) {
        if (items[i].productId == productId) return i;
    }
    return -1;
}

int Cart::quantityOf(int productId) const {
    int position = findItem(productId);
    return position < 0 ? 0 : items[position].quantity;
}

bool Cart::addItem(int productId, int quantity) {
    int position = findItem(productId);
    if (position >= 0) {                        // product already in the cart: just add the quantity
        items[position].quantity += quantity;
        return true;
    }
    if (itemCount >= MAX_CART_ITEMS) return false;
    items[itemCount].productId = productId;
    items[itemCount].quantity = quantity;
    itemCount++;
    return true;
}

bool Cart::removeItem(int productId) {
    int position = findItem(productId);
    if (position < 0) return false;
    for (int i = position; i < itemCount - 1; i++) items[i] = items[i + 1];   // close the gap
    itemCount--;
    return true;
}

void Cart::clear() { itemCount = 0; }

double Cart::calculateTotal(const Inventory& inventory) const {
    double total = 0;
    for (int i = 0; i < itemCount; i++) {
        Product* product = inventory.findById(items[i].productId);
        if (product != nullptr) total += product->price * items[i].quantity;
    }
    return total;
}

void Cart::display(const Inventory& inventory) const {
    if (isEmpty()) {
        cout << "The cart is empty.\n";
        return;
    }
    cout << left << setw(6) << "ID" << setw(34) << "Name" << right << setw(5) << "Qty"
         << setw(13) << "Unit(PHP)" << setw(14) << "Subtotal" << "\n" << string(72, '-') << "\n";
    for (int i = 0; i < itemCount; i++) {
        Product* product = inventory.findById(items[i].productId);
        if (product == nullptr) {               // the product was deleted after it was added
            cout << left << setw(6) << items[i].productId << setw(34) << "(product removed)"
                 << right << setw(5) << items[i].quantity << "\n";
            continue;
        }
        cout << left << setw(6) << product->id << setw(34) << shortName(product->name)
             << right << setw(5) << items[i].quantity << setw(13) << formatMoney(product->price)
             << setw(14) << formatMoney(product->price * items[i].quantity) << "\n";
    }
    cout << string(72, '-') << "\n" << right << setw(58) << "TOTAL (PHP): "
         << setw(14) << formatMoney(calculateTotal(inventory)) << "\n";
}

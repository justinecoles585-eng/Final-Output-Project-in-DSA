// Product.cpp - display helpers for the Product record.

#include "../include/Product.h"
#include "../include/Input.h"
#include <iostream>
#include <iomanip>

string shortName(const string& name) {
    if (name.size() > 33) return name.substr(0, 31) + "..";
    return name;
}

void printProductHeader() {
    cout << left << setw(6) << "ID" << setw(34) << "Name" << setw(13) << "Category"
         << setw(8) << "Brand" << right << setw(12) << "Price(PHP)" << setw(7) << "Stock" << "\n"
         << string(80, '-') << "\n";
}

void printProduct(const Product& product) {
    cout << left << setw(6) << product.id << setw(34) << shortName(product.name)
         << setw(13) << product.category << setw(8) << product.brand
         << right << setw(12) << formatMoney(product.price) << setw(7) << product.stockQuantity;
    if (product.stockQuantity == 0) cout << "  SOLD OUT";
    else if (product.stockQuantity <= 5) cout << "  LOW";
    cout << "\n";
}

// Product.h - the Product record required by the project, plus display helpers.
// Used by: every module that shows or stores products.

#ifndef PRODUCT_H
#define PRODUCT_H

#include <string>
using namespace std;

struct Product {
    int id;               // Unique Identifier
    string name;          // Product Title
    string category;      // Category (Guitar, Keyboard, Accessories)
    string brand;         // Official Partner Brand (Yamaha, Fender, Roland, Casio)
    double price;         // Unit Price (PHP)
    int stockQuantity;    // Available Inventory Level
};

// Shortens a long product name so table columns stay aligned.
string shortName(const string& name);

// Prints the table header and one product row (flags LOW / SOLD OUT stock).
void printProductHeader();
void printProduct(const Product& product);

#endif

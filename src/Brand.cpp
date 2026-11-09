// Brand.cpp - partner brand and category lists (sequential storage: plain arrays).

#include "../include/Brand.h"
#include "../include/Input.h"
#include <iostream>

static const string BRAND_NAMES[NUM_BRANDS] = {"Yamaha", "Fender", "Roland", "Casio"};
static const string CATEGORY_NAMES[NUM_CATEGORIES] = {"Guitar", "Keyboard", "Accessories"};

string getBrandName(int index) {
    if (index < 0 || index >= NUM_BRANDS) return "";
    return BRAND_NAMES[index];
}

string getCategoryName(int index) {
    if (index < 0 || index >= NUM_CATEGORIES) return "";
    return CATEGORY_NAMES[index];
}

bool isPartnerBrand(const string& brand) {
    for (int i = 0; i < NUM_BRANDS; i++) {
        if (BRAND_NAMES[i] == brand) return true;
    }
    return false;
}

int chooseBrand() {
    cout << "Partner brands:\n";
    for (int i = 0; i < NUM_BRANDS; i++) cout << "  " << (i + 1) << ". " << BRAND_NAMES[i] << "\n";
    return readInt("Choose a brand: ", 1, NUM_BRANDS) - 1;
}

int chooseCategory() {
    cout << "Categories:\n";
    for (int i = 0; i < NUM_CATEGORIES; i++) cout << "  " << (i + 1) << ". " << CATEGORY_NAMES[i] << "\n";
    return readInt("Choose a category: ", 1, NUM_CATEGORIES) - 1;
}

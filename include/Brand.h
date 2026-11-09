// Brand.h - the official partner brands and the product categories of the store.
// Used by: CatalogMenu (add product, filter), main (sample data).
// To add a brand or category: change NUM_BRANDS / NUM_CATEGORIES here and the name list in Brand.cpp.

#ifndef BRAND_H
#define BRAND_H

#include <string>
using namespace std;

const int NUM_BRANDS = 4;
const int NUM_CATEGORIES = 3;

string getBrandName(int index);        // index 0 .. NUM_BRANDS-1
string getCategoryName(int index);     // index 0 .. NUM_CATEGORIES-1
bool isPartnerBrand(const string& brand);

// Show a numbered list and return the chosen index (0-based).
int chooseBrand();
int chooseCategory();

#endif

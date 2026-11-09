// CatalogMenu.h - console screens for the product catalog:
// browse / filter, search, sort, and manage (add, delete, restock).
// Used by: main. Calls: Inventory, Search, Sorting, Brand, Input.

#ifndef CATALOG_MENU_H
#define CATALOG_MENU_H

#include "Inventory.h"

class OrderQueue;      // only needed as a reference type

void browseMenu(const Inventory& inventory);
void searchMenu(const Inventory& inventory);
void sortMenu(const Inventory& inventory);
void manageProductsMenu(Inventory& inventory, const OrderQueue& orders);

#endif

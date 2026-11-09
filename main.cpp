#include <iostream>
#include "include/Product.h"
#include "include/Inventory.h"
#include "include/Cart.h"
#include "include/OrderQueue.h"
#include "include/TransactionStack.h"
#include "include/CatalogMenu.h"
#include "include/SalesMenu.h"
#include "include/Input.h"
#include "include/Brand.h"
using namespace std;

// Start-up data: 14 products from the 4 partner brands.
// The order is mixed on purpose so the BST index starts reasonably balanced.
static void loadSampleProducts(Inventory& inventory) {
    const Product samples[] = {
        {2008, "Roland FP-10 Digital Piano",     "Keyboard",    "Roland", 32000.00, 3},
        {2003, "Yamaha PAC112V Electric Guitar", "Guitar",      "Yamaha", 17500.00, 5},
        {2012, "Casio CDP-S160 Digital Piano",   "Keyboard",    "Casio",  21000.00, 4},
        {2001, "Yamaha F310 Acoustic Guitar",    "Guitar",      "Yamaha", 8500.00,  10},
        {2006, "Fender Frontman 10G Amplifier",  "Accessories", "Fender", 3900.00,  12},
        {2010, "Roland Micro Cube GX Amp",       "Accessories", "Roland", 9500.00,  9},
        {2014, "Casio LK-S250 Lighted Keyboard", "Keyboard",    "Casio",  9500.00,  2},
        {2002, "Yamaha PSR-E373 Keyboard",       "Keyboard",    "Yamaha", 11500.00, 7},
        {2005, "Fender CD-60S Acoustic Guitar",  "Guitar",      "Fender", 13000.00, 0},
        {2009, "Roland GO:KEYS 3 Keyboard",      "Keyboard",    "Roland", 15900.00, 6},
        {2011, "Casio CT-S300 Keyboard",         "Keyboard",    "Casio",  6500.00,  20},
        {2004, "Fender Squier Stratocaster",     "Guitar",      "Fender", 14500.00, 8},
        {2007, "Fender 351 Guitar Picks (12pk)", "Accessories", "Fender", 2500.00,   80},
        {2015, "Yamaha THR10II Wireless Amp",     "Accessories", "Yamaha", 12500.00, 15},
        {2016, "Roland GO:PIANO 88 Keyboard",     "Keyboard",    "Roland", 19900.00, 5},
        {2013, "Casio SA-76 Mini Keyboard",      "Keyboard",    "Casio",  2800.00,  30}};
    int total = sizeof(samples) / sizeof(samples[0]);
    for (int i = 0; i < total; i++) inventory.addProduct(samples[i]);
}

static void printMainMenu() {
    cout << "\n=========== RIFF HOUSE : Musical Instruments Shop ===========\n"
            "  1. Browse products ( display / filter by brand)\n"
            "  2. Search products\n"
            "  3. Sort products\n"
            "  4. Manage products (add / delete / restock)\n"
            "  5. Shopping cart and checkout\n"
            "  6. Order queue\n"
            "  7. Transaction history\n"
            "  8. QUICK ORDER (order a product in one step)\n"
            "  0. Exit\n"
            "=============================================================\n";
}

int main() {
    Inventory inventory;              // linked list + BST index
    OrderQueue orders;                // queue of placed orders
    TransactionStack history;         // stack of completed transactions
    Cart cart;                        // current customer's cart (array)

    loadSampleProducts(inventory);
    cout << "Partner brands:";
    for (int i = 0; i < NUM_BRANDS; i++) cout << " " << getBrandName(i);
    cout << "\n";

    try {
        bool running = true;
        while (running) {
            printMainMenu();
            int choice = readInt("Enter choice (0-8): ", 0, 8);
            cout << "\n";
            if (choice == 1) browseMenu(inventory);
            else if (choice == 2) searchMenu(inventory);
            else if (choice == 3) sortMenu(inventory);
            else if (choice == 4) manageProductsMenu(inventory, orders);
            else if (choice == 5) cartMenu(cart, inventory, orders);
            else if (choice == 6) orderMenu(inventory, orders, history);
            else if (choice == 7) historyMenu(inventory, history);
            else if (choice == 8) quickOrderMenu(inventory, orders, history);
            else running = false;
        }
    } catch (const InputClosed&) {
        cout << "\nInput closed. Exiting safely.\n";
    }

    // Release all dynamic memory (the BST first, because it only points into the list).
    int treeNodes = 0;
    int listNodes = inventory.releaseAll(treeNodes);
    int queueNodes = orders.clear();
    int stackNodes = history.clear();
    cout << "Memory released -> list nodes: " << listNodes << ", BST nodes: " << treeNodes
         << ", queue nodes: " << queueNodes << ", stack nodes: " << stackNodes << "\n";
    cout << "Thank you for using Riff House. Goodbye!\n";
    return 0;
}

// Checkout.h - the business rules that connect Inventory, Cart, OrderQueue and TransactionStack.
// The data-structure modules know nothing about each other; this module is the only
// place where a sale flows:  Cart -> placeOrder -> OrderQueue -> processNextOrder
//                            -> stock update (Inventory) + TransactionStack.
// Used by: SalesMenu.

#ifndef CHECKOUT_H
#define CHECKOUT_H

#include <string>
#include "Inventory.h"
#include "Cart.h"
#include "OrderQueue.h"
#include "TransactionStack.h"
using namespace std;

// Units that can still be sold: stock minus the units promised to orders waiting in the queue.
int getAvailableStock(const Inventory& inventory, const OrderQueue& orders, int productId);

// Cart step: refuses unknown IDs and quantities above what is still available
// (counting what is already in this cart). Prints the reason when it refuses.
bool addToCart(Cart& cart, const Inventory& inventory, const OrderQueue& orders, int productId, int quantity);

// Checkout step: re-checks every cart line, turns the cart into an order and queues it.
bool placeOrder(Cart& cart, const string& customer, const Inventory& inventory, OrderQueue& orders);

// Serves the order at the front of the queue: reduces stock and records a transaction.
// A line that cannot be fulfilled cancels the WHOLE order and changes nothing.
bool processNextOrder(Inventory& inventory, OrderQueue& orders, TransactionStack& history);

// Reverses the latest transaction (stack pop) and puts the stock back.
bool refundLastTransaction(Inventory& inventory, TransactionStack& history);

#endif

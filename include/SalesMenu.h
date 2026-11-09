// SalesMenu.h - console screens for selling: shopping cart and checkout,
// order queue processing, and transaction history.
// Used by: main. Calls: Checkout, Cart, OrderQueue, TransactionStack, Input.

#ifndef SALES_MENU_H
#define SALES_MENU_H

#include "Inventory.h"
#include "Cart.h"
#include "OrderQueue.h"
#include "TransactionStack.h"

void cartMenu(Cart& cart, Inventory& inventory, OrderQueue& orders);
void orderMenu(Inventory& inventory, OrderQueue& orders, TransactionStack& history);
void historyMenu(Inventory& inventory, TransactionStack& history);
void quickOrderMenu(Inventory& inventory, OrderQueue& orders, TransactionStack& history);

#endif

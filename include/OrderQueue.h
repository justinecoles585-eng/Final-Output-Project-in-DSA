// OrderQueue.h - checkout line of customer orders (Queue, first come first served).
// Orders are served in the order they were placed, so a queue (FIFO) is the natural fit.
// Used by: Checkout (place / process orders), SalesMenu, CatalogMenu (delete check).

#ifndef ORDER_QUEUE_H
#define ORDER_QUEUE_H

#include <string>
#include "Cart.h"
using namespace std;

struct Order {
    int orderId;
    string customer;
    CartItem items[MAX_CART_ITEMS];
    int itemCount;
    Order() : orderId(0), customer(""), itemCount(0) {}
};

struct OrderNode {
    Order order;
    OrderNode* next;
    OrderNode(const Order& o) : order(o), next(nullptr) {}
};

class OrderQueue {
private:
    OrderNode* front;      // next order to be served
    OrderNode* rear;       // last order placed
    int count;
    int nextOrderId;

public:
    OrderQueue();
    ~OrderQueue();
    OrderQueue(const OrderQueue&) = delete;
    OrderQueue& operator=(const OrderQueue&) = delete;

    int enqueue(Order order);                    // adds at the rear, returns the new order number
    bool dequeue(Order& served);                 // removes from the front, false if empty
    bool isEmpty() const;
    int size() const;
    int reservedQuantity(int productId) const;   // units of a product promised to waiting orders
    void display() const;
    int clear();                                 // frees all nodes, returns how many
};

#endif

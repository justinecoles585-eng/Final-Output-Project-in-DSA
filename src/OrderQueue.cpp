// OrderQueue.cpp - linked-list queue of orders.

#include "../include/OrderQueue.h"
#include <iostream>

OrderQueue::OrderQueue() : front(nullptr), rear(nullptr), count(0), nextOrderId(1) {}

OrderQueue::~OrderQueue() { clear(); }

int OrderQueue::enqueue(Order order) {
    order.orderId = nextOrderId++;
    OrderNode* node = new OrderNode(order);
    if (rear == nullptr) front = node;           // empty queue
    else rear->next = node;
    rear = node;
    count++;
    return order.orderId;
}

bool OrderQueue::dequeue(Order& served) {
    if (front == nullptr) return false;
    OrderNode* node = front;
    served = node->order;
    front = node->next;
    if (front == nullptr) rear = nullptr;        // the queue became empty
    delete node;
    count--;
    return true;
}

bool OrderQueue::isEmpty() const { return front == nullptr; }

int OrderQueue::size() const { return count; }

int OrderQueue::reservedQuantity(int productId) const {
    int total = 0;
    for (OrderNode* current = front; current != nullptr; current = current->next) {
        for (int i = 0; i < current->order.itemCount; i++) {
            if (current->order.items[i].productId == productId) total += current->order.items[i].quantity;
        }
    }
    return total;
}

void OrderQueue::display() const {
    if (isEmpty()) {
        cout << "The order queue is empty.\n";
        return;
    }
    cout << "Order queue (" << count << " waiting), next to be served first:\n";
    int position = 1;
    for (OrderNode* current = front; current != nullptr; current = current->next, position++) {
        const Order& order = current->order;
        cout << "  #" << position << "  Order " << order.orderId << " - " << order.customer << ":";
        for (int i = 0; i < order.itemCount; i++) {
            cout << " [ID " << order.items[i].productId << " x" << order.items[i].quantity << "]";
        }
        cout << "\n";
    }
}

int OrderQueue::clear() {
    int freed = 0;
    while (front != nullptr) {
        OrderNode* node = front;
        front = front->next;
        delete node;
        freed++;
    }
    rear = nullptr;
    count = 0;
    return freed;
}

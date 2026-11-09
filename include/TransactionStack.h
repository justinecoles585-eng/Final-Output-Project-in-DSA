// TransactionStack.h - completed sales (Stack, last in first out).
// The most recent transaction is always on top: history is shown newest first, and the
// "refund last transaction" feature can only ever reverse the latest sale (pop).
// Each line stores the product name and price AT THE TIME OF SALE, so history stays
// correct even if the product is later edited or deleted.
// Used by: Checkout (record / refund), SalesMenu (view history).

#ifndef TRANSACTION_STACK_H
#define TRANSACTION_STACK_H

#include <string>
#include "Cart.h"
using namespace std;

struct TransactionLine {
    int productId;
    string name;
    double unitPrice;
    int quantity;
    TransactionLine() : productId(0), name(""), unitPrice(0), quantity(0) {}
};

struct Transaction {
    int transactionId;
    int orderId;
    string customer;
    TransactionLine lines[MAX_CART_ITEMS];
    int lineCount;
    double total;
    Transaction() : transactionId(0), orderId(0), customer(""), lineCount(0), total(0) {}
};

struct TransactionNode {
    Transaction transaction;
    TransactionNode* next;
    TransactionNode(const Transaction& t) : transaction(t), next(nullptr) {}
};

void printTransaction(const Transaction& transaction);   // prints one receipt

class TransactionStack {
private:
    TransactionNode* top;
    int count;
    int nextTransactionId;

public:
    TransactionStack();
    ~TransactionStack();
    TransactionStack(const TransactionStack&) = delete;
    TransactionStack& operator=(const TransactionStack&) = delete;

    int push(Transaction transaction);           // returns the new transaction number
    bool pop(Transaction& latest);               // false if empty
    bool isEmpty() const;
    int size() const;
    void displayAll() const;                     // newest first
    int clear();                                 // frees all nodes, returns how many
};

#endif

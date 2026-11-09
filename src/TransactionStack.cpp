// TransactionStack.cpp - linked-list stack of completed transactions.

#include "../include/TransactionStack.h"
#include "../include/Input.h"
#include <iostream>

void printTransaction(const Transaction& transaction) {
    cout << "  Transaction #" << transaction.transactionId << " (Order #" << transaction.orderId
         << ") - " << transaction.customer << "\n";
    for (int i = 0; i < transaction.lineCount; i++) {
        const TransactionLine& line = transaction.lines[i];
        cout << "      " << line.quantity << " x " << line.name << " @ PHP " << formatMoney(line.unitPrice)
             << " = PHP " << formatMoney(line.unitPrice * line.quantity) << "\n";
    }
    cout << "      TOTAL: PHP " << formatMoney(transaction.total) << "\n";
}

TransactionStack::TransactionStack() : top(nullptr), count(0), nextTransactionId(1) {}

TransactionStack::~TransactionStack() { clear(); }

int TransactionStack::push(Transaction transaction) {
    transaction.transactionId = nextTransactionId++;
    TransactionNode* node = new TransactionNode(transaction);
    node->next = top;                            // the new node becomes the top
    top = node;
    count++;
    return transaction.transactionId;
}

bool TransactionStack::pop(Transaction& latest) {
    if (top == nullptr) return false;
    TransactionNode* node = top;
    latest = node->transaction;
    top = node->next;
    delete node;
    count--;
    return true;
}

bool TransactionStack::isEmpty() const { return top == nullptr; }

int TransactionStack::size() const { return count; }

void TransactionStack::displayAll() const {
    if (isEmpty()) {
        cout << "No transactions yet.\n";
        return;
    }
    cout << "Transaction history (" << count << "), newest first:\n";
    for (TransactionNode* current = top; current != nullptr; current = current->next) {
        printTransaction(current->transaction);
    }
}

int TransactionStack::clear() {
    int freed = 0;
    while (top != nullptr) {
        TransactionNode* node = top;
        top = top->next;
        delete node;
        freed++;
    }
    count = 0;
    return freed;
}

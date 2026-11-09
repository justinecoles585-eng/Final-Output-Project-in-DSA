// Input.h - safe console input and small text helpers.
// Every read takes a whole line and validates it, so wrong input never crashes
// the program or makes it loop forever.
// Used by: all menu modules, Brand, Search, Sorting, Product, Cart, TransactionStack.

#ifndef INPUT_H
#define INPUT_H

#include <string>
using namespace std;

// Thrown when the input stream ends (Ctrl+D / Ctrl+Z / end of a piped file).
// main.cpp catches it so the program can still free all of its memory.
struct InputClosed {};

string readLine(const string& prompt);
string readNonEmpty(const string& prompt);                    // text that is not blank
int    readInt(const string& prompt, int low, int high);      // whole number in [low, high]
double readDouble(const string& prompt, double low, double high);
bool   readYesNo(const string& prompt);                       // y / n

string trim(const string& text);        // removes spaces at both ends
string toLower(string text);            // lowercase copy (for case-insensitive search)
string formatMoney(double amount);      // 1500 -> "1500.00"

#endif

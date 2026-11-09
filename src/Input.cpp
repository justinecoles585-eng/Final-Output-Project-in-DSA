// Input.cpp - safe console input and text helpers.

#include "../include/Input.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cstdlib>
using namespace std;

string readLine(const string& prompt) {
    cout << prompt;
    string line;
    if (!getline(cin, line)) throw InputClosed();
    return line;
}

string trim(const string& text) {
    size_t first = text.find_first_not_of(" \t\r\n");
    if (first == string::npos) return "";
    size_t last = text.find_last_not_of(" \t\r\n");
    return text.substr(first, last - first + 1);
}

string toLower(string text) {
    for (size_t i = 0; i < text.size(); i++) {
        if (text[i] >= 'A' && text[i] <= 'Z') text[i] = (char)(text[i] + 32);
    }
    return text;
}

string formatMoney(double amount) {
    ostringstream out;
    out << fixed << setprecision(2) << amount;
    return out.str();
}

// True only for text made of digits (with an optional sign and, if allowed, ONE decimal point).
// This rejects things strtod would accept, such as "1e5", "0x10", "inf" or "nan".
static bool looksLikeNumber(const string& text, bool allowDecimal) {
    size_t i = 0;
    if (!text.empty() && (text[0] == '-' || text[0] == '+')) i = 1;
    if (i >= text.size()) return false;
    int dots = 0;
    for (; i < text.size(); i++) {
        if (text[i] == '.' && allowDecimal) {
            dots++;
            if (dots > 1) return false;
        } else if (text[i] < '0' || text[i] > '9') {
            return false;
        }
    }
    return true;
}

int readInt(const string& prompt, int low, int high) {
    while (true) {
        string text = trim(readLine(prompt));
        if (looksLikeNumber(text, false)) {
            long value = strtol(text.c_str(), nullptr, 10);   // huge numbers become LONG_MAX and fail the range check
            if (value >= low && value <= high) return (int)value;
        }
        cout << "  Invalid input. Enter a whole number from " << low << " to " << high << ".\n";
    }
}

double readDouble(const string& prompt, double low, double high) {
    while (true) {
        string text = trim(readLine(prompt));
        if (looksLikeNumber(text, true)) {
            char* end = nullptr;
            double value = strtod(text.c_str(), &end);
            if (*end == '\0' && value >= low && value <= high) return value;
        }
        cout << "  Invalid input. Enter a number from " << formatMoney(low) << " to " << formatMoney(high) << ".\n";
    }
}

string readNonEmpty(const string& prompt) {
    while (true) {
        string text = trim(readLine(prompt));
        if (!text.empty()) return text;
        cout << "  Input cannot be empty.\n";
    }
}

bool readYesNo(const string& prompt) {
    while (true) {
        string text = toLower(trim(readLine(prompt)));
        if (text == "y" || text == "yes") return true;
        if (text == "n" || text == "no") return false;
        cout << "  Please type y or n.\n";
    }
}

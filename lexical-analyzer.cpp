#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
using namespace std;

// Note: This is a Python analyzer made in C++

vector<string> keywords = {"if", "else", "elif", "while", "for", "return", "def", "class"};

bool is_keyword(const string& token) {
    for (const string& keyword : keywords) {
        if (token == keyword) {
            return true;
        }
    }
    return false;
}

// Define variables and functions for the lexical analyzer here.
bool is_valid_identifier(const string& token) {
    // Check if the token is a valid identifier (e.g., starts with a letter or underscore, followed by letters, digits, or underscores)
    if (token.empty()) return false;

    if (!isalpha(token[0]) && token[0] != '_') return false;

    for (int i = 1; i < token.size(); ++i) {
        if (!isalnum(token[i]) && token[i] != '_') return false;
    }

    if (is_keyword(token)) return false; // Identifiers cannot be keywords

    return true;
}

bool is_valid_number(const string& token) {
    // Check if the token is a valid number (e.g., integer or float)
    bool has_decimal_point = false;
    for (char c : token) {
        if (c == '.') {
            if (has_decimal_point) return false; // More than one decimal point
            has_decimal_point = true;
        } else if (!isdigit(c)) {
            return false; // Not a digit
        }
    }
    return !token.empty();
}

bool is_valid_string(const string& token) {
    // Check if the token is a valid string (e.g., enclosed in double quotes)
    return token.size() >= 2 && token.front() == '"' && token.back() == '"';
}

void lexical_analyzer(const string& code) {
    cout << "-- Lexical Analyzer --" << endl;


}
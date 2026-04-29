#include "compiler.h"
#include <cctype>

// Define the global vectors here. Because of "extern" in compiler.h, other files can now use these.
vector<string> keywords = {"def", "return", "if", "else", "elif", "while", "for", "class", "True", "False", "None"};
vector<string> punctuations = {"(", ")", "{", "}", "[", "]", ",", ":", ";", "."};
vector<string> operators = {"+", "-", "*", "/", "=", "==", "!=", "<", ">", "<=", ">=", "**", "//", "%", "and", "or", "not"};
vector<string> print_statements = {"print"};

bool is_keyword(const string& token) {
    for (const string& keyword : keywords) {
        if (token == keyword) return true;
    }
    return false;
}

bool is_punctuation(const string& token) {
    for (const string& punctuation : punctuations) {
        if (token == punctuation) return true;
    }
    return false;
}

bool is_operator(const string& token) {
    for (const string& op : operators) {
        if (token == op) return true;
    }
    return false;
}

bool is_number(const string& token) {
    bool has_decimal_point = false;
    for (char c : token) {
        if (c == '.') {
            if (has_decimal_point) return false; 
            has_decimal_point = true;
        } else if (!isdigit(c)) {
            return false; 
        }
    }
    return !token.empty();
}

bool is_valid_identifier(const string& token) {
    if (token.empty()) return false;
    if (!isalpha(token[0]) && token[0] != '_') return false;

    // Fixed size_t vs int warning here
    for (size_t i = 1; i < token.size(); ++i) {
        if (!isalnum(token[i]) && token[i] != '_') return false;
    }

    if (is_keyword(token)) return false; 
    return true;
}

bool is_print_statement(const string& token) {
    for (const string& print_stmt : print_statements) {
        if (token == print_stmt) return true;
    }
    return false;
}

vector<Token> lexical_analyzer(string code) {
    vector<Token> tokens;
    string type;
    string value;
    
    for (size_t i = 0; i < code.size(); ++i) {
        char c = code[i];

        if (isspace(c)) continue; 
        if (c == '#') break; // Python comments ignore the rest of the line

        if (isalpha(c) || c == '_') {
            value += c;
            while (i + 1 < code.size() && (isalnum(code[i + 1]) || code[i + 1] == '_')) {
                value += code[++i];
            }
            // Check if it's a logical operator (and, or, not)
            if (value == "and" || value == "or" || value == "not") {
                type = "logical_" + value;
            } else if (is_keyword(value)) {
                type = "keyword";
            } else {
                type = "identifier";
            }
            tokens.push_back(Token(type, value));
            value.clear();
            
        } else if (isdigit(c) || c == '.') {
            value += c;
            while (i + 1 < code.size() && (isdigit(code[i + 1]) || code[i + 1] == '.')) {
                value += code[++i];
            }
            type = is_number(value) ? "number" : "invalid_number";
            tokens.push_back(Token(type, value));
            value.clear();
            
        } else if (c == '"') {
            value += c;
            while (i + 1 < code.size() && code[i + 1] != '"') {
                value += code[++i];
            }
            if (i + 1 < code.size()) {
                value += '"'; 
                ++i; 
                type = "string";
            } else {
                type = "unterminated_string";
            }
            tokens.push_back(Token(type, value));
            value.clear();
            
        } else if (is_punctuation(string(1, c))) {
            tokens.push_back(Token("punctuation", string(1, c)));
            
        } else if (is_operator(string(1, c))) {
            string op(1, c);
            while (i + 1 < code.size() && is_operator(op + code[i + 1])) {
                op += code[++i];
            }
            
            if (op == "+") type = "addition";
            else if (op == "-") type = "subtraction";
            else if (op == "*") type = "multiplication";
            else if (op == "/") type = "division";
            else if (op == "%") type = "modulo";
            else if (op == "=") type = "assignment";
            else if (op == "==") type = "equals";
            else if (op == "!=") type = "not_equals";
            else if (op == "<") type = "less_than";
            else if (op == ">") type = "greater_than";
            else if (op == "<=") type = "less_than_eq";
            else if (op == ">=") type = "greater_than_eq";
            else type = "operator";

            tokens.push_back(Token(type, op));
        } else if (is_print_statement(string(1, c))) {
            string print_stmt(1, c);
            while (i + 1 < code.size() && is_print_statement(print_stmt + code[i + 1])) {
                print_stmt += code[++i];
            }
            tokens.push_back(Token("print_statement", print_stmt));
        } else {
            tokens.push_back(Token("invalid", string(1, c)));
        }
    }
    return tokens;
}
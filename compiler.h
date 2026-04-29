#ifndef COMPILER_H
#define COMPILER_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;

// Shared Token Class
class Token {
public:
    string type;
    string value;
    Token() : type(""), value("") {}
    Token(const string& type, const string& value) : type(type), value(value) {}
};

// 'extern' tells the compiler these exist, but they are defined in lexical-analyzer.cpp
extern vector<string> keywords;
extern vector<string> punctuations;
extern vector<string> operators;
extern vector<string> print_statements;

// Shared CodeGenerator Class
class CodeGenerator {
private:
    int temp_counter;
public:
    CodeGenerator(); // Constructor
    vector<string> instructions;
    string new_temp();
    void emit(const string& instruction);
    void print_instructions() const;
};

// Function Prototypes
vector<Token> lexical_analyzer(string code);
void build_tree(const vector<Token>& tokens);
vector<Token> token_convert(vector<Token> line_tokens);

#endif
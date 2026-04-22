#include <iostream>
#include <string>
#include <vector>

using namespace std;

// Re-declare Token
class Token {
public:
    string type;
    string value;
    Token(const string& type, const string& value) : type(type), value(value) {}
};

class Node {
public:
    virtual ~Node() = default;
    virtual float evaluate() const = 0;
};

class NumberNode : public Node { // Python uses floats, not doubles
public:
    // Hold numbers as floats for Python compatibility
    float value;
    NumberNode(float value) : value(value) {}
    float evaluate() const override { return value; }
};

class BinaryOpNode : public Node {
public:
    // Left and right child nodes for binary operations
    Node* left;
    Node* right;
    BinaryOpNode(Node* left, Node* right) : left(left), right(right) {}
    ~BinaryOpNode() {
        delete left;
        delete right;
    }
};

class UnaryOpNode : public Node {
public:
    // Child node for unary operations
    Node* operand;
    UnaryOpNode(Node* operand) : operand(operand) {}
    ~UnaryOpNode() {
        delete operand;
    }
};

void build_tree(const vector<Token>& tokens) {
    if (tokens.empty()) return;

    // Implementation logic will go here
}
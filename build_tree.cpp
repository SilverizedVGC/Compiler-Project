#include "compiler.h"
#include <stack>

vector<string> symbol_names;
vector<float> symbol_values;

class ASM_Generator {
private:
    int temp_counter = 1;

public:
    vector<string> instructions;

    // Generates t1, t2, t3...
    string new_temp() {
        return "t" + to_string(temp_counter++);
    }

    // Stores the formatted TAC instruction
    void emit(const string& instruction) {
        instructions.push_back(instruction);
    }

    // Prints the final output
    void print_instructions() const {
        cout << "\n--- Assembly Code ---" << endl;
        for (const string& instr : instructions) {
            cout << instr << endl;
        }
    }
};

class Node {
public:
    virtual ~Node() = default;
    virtual float evaluate() const = 0; // Your existing interpreter method
    
    // The code generation method
    virtual string generate_ir(CodeGenerator& cg) const = 0; 
};

class NumberNode : public Node { 
public:
    // Hold numbers as floats for Python compatibility
    float value;
public:
    NumberNode(float value) : value(value) {}
    float evaluate() const override { return value; }

    string generate_ir(CodeGenerator& cg) const override {
        // Simply load the number's value as a string
        return to_string(value);
    }

    /*
    string generate_asm() {
        return;
    }
    */
};

class BinaryOpNode : public Node {
public:
    string op;
    // Left and right child nodes for binary operations
    Node* left;
    Node* right;
public:
    BinaryOpNode(const string& op, Node* left, Node* right) : op(op), left(left), right(right) {}
    ~BinaryOpNode() {
        delete left;
        delete right;
    }

    float evaluate() const override {
        float left_val = left->evaluate();
        float right_val = right->evaluate();

        if (op == "+") return left_val + right_val;
        if (op == "-") return left_val - right_val;
        if (op == "*") return left_val * right_val;
        if (op == "/") return (right_val != 0) ? left_val / right_val : 0.0f; // Basic div by zero check
        return 0.0f;
    }

    // For code gen, we will need to generate code based on the operator and the left/right nodes
    string generate_ir(CodeGenerator& cg) const override {
        // 1. Traverse left and right children recursively (Post-order)
        string left_val = left->generate_ir(cg);
        string right_val = right->generate_ir(cg);

        // 2. Get a new temp variable from the generator
        string temp = cg.new_temp();

        // 3. Emit the instruction (e.g., "t1 = 5 + 3")
        cg.emit(temp + " = " + left_val + " " + op + " " + right_val);

        // 4. Return the temp name so parent nodes can use it
        return temp;
    }
    
};

class UnaryOpNode : public Node {
public:
    string op;
    // Child node for unary operations
    Node* operand;
public:
    UnaryOpNode(const string& op, Node* operand) : op(op), operand(operand) {}
    ~UnaryOpNode() {
        delete operand;
    }

    float evaluate() const override {
        float val = operand->evaluate();
        if (op == "-") return -val;
        return val;
    }

    string generate_ir(CodeGenerator& cg) const override {
        // 1. Traverse the operand
        string operand_val = operand->generate_ir(cg);

        // 2. Get a new temp variable from the generator
        string temp = cg.new_temp();

        // 3. Emit the instruction (e.g., "t1 = -5")
        cg.emit(temp + " = " + op + " " + operand_val);

        // 4. Return the temp name so parent nodes can use it
        return temp;
    }
};

class VariableNode : public Node {
public:
    // Variable name for variable nodes
    string name;
public:
    VariableNode(const string& name) : name(name) {}

    float evaluate() const override {
        for (size_t i = 0; i < symbol_names.size(); ++i) {
                if (symbol_names[i] == name) {
                    return symbol_values[i];
                }
            }
            cerr << "Runtime Error: Undefined variable " << name << endl;
            return 0.0f;
    }

    string generate_ir(CodeGenerator& cg) const override {
        // Return the variable name as a string
        return name;
    }
};

class AssignmentNode : public Node {
public:
    // Variable name and expression for assignment
    string variable_name;
    Node* expression;
public:
    AssignmentNode(const string& variable_name, Node* expression) : variable_name(variable_name), expression(expression) {}
    ~AssignmentNode() {
        delete expression;
    }

    float evaluate() const override {
        float val = expression->evaluate();
        
        // Linear search for existing variable
        for (size_t i = 0; i < symbol_names.size(); ++i) {
            if (symbol_names[i] == variable_name) {
                symbol_values[i] = val;
                return val;
            }
        }
        
        // If not found, add new variable
        symbol_names.push_back(variable_name);
        symbol_values.push_back(val);
        return val;
    }

    string generate_ir(CodeGenerator& cg) const override {
        // 1. Traverse the expression on the right side of the equals sign
        string expr_val = expression->generate_ir(cg);

        // 2. Emit the final assignment to the actual variable name
        cg.emit(variable_name + " = " + expr_val);

        // 3. Return the variable name (or empty string, since assignment is the end of the line)
        return variable_name; 
    }
};

class ProgramNode : public Node {
public:
    // List of statements in the program
    vector<Node*> statements;
public:
    ProgramNode(const vector<Node*>& statements) : statements(statements) {}
    ~ProgramNode() {
        for (Node* stmt : statements) {
            delete stmt;
        }
    }

    float evaluate() const override {
        float last_val = 0.0f;
        for (Node* stmt : statements) {
            last_val = stmt->evaluate();
        }
        return last_val; // Returns the result of the last line executed
    }

    string generate_ir(CodeGenerator& cg) const override {
        // Loop through the vector of statement nodes and tell each one to generate
        for (Node* stmt : statements) {
            stmt->generate_ir(cg);
        }
        return "";
    }
};

void print_tree(const Node* node, int indent = 0) {
    if (!node) return;

    // Indent based on the level of the tree
    for (int i = 0; i < indent; ++i) cout << "  ";

    // Print the type of node
    if (dynamic_cast<const ProgramNode*>(node)) {
        const ProgramNode* prog_node = dynamic_cast<const ProgramNode*>(node);
        cout << "Program" << endl;
        for (Node* stmt : prog_node->statements) {
            print_tree(stmt, indent + 1); // Print all statements inside the program
        }
    } else if (dynamic_cast<const NumberNode*>(node)) {
        const NumberNode* num_node = dynamic_cast<const NumberNode*>(node);
        cout << "Number: " << num_node->evaluate() << endl;
    } else if (dynamic_cast<const VariableNode*>(node)) {
        const VariableNode* var_node = dynamic_cast<const VariableNode*>(node);
        cout << "Variable: " << var_node->name << endl;
    } else if (dynamic_cast<const BinaryOpNode*>(node)) {
        const BinaryOpNode* bin_node = dynamic_cast<const BinaryOpNode*>(node);
        cout << "Binary Operation: " << bin_node->op << endl;
        print_tree(bin_node->left, indent + 1); // Print left subtree
        print_tree(bin_node->right, indent + 1); // Print right subtree
    } else if (dynamic_cast<const UnaryOpNode*>(node)) {
        const UnaryOpNode* unary_node = dynamic_cast<const UnaryOpNode*>(node);
        cout << "Unary Operation" << endl;
        print_tree(unary_node->operand, indent + 1); // Print operand subtree
    } else if (dynamic_cast<const AssignmentNode*>(node)) {
        const AssignmentNode* assign_node = dynamic_cast<const AssignmentNode*>(node);
        cout << "Assignment: " << assign_node->variable_name << endl;
        print_tree(assign_node->expression, indent + 1); // Print expression subtree
    }
}

class Parser {
private:
    const vector<Token>& tokens;
    size_t pos;

    // Get current token
    Token current_token() const {
        if (pos < tokens.size()) return tokens[pos];
        return Token("EOF", ""); // End of file token
    }

    // Advance to the next token
    void advance() {
        if (pos < tokens.size()) pos++;
    }

public:
    // Parser class to build the syntax tree from tokens
    Parser(const vector<Token>& tokens) : tokens(tokens), pos(0) {}

    // Parses the entire file/program and collects all statements
    Node* parse_program() {
        vector<Node*> stmts;
        while (current_token().type != "EOF") {
            Node* stmt = parse_statement();
            if (stmt) {
                stmts.push_back(stmt);
            }
        }
        return new ProgramNode(stmts);
    }

    // Handle assignments and expressions
    Node* parse_statement() {
        // Verify if assignment
        if (current_token().type == "identifier" && pos + 1 < tokens.size() && tokens[pos + 1].value == "=") {
            Token var_token = current_token(); // Get the variable token
            advance(); // Move past the identifier 
            advance(); // Move past the '=' token
            Node* parse_expr = parse_expression(); // Parse the right-hand side expression
            return new AssignmentNode(var_token.value, parse_expr); // Create an assignment node
        }

        return parse_expression(); // If not an assignment, parse as an expression
    }

    // Handle + and - operators
    Node* parse_expression() {
        Node* left = parse_term(); // Parse the left term

        while (current_token().value == "+" || current_token().value == "-") {
            string op = current_token().value; // Get the operator
            advance(); // Move past the operator
            Node* right = parse_term(); // Parse the right term
            left = new BinaryOpNode(op, left, right); // Create a binary operation node
        }

        return left; // Return the left node
    }

    // Handle * and / operators
    Node* parse_term() {
        Node* left = parse_factor(); // Parse the left factor

        while (current_token().value == "*" || current_token().value == "/") {
            string op = current_token().value; // Get the operator
            advance(); // Move past the operator
            Node* right = parse_factor(); // Parse the right factor
            left = new BinaryOpNode(op, left, right); // Create a binary operation node
        }

        return left; // Return the left node
    }

    // Handle numbers and parentheses
    Node* parse_factor() {
        Token token = current_token(); // Get the current token
        if (token.type == "number") {
            advance(); // Move past the number token
            return new NumberNode(stof(token.value)); // Create a number node
        }

        else if (token.type == "identifier") {
            advance(); // Move past the identifier token
            return new VariableNode(token.value); // Create a variable node
        }

        if (token.value == "(") {
            advance(); // Move past the opening parenthesis
            Node* expr = parse_expression(); // Parse the expression inside the parentheses
            if (current_token().value != ")") {
                cerr << "Error: Expected closing parenthesis." << endl;
                return new NumberNode(0); // Return a default number node on error
            }
            advance(); // Move past the closing parenthesis
            return expr; // Return the expression node
        }

        cerr << "Error: Unexpected token in factor: " << token.value << endl;
        advance(); // Move past the unexpected token
        return new NumberNode(0); // Return a default number node on error
    }
};

void build_tree(const vector<Token>& tokens) {
    if (tokens.empty()) return;

    Parser parser(tokens); // Create a parser instance with the tokens
    Node* root = parser.parse_program(); // Parse the program

    // Print Tree
    cout << "\n--- Syntax Tree ---" << endl;
    print_tree(root); // Call the print_tree function to display the tree structure

    // Print Intermediate Code
    CodeGenerator cg;
    root->generate_ir(cg);
    cg.print_instructions();
    
    // Clean up memory
    delete root; // Delete the root node to free memory
}
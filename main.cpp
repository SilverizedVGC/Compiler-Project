#include <iostream>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

// Forward declaration of the Token class and functions
class Token {
public:
    string type;
    string value;
    Token(const string& type, const string& value) : type(type), value(value) {}
};

class Error {
public:
    Token token;
    int line_number;
    Error(const Token& token, int line_number) : token(token), line_number(line_number) {}
};

vector<Token> lexical_analyzer(string code);
void build_tree(const vector<Token>& tokens);

int main(void) {
    bool syntax_error = false;
    string filename;
    cout << "Enter the filename of the Python code: ";
    cin >> filename;
    
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return 1;
    }
    
    string line;
    // Initialize an array of tokens to hold the tokens from each line of the file
    vector<Token> all_tokens;

    vector<Error> errors; // Vector to hold syntax errors for reporting after processing the file

    int lines = 0;
    
    // Read file line by line and pass into lexical analyzer
    while (getline(file, line)) {
        lines++;
        vector<Token> line_tokens = lexical_analyzer(line);
        
        cout << "Line " << lines << ": ";
        if (line_tokens.empty()) {
            cout << "No tokens found." << endl;
            continue;
        }
        cout << endl;

        // Iterate through all tokens to print them AND check for errors
        for (const auto& token : line_tokens) {
            cout << token.type << ": " << token.value << endl;
            
            if (token.type == "SYNTAX_ERROR" || token.type == "unterminated_string" || token.type == "invalid_number" || token.type == "invalid") {
                syntax_error = true;
                errors.push_back(Error(token, lines)); // Store the actual error token
            }
        }

        // Append tokens to our main list
        all_tokens.insert(all_tokens.end(), line_tokens.begin(), line_tokens.end());
    }
    if (!syntax_error) build_tree(all_tokens); // Build the syntax tree for all tokens collected from the file if no syntax errors
    else {
        // Print syntax error tokens for debugging
        cout << "\n--- Syntax Errors Detected ---" << endl;
        for (const auto& error : errors) {
            cout << error.token.type << " in line " << error.line_number << ": " << error.token.value << endl; // Print the error message stored in the value field
        }
    }

    file.close();

    return 0;
}
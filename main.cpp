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

vector<Token> lexical_analyzer(string code);
void build_tree(const vector<Token>& tokens);

int main(void) {
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

    int lines = 0;
    
    // Read file line by line and pass into lexical analyzer
    while (getline(file, line)) {
        lines++;
        vector<Token> line_tokens = lexical_analyzer(line);
        // Debug: Print tokens for each line
        cout << "Line " << lines << ": " << endl;
        if (line_tokens.empty()) cout << "No tokens found." << endl;
        else {
            for (const auto& token : line_tokens) {
                cout << token.type << ": " << token.value << endl;
            }
        }
        // build_tree(line_tokens); // Build the syntax tree for the current line's tokens
        // Append tokens from this line to our main list
        all_tokens.insert(all_tokens.end(), line_tokens.begin(), line_tokens.end());
    }
    build_tree(all_tokens); // Build the syntax tree for all tokens collected from the file

    file.close();

    return 0;
}
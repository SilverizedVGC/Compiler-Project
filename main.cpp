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
    vector<Token> all_tokens;
    
    // Read file line by line and pass into lexical analyzer
    while (getline(file, line)) {
        vector<Token> line_tokens = lexical_analyzer(line);
        // Append tokens from this line to our main list
        all_tokens.insert(all_tokens.end(), line_tokens.begin(), line_tokens.end());
    }
    
    // Pass the collected tokens to the syntax tree builder
    build_tree(all_tokens);

    return 0;
}
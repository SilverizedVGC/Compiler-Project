#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <unistd.h>
#include <fstream>
using namespace std;

int main(void) {
    // File input
    string filename;
    cout << "Enter the filename of the Python code: ";
    cin >> filename;
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return 1;
    }
    // Read file line by line and pass into lexical analyzer
    string line;
    while (getline(file, line)) {
        lexical_analyzer(line);
    }
    // Note: Pass the tokens from the lexical analyzer to the syntax tree builder
    vector<string> tokens; // This should be filled with tokens from the lexical analyzer
    build_tree(tokens);




    return 0;
}
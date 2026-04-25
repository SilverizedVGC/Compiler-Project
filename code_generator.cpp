#include <iostream>
#include <string>
#include <vector>

using namespace std;

class CodeGenerator {
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
        cout << "\n--- Intermediate Representation (TAC) ---" << endl;
        for (const string& instr : instructions) {
            cout << instr << endl;
        }
    }
};
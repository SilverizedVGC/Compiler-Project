#include "compiler.h"

// The class definition is in compiler.h. Here we just define what the functions actually DO.
CodeGenerator::CodeGenerator() : temp_counter(1) {}

string CodeGenerator::new_temp() {
    return "t" + to_string(temp_counter++);
}

void CodeGenerator::emit(const string& instruction) {
    instructions.push_back(instruction);
}

void CodeGenerator::print_instructions() const {
    cout << "\n--- Intermediate Representation (TAC) ---" << endl;
    for (const string& instr : instructions) {
        cout << instr << endl;
    }
}
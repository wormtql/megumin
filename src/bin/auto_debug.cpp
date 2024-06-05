//
// Used to auto debug software implementation and hardware implementation differences
//

#include <random>
#include <iostream>

#include <random_instruction/RandomInstruction.h>
#include <random_instruction/RandomDataProcessingImm.h>
#include <random_instruction/RandomDataProcessingReg.h>
#include <program_mutation/RandomInstructionMutation.h>
#include <visitor/InstructionPrinter.h>

using namespace megumin;
using namespace arm;
using namespace std;

void print_instruction(const Instruction& ins) {
    InstructionPrinter printer{cout};
    printer.visit_instruction(ins);
}

int main() {
    std::mt19937 generator{1000};
    RandomInstructionMutation random_instruction_generator{generator, {}};
    random_instruction_generator.set_use_fp_instructions(false);
    random_instruction_generator.set_use_integral_instructions(true);

    Program prog;

    for (int i = 0; i < 10; i++) {
        auto instruction = random_instruction_generator.generate_instruction();
//        print_instruction(instruction);
//        cout << endl;
        prog.add_instruction(instruction);
    }
    prog.print(cout);

    return 0;
}
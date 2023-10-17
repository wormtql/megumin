#include <iostream>
#include <bitset>
#include <map>

#include <MachineState.h>
#include <Bitvec.h>
#include <Instruction.h>
#include <bb/BasicBlock.h>
#include <bb/BBExtractor.h>
#include <visitor/InstructionExecution.h>
#include <megumin_utils.h>

using namespace std;
using namespace arm;

int main() {
    MachineState s{};
    s.fp.set64(1, bits{(double) -3.5});
    InstructionExecution execution{s};

//    Program prog = megumin::aarch64_asm("fneg d2, d1").value();
    Program prog = megumin::aarch64_asm("frintn d2, d1").value();
    execution.execute(prog.get_instruction_const(0));

    cout << s.fp.get(64, 1).as_f64() << endl;
    cout << s.fp.get(64, 2).as_f64() << endl;

    return 0;
}

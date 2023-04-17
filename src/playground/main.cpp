#include <iostream>
#include <bitset>

#include <MachineState.h>
#include <Bitvec.h>
#include <Instruction.h>

using namespace arm;

int main() {
    MachineState state;

    // add x1, x2, #10
    const char* code = "\x41\x28\x00\x91";
    // sub x2, x1, #5
    const char* code2 = "\x22\x14\x00\xd1";
    // and x1, x2, #4
    const char* code3 = "\x41\x00\x7e\x92";
    uint32_t instr = *reinterpret_cast<const uint32_t*>(code);
    uint32_t i2 = *reinterpret_cast<const uint32_t*>(code2);
    uint32_t i3 = *reinterpret_cast<const uint32_t*>(code3);
//    std::cout << instr;
    std::cout << std::bitset<32>(i3) << std::endl;

    Instruction instruction{bits{32, instr}};
    Instruction instruction1{bits{32, i2}};
    Instruction instruction2{bits{32, i3}};
    instruction.execute(state);
    instruction1.execute(state);
    instruction2.execute(state);

    std::cout << state.gp.get32(1).as_u64() << std::endl;
    std::cout << state.gp.get32(2).as_u64() << std::endl;

    return 0;
}

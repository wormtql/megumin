#include <iostream>
#include <bitset>
#include <map>

#include <arm/arm.h>
#include <megumin_utils.h>

using namespace std;
using namespace arm;

/**
int f(int a) {
    if (a < 10) {
        return 0;
    } else {
        return 1;
    }
}

-->

f(int):
        sub     sp, sp, #16
        str     w0, [sp, 12]
        ldr     w0, [sp, 12]
        cmp     w0, 9
        bgt     .L2
        mov     w0, 0
        b       .L3
.L2:
        mov     w0, 1
.L3:
        add     sp, sp, 16
        ret

 简化：
.L0
     cmp w0, 9
     bgt .L2
.L1
     mov w0, 0
     b .L3
.L2
     mov w0, 1
.L3
     nop
 */

int main() {
    Program program{};

    std::vector<arm::Instruction> bb0 = megumin::aarch64_asm_basic_block("cmp w0, 9\nbgt 0").value();
    bb0[1].set_branch_target(2);
    std::vector<arm::Instruction> bb1 = megumin::aarch64_asm_basic_block("mov w0, 0\nb 0").value();
    bb1[1].set_branch_target(3);
    std::vector<arm::Instruction> bb2 = megumin::aarch64_asm_basic_block("mov w0, 1").value();

    program.add_basic_block(bb0, {{1, 2}});
    program.add_basic_block(bb1, {3});
    program.add_basic_block(bb2, {3});
    program.add_nop_basic_block();
    program.calc_in_connections_from_out_connections();

//    program.print();
//    auto min_def_ins = program.get_minimum_def_ins();
//    min_def_ins.print_one_line();
    program.calculate_def_ins();
    program.print_with_def_ins();

    return 0;
}

#include <cstdint>
#include <iostream>
#include <vector>
#include <climits>
#include <random>

#include <Instruction.h>
#include <Program.h>
#include <cost/CorrectnessCost.h>
#include <cost/SimpleCost.h>
#include <search/Search.h>
#include <program_mutation/WeightedProgramMutation.h>
#include <random_instruction/RandomDataProcessingImm.h>
#include <blackbox_program/blackbox_program.h>
#include <megumin_utils.h>

using namespace arm;
using std::cout;
using std::endl;
using std::vector;

void f(const arm::Program& target, vector<MachineState> test_cases, int init_mode = 1) {
    std::mt19937 generator{10000};

    megumin::SimpleCost simple_cost{target, std::move(test_cases)};

    megumin::WeightedProgramMutation weighted_program_mutation{generator};

    megumin::Search search{&weighted_program_mutation, &simple_cost, generator};
    if (target.is_all_integral_instructions()) {
        search.set_use_fp(false);
        search.set_use_integral(true);
    } else if (target.is_all_fp_instructions()) {
        search.set_use_fp(true);
        search.set_use_integral(false);
    } else {
        search.set_use_integral(true);
        search.set_use_fp(true);
    }
    megumin::SearchState state;

    arm::Program init_program{};

    if (init_mode == 0) {
        arm::Program empty_program{};
        for (int i = 0; i < target.get_size(); i++) {
            empty_program.add_instruction(Instruction::nop());
        }
        init_program = empty_program;
    } else if (init_mode == 1) {
        init_program = target;
    }


    RegSet min_def_ins = target.get_minimum_def_ins();
    min_def_ins.set_fp(1, true);
    min_def_ins.set_gp(1, true);
    cout << min_def_ins << endl;

    init_program.set_entry_def_ins(min_def_ins);
    init_program.calculate_def_ins();

    state.current = init_program;
    state.current_cost = simple_cost.cost(init_program, std::numeric_limits<double>::max()).second;
    cout << state.current_cost << endl;
    state.current_correct_best = target;
    state.current_correct_best_cost = state.current_cost;
    state.current_best = init_program;
    state.current_best_cost = state.current_cost;

    search.do_search(state);
}

int main() {
    // auto program = megumin::aarch64_asm("fmov d1, d2; fmov d3, d2");
//    auto program = megumin::aarch64_asm("add sp, x1, #10").value();
//    auto program = megumin::aarch64_asm("smulh x0, x1, x2").value();
    auto program = megumin::aarch64_asm(R"(ror	x10, x10, #32
add	x15, x15, x18
ror	x12, x12, #63
ror	x13, x13, #63
add	x9, x21, x9
add	x17, x17, x5
add	x16, x10, x16
eor	x2, x9, x2
add	x3, x3, x12
add	x1, x1, x13
eor	x11, x16, x11
eor	x0, x3, x0
eor	x4, x1, x4
ror	x2, x2, #24
mov	x24, x26
mov	x26, x18
ror	x11, x11, #24
ror	x0, x0, #32
ror	x4, x4, #32
add	x15, x15, x2
eor	x20, x15, x21)").value();
//    auto program = megumin::aarch64_asm("madd x0, x1, x2, x3").value();
//     auto program = megumin::aarch64_asm("and sp, x1, #16").value();
    // auto program = megumin::aarch64_asm("clz w1, w2; cls x3, x1");
//     auto program = megumin::aarch64_asm("sub sp, sp, #1, lsl #12; sub sp, sp, #1744").value();
    // auto program = megumin::aarch64_asm("sub sp, sp, #1, lsl #12");
    // auto program = megumin::aarch64_asm("sub sp, sp, #1");
    // auto program = megumin::aarch64_asm("extr x0, x1, x2, #5");
    // auto program = megumin::aarch64_asm("fadd d1, d2, d3");
//     auto program = megumin::aarch64_asm("fcmp d1, #0.0").value();
//     auto program = megumin::aarch64_asm(R"(fmadd	d0, d0, d1, d11
//	fmov	d1, #10.00000000
//	fabs	d0, d0
//	fmul	d0, d0, d1
//	fmadd	d0, d8, d9, d0)").value();
    // auto program = megumin::aarch64_asm("sub x10, x8, #1; lsl x11, x10, #4");
    // auto program = megumin::aarch64_asm("fadd d4, d5, d1");
    // auto program = megumin::aarch64_asm("mov w8, #3; mov w9, #7");
    // auto program = megumin::aarch64_asm("mov w9, #7; bfi x9, x27, #3, #61");
    // auto program = megumin::aarch64_asm("mov w8, #3");
    // auto program = megumin::aarch64_asm("movk x1, #100");
    // auto program = megumin::aarch64_asm("movz x1, #100");
    // auto program = megumin::aarch64_asm("movn x1, #100");
    // auto program = megumin::aarch64_asm("add x8, x23, x8, lsl #5");
//    auto program = megumin::aarch64_asm("add x8, x8, x2; lsl x18, x8, #1; lsl x3, x12, #5; add x23, x5, x3; mov w30, #96").value();
    // auto program = megumin::aarch64_asm("add x11, x9, x11; add x10, x9, x10");
    // auto program = megumin::aarch64_asm("add	x11, x4, #8;add	x12, x11, x9");
//    auto program = megumin::aarch64_asm("mov x0, x20; cmp x0, x20");
//    auto program = megumin::aarch64_asm(R"(subs x31, x20, x8
//mov x8, x11
//add x8, x8, x11
//subs x31, x8, x10
//csel x9, x10, x31, hi
//sub x8, x8, x9
//add x9, x8, x11
//subs x31, x9, x10
//csel x10, x10, x31, hi
//sub x9, x9, x10)").value();
//    auto program = megumin::aarch64_asm(R"(add x8, x8, x11
//cmp	x8, x10
//csel x9, x10, xzr, hi
//sub x8, x8, x9
//add x9, x8, x11
//cmp x9, x10
//csel x10, x10, xzr, hi
//sub x9, x9, x10)").value();

    cout << "size: " << program.get_size() << endl;
    program.print();
    cout << endl;

    std::vector<MachineState> test_cases;
    for (int i = 0; i < 100; i++) {
        test_cases.emplace_back(MachineState{});
        test_cases[i].fill_gp_random();
        test_cases[i].fill_fp_random();
        test_cases[i].fill_nzcv_random();
        test_cases[i].fill_sp_random();
    }

//    program.print();
    f(program, test_cases, 1);

//    megumin::CorrectnessCost cost{program, std::move(test_cases)};
//
//    Program rewrite;
//    rewrite.add_instruction(instruction);
////    rewrite.add_instruction(instruction2);
//
//    auto c = cost.cost(rewrite, DBL_MAX);
//    cout << "cost: " << c.second << endl;
}
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
    // add x1, x1, #10
    Instruction instruction{(void*)"\x21\x28\x00\x91"};
    // add x1, x1, #100
    Instruction instruction1{(void*)"\x21\x90\x01\x91"};
    // movk x1, #100
    Instruction instruction3{(void*)"\x81\x0c\x80\xf2"};
    // sbfm x1, x2, #10, #20
    Instruction instruction4{(void*)"\x41\x50\x0a\x13"};
    // extr x0, x1, x2, #5
    Instruction instruction5{(void*)"\x20\x14\xc2\x93"};
    // udiv x1, x2, x3
    Instruction instruction6{(void*)"\x41\x08\xc3\x9a"};
    // asrv x2, x1, x4
    Instruction instruction7{(void*)"\x22\x28\xc4\x9a"};
    // rev16 w1, w2
    Instruction instruction8{(void*)"\x41\x04\xc0\x5a"};
    // rev32 x2, x1
    Instruction instruction9{(void*)"\x22\x08\xc0\xda"};
    // clz w1, w2
    Instruction instruction10{(void*)"\x41\x10\xc0\x5a"};
    // cls x3, x1
    Instruction instruction11{(void*)"\x23\x14\xc0\xda"};
    // fmov d1, d2
    Instruction instruction12{(void*)"\x41\x40\x60\x1e"};
    // fabs d2, d3
    Instruction instruction13{(void*)"\x62\xc0\x60\x1e"};
    // fadd d1, d2, d3
    Instruction instruction14{(void*)"\x41\x28\x63\x1e"};
    // lsl x14, x14, #1
    Instruction instruction15{(void*)"\xce\xf9\x7f\xd3"};
    // and x1, x2, x10, lsl #2
    Instruction instruction16{(void*)"\x41\x08\x0a\x8a"};
    // add x1, x2, x3, lsl #2
    Instruction instruction17{(void*)"\x41\x08\x03\x8b"};
    // adc x3, x4, x10
    Instruction instruction18((void*)"\x83\x00\x0a\x9a");
    // sbcs x5, x6, x4
    Instruction instruction19{(void*)"\xc5\x00\x04\xfa"};
    // csinc x9, x9, xzr, hi
    Instruction instruction20{(void*)"\x29\x85\x9f\x9a"};
    // csel x9, x9, x10, ne
    Instruction instruction21{(void*)"\x29\x11\x8a\x9a"};

//     Program program;
//    program.add_instruction(instruction);
//    program.add_instruction(instruction1);
//    program.add_instruction(instruction3);
//    program.add_instruction(instruction4);
//    program.add_instruction(instruction5);
//    program.add_instruction(instruction6);
//    program.add_instruction(instruction7);
//    program.add_instruction(instruction8);
//    program.add_instruction(instruction9);
//    program.add_instruction(instruction10);
//    program.add_instruction(instruction11);
//     program.add_instruction(instruction12);
//     program.add_instruction(instruction13);
//     program.add_instruction(instruction14);
//     program.add_instruction(instruction15);
//    program.add_instruction(instruction16);
//    program.add_instruction(instruction17);
//    program.add_instruction(instruction18);
//    program.add_instruction(instruction19);
//    program.add_instruction(instruction20);
//    program.add_instruction(instruction21);

    // auto program = megumin::aarch64_asm("fmov d1, d2; fmov d3, d2");
//    auto program = megumin::aarch64_asm("add sp, x1, #10").value();
    auto program = megumin::aarch64_asm("smulh x0, x1, x2").value();
//    auto program = megumin::aarch64_asm("madd x0, x1, x2, x3").value();
//     auto program = megumin::aarch64_asm("and sp, x1, #16").value();
    // auto program = megumin::aarch64_asm("clz w1, w2; cls x3, x1");
//     auto program = megumin::aarch64_asm("sub sp, sp, #1, lsl #12; sub sp, sp, #1744").value();
    // auto program = megumin::aarch64_asm("sub sp, sp, #1, lsl #12");
    // auto program = megumin::aarch64_asm("sub sp, sp, #1");
    // auto program = megumin::aarch64_asm("extr x0, x1, x2, #5");
    // auto program = megumin::aarch64_asm("fadd d1, d2, d3");
    // auto program = megumin::aarch64_asm("fsub d1, d2, d3; fadd d4, d5, d2");
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
    f(program, test_cases, 0);

//    megumin::CorrectnessCost cost{program, std::move(test_cases)};
//
//    Program rewrite;
//    rewrite.add_instruction(instruction);
////    rewrite.add_instruction(instruction2);
//
//    auto c = cost.cost(rewrite, DBL_MAX);
//    cout << "cost: " << c.second << endl;
}
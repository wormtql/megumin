//
// Created by 58413 on 2023/4/18.
//

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
#include <utils/utils.h>

using namespace arm;
using std::cout;
using std::endl;
using std::vector;

void f(const arm::Program& target, vector<MachineState> test_cases) {
    std::mt19937 generator{1000};

    megumin::SimpleCost simple_cost{target, std::move(test_cases)};

//    megumin::RandomDataProcessingImm random_data_processing_imm{generator};
//    megumin::RandomInstructionTop random_instruction{generator};
//    megumin::MutateDataProcessingImmAddSub mutate_instruction{generator};
//    megumin::SimpleInClassMutation mutate_instruction{generator};
//    megumin::SimpleProgramMutation simple_program_mutation{generator, &random_instruction, &mutate_instruction};
    megumin::WeightedProgramMutation weighted_program_mutation{generator};

    megumin::Search search{&weighted_program_mutation, &simple_cost, generator};
    megumin::SearchState state;


    arm::Program empty_program{};
    for (int i = 0; i < target.get_size(); i++) {
        empty_program.add_instruction(Instruction::nop());
    }
    RegSet entry_def_ins;
    entry_def_ins.set_gp(1, true);
    entry_def_ins.set_fp(2, true);
    entry_def_ins.set_fp(3, true);
    empty_program.set_entry_def_ins(entry_def_ins);
    empty_program.calculate_def_ins();

    state.current = empty_program;
    state.current_cost = simple_cost.cost(empty_program, std::numeric_limits<double>::max()).second;
    state.current_correct_best = target;
    state.current_correct_best_cost = state.current_cost;
    state.current_best = empty_program;
    state.current_best_cost = state.current_cost;

    search.do_search(state);
}

int main() {
    // add x1, x1, #10
    Instruction instruction{(void*)"\x21\x28\x00\x91"};
    // add x1, x1, #100
    Instruction instruction1{(void*)"\x21\x90\x01\x91"};
    Instruction instruction2{(void*)"\x21\x90\x01\x91"};
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

     Program program;
    program.add_instruction(instruction);
//    program.add_instruction(instruction1);
//    program.add_instruction(instruction2);
//    program.add_instruction(instruction3);
//    program.add_instruction(instruction4);
//    program.add_instruction(instruction5);
//    program.add_instruction(instruction6);
//    program.add_instruction(instruction7);
//    program.add_instruction(instruction8);
//    program.add_instruction(instruction9);
//    program.add_instruction(instruction10);
//    program.add_instruction(instruction11);
    // program.add_instruction(instruction12);
     program.add_instruction(instruction13);
     program.add_instruction(instruction14);

#ifdef USE_KEYSTONE
    // auto program = megumin::aarch64_asm("fmov d1, d2; fmov d3, d2");
    // auto program = megumin::aarch64_asm("add x1, x1, #10");
    // auto program = megumin::aarch64_asm("clz w1, w2; cls x3, x1");
    // auto program = megumin::aarch64_asm("sub sp, sp, #1, lsl #12; sub sp, sp, #1744");
    // auto program = megumin::aarch64_asm("sub sp, sp, #1, lsl #12");
    // auto program = megumin::aarch64_asm("sub sp, sp, #1");
    // auto program = megumin::aarch64_asm("extr x0, x1, x2, #5");
    // auto program = megumin::aarch64_asm("fadd d1, d2, d3");
    // auto program = megumin::aarch64_asm("fsub d1, d2, d3; fadd d4, d5, d2");
    auto program = megumin::aarch64_asm("sub x10, x8, #1; lsl x11, x10, #4");
    // auto program = megumin::aarch64_asm("fadd d4, d5, d1");
#endif

    cout << "size: " << program.get_size() << endl;
    program.print();
    cout << endl;

    std::vector<MachineState> test_cases;
    for (int i = 0; i < 10; i++) {
        test_cases.emplace_back(MachineState{});
        test_cases[i].fill_gp_random();
        test_cases[i].fill_fp_random();
    }

//    program.print();
    f(program, test_cases);

//    megumin::CorrectnessCost cost{program, std::move(test_cases)};
//
//    Program rewrite;
//    rewrite.add_instruction(instruction);
////    rewrite.add_instruction(instruction2);
//
//    auto c = cost.cost(rewrite, DBL_MAX);
//    cout << "cost: " << c.second << endl;
}
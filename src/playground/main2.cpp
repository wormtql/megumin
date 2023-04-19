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
#include <mutation/SimpleProgramMutation.h>
#include <random_instruction/RandomDataProcessingImm.h>
#include <mutation/MutateDataProcessingImm.h>

using namespace arm;
using std::cout;
using std::endl;
using std::vector;

void f(const arm::Program& target, vector<MachineState> test_cases) {
    std::mt19937 generator{100};

    megumin::SimpleCost simple_cost{target, std::move(test_cases)};

    megumin::RandomDataProcessingImm random_data_processing_imm{generator};
    megumin::MutateDataProcessingImmAddSub mutate_instruction{generator};
    megumin::SimpleProgramMutation simple_program_mutation{generator, &random_data_processing_imm, &mutate_instruction};

    megumin::Search search{&simple_program_mutation, &simple_cost, generator};
    megumin::SearchState state;


    arm::Program empty_program{};
    for (int i = 0; i < target.get_size(); i++) {
        empty_program.add_instruction(Instruction::nop());
    }

    state.current = empty_program;
    state.current_cost = simple_cost.cost(empty_program, 10000000).second;
    state.current_correct_best = target;
    state.current_correct_best_cost = state.current_cost;
    state.current_best = empty_program;
    state.current_best_cost = state.current_cost;

    search.do_search(state);
}

int main() {
    // add x1, x2, #10
    const char* code = "\x41\x28\x00\x91";
    // sub x2, x1, #5
    const char* code2 = "\x22\x14\x00\xd1";
    // and x1, x2, #4
    const char* code3 = "\x41\x00\x7e\x92";

    uint32_t instr = *reinterpret_cast<const uint32_t*>(code);
    uint32_t i2 = *reinterpret_cast<const uint32_t*>(code2);
    uint32_t i3 = *reinterpret_cast<const uint32_t*>(code3);

    Instruction instruction{bits{32, instr}};
    Instruction instruction1{bits{32, i2}};
    Instruction instruction2{bits{32, i3}};

    Program program;
    program.add_instruction(instruction);
    program.add_instruction(instruction);
//    program.add_instruction(instruction2);

    std::vector<MachineState> test_cases;
    for (int i = 0; i < 10; i++) {
        test_cases.emplace_back(MachineState{});
        test_cases[i].fill_gp_random();
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
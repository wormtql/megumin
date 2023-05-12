//
// Created by 58413 on 2023/4/22.
//

#include "RandomInstructionTop.h"
#include "random_instruction/RandomDataProcessingImm.h"
#include "random_instruction/RandomDataProcessingReg.h"
#include "random_instruction/RandomFPAndSIMD.h"

namespace megumin {
    RandomInstructionTop::RandomInstructionTop(std::mt19937 &generator, Prob prob)
        : generator(generator),
          discrete{{
              prob.w_data_processing_imm,
              prob.w_data_processing_reg,
              prob.w_fp_and_simd,
          }}
    {
        dispatches.push_back(std::make_unique<RandomDataProcessingImm>(generator));
//        dispatches.push_back(std::make_unique<RandomDataProcessingImm>(generator));
        dispatches.push_back(std::make_unique<RandomDataProcessingReg>(generator));
        dispatches.push_back(std::make_unique<RandomFPAndSIMD>(generator));
    }

    arm::Instruction RandomInstructionTop::random_instruction() {
        int index = discrete(generator);
        auto& dispatch = dispatches[index];
        return dispatch->random_instruction();
    }
}
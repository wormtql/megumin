//
// Created by 58413 on 2023/5/11.
//

#include "RandomFPAndSIMD.h"
#include <Bitvec.h>

using arm::bits;

namespace megumin {
    arm::Instruction RandomFPDataProcessing1::random_instruction() {
        bits instruction{32, 0};

        instruction.set_range(24, 32, 0b00011110);
        instruction.set_bit(21, true);
        instruction.set_range(10, 15, 0b10000);

        // ptype
        instruction.set_range(22, 24, uniform_int(generator) % 2);
        // opcode, todo more opcode
        instruction.set_range(15, 21, uniform_int(generator) % 4);
        // rn
        instruction.set_range(5, 10, uniform_int(generator) % (1 << 5));
        // rd
        instruction.set_range(0, 5, uniform_int(generator) % (1 << 5));

        return arm::Instruction{instruction};
    }

}

// top RandomFPAndSIMD
namespace megumin {
    RandomFPAndSIMD::RandomFPAndSIMD(std::mt19937 &generator, Prob prob)
        : generator(generator),
          discrete({
              prob.w_fp_data_processing1
          })
    {
        dispatches.push_back(std::make_unique<RandomFPDataProcessing1>(generator));
    }

    arm::Instruction RandomFPAndSIMD::random_instruction() {
        int index = discrete(generator);
        return dispatches[index]->random_instruction();
    }
}
//
// Created by 58413 on 2023/4/20.
//

#include <Bitvec.h>
#include "MutateDataProcessingImm.h"
#include "SimpleInClassMutation.h"

using arm::bits;

namespace megumin {
    arm::Instruction SimpleInClassMutation::mutate(const arm::Instruction &instruction) {
        if (instruction.is_nop()) {
            return instruction;
        }

        auto type = instruction.get_type();
        if (type == arm::InstructionType::DataProcessingImm) {
            return mutate_data_processing_imm(instruction);
        } else if (type == arm::InstructionType::DataProcessingReg) {
            // todo
//            execute_data_processing_reg(state);
        }
        assert(false);
    }

    arm::Instruction SimpleInClassMutation::mutate_data_processing_imm(const arm::Instruction &instruction) {
        bits op0 = instruction.get_range(23, 26);
        if (op0 == 0b010) {
            return mutate_add_sub_imm->mutate(instruction);
        } else if (op0 == 0b011) {
            // todo
        } else if (op0 == 0b100) {
            return mutate_logical_imm->mutate(instruction);
        } else if (op0 == 0b101) {
            return mutate_move_wide_imm->mutate(instruction);
        } else if (op0 == 0b110) {
            // todo
        } else if (op0 == 0b111) {
            // todo
        }
        assert(false);
    }

    SimpleInClassMutation::SimpleInClassMutation(std::mt19937& generator) {
        mutate_add_sub_imm = new MutateDataProcessingImmAddSub(generator);
        mutate_logical_imm = new MutateDataProcessingImmLogical(generator);
        mutate_move_wide_imm = new MutateDataProcessingImmMoveWide(generator);
    }

    SimpleInClassMutation::~SimpleInClassMutation() {
        delete mutate_logical_imm;
        delete mutate_add_sub_imm;
        delete mutate_move_wide_imm;
    }
}

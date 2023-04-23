//
// Created by 58413 on 2023/4/20.
//

#include <Bitvec.h>
#include "MutateDataProcessingImm.h"
#include "SimpleInClassMutation.h"
#include "MutateDataProcessingReg.h"

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
            return mutate_data_processing_reg(instruction);
        }
        // todo
        assert(false);
    }

    arm::Instruction SimpleInClassMutation::mutate_data_processing_reg(const arm::Instruction &instruction) {
        bits op0 = instruction.get_range(29, 31);
        bool op1 = instruction.get_bit(28);
        bits op2 = instruction.get_range(21, 25);
        bits op3 = instruction.get_range(10, 16);

        if (op0 == 0 && op1 == 1 && op2 == 0b0110) {
            return mutate_source2->mutate(instruction);
//            return instruction;
        }
        // todo
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
            return mutate_bitfield->mutate(instruction);
        } else if (op0 == 0b111) {
            return mutate_extract->mutate(instruction);
        }
        assert(false);
    }

    SimpleInClassMutation::SimpleInClassMutation(std::mt19937& generator) {
        mutate_add_sub_imm = std::make_unique<MutateDataProcessingImmAddSub>(generator);
        mutate_logical_imm = std::make_unique<MutateDataProcessingImmLogical>(generator);
        mutate_move_wide_imm = std::make_unique<MutateDataProcessingImmMoveWide>(generator);
        mutate_bitfield = std::make_unique<MutateDataProcessingBitfield>(generator);
        mutate_extract = std::make_unique<MutateDataProcessingExtract>(generator);

        mutate_source2 = std::make_unique<MutateDataProcessingReg2Source>(generator);
    }
}

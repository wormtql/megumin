//
// Created by 58413 on 2023/4/14.
//

#include "Instruction.h"
#include "Bitvec.h"
#include "ArmUtils.h"

namespace arm {
    Instruction::Instruction(bits instruction): instruction(instruction) {}

    InstructionType Instruction::get_type() const {
//        Bitvec op0 = get_range(instruction, 25, 29);
        bits op0 = instruction.get_range(25, 29);
        if (op0 == 0) {
            return InstructionType::Reserved;
        } else if (op0 == 0b0001 || op0 == 0b0011) {
            return InstructionType::Reserved;
        } else if (op0 == 0b0010) {
            return InstructionType::SVE;
        } else if ((op0 >> 1) == 0b100) {
            return InstructionType::DataProcessingImm;
        } else if ((op0 >> 1) == 0b101) {
            return InstructionType::BranchExceptionSystem;
        } else if (op0.is_set(2) && !op0.is_set(0)) {
            return InstructionType::LoadAndStore;
        } else if ((op0 & 0b111) == 0b101) {
            return InstructionType::DataProcessingReg;
        } else if ((op0 & 0b111) == 0b111) {
            return InstructionType::DataProcessingSIMD;
        }
    }

//    InstructionType2 Instruction::get_type2() {
//        bits op0 = instruction.get_range(25, 29);
//        if (op0 == 0) {
//            return InstructionType::Reserved;
//        } else if (op0 == 0b0001 || op0 == 0b0011) {
//            return InstructionType::Reserved;
//        } else if (op0 == 0b0010) {
//            return InstructionType::SVE;
//        } else if ((op0 >> 1) == 0b100) {
//            return InstructionType::DataProcessingImm;
//        } else if ((op0 >> 1) == 0b101) {
//            return InstructionType::BranchExceptionSystem;
//        } else if (op0.is_set(2) && !op0.is_set(0)) {
//            return InstructionType::LoadAndStore;
//        } else if ((op0 & 0b111) == 0b101) {
//            return InstructionType::DataProcessingReg;
//        } else if ((op0 & 0b111) == 0b111) {
//            return InstructionType::DataProcessingSIMD;
//        }
//    }

    // https://developer.arm.com/documentation/ddi0596/2021-12/Index-by-Encoding/Data-Processing----Immediate?lang=en#addsub_imm
    void Instruction::execute_data_processing_imm_add_sub_imm(MachineState &state) const {
        bool sf = this->instruction.is_set(31);
        bool op = this->instruction.is_set(30);
        bool sh = this->instruction.is_set(22);
        bool S = this->instruction.is_set(29);
        bits imm12 = this->instruction.get_range(10, 22);
        bits rn = this->instruction.get_range(5, 10);
        bits rd = this->instruction.get_range(0, 5);

        if (!op && !S) {
            // add
            if (!sf) {
                // 32
                bits operand1 = state.gp.get32(rn.as_i32());
                bits imm = sh ? (imm12.zero_extend(32) << 12) : imm12.zero_extend(32);
                auto result = ArmUtils::add_with_carry(operand1, imm, false);
                state.gp.set32(rd.as_i32(), result.first);
            } else {
                // 64
                bits operand1 = state.gp.get64(rn.as_i32());
                bits imm = sh ? (imm12.zero_extend(64) << 12) : imm12.zero_extend(64);
                auto result = ArmUtils::add_with_carry(operand1, imm, false);
                state.gp.set64(rd.as_i32(), result.first);
            }
        } else if (!op && S) {
            // adds
            if (!sf) {
                // 32
                bits operand1 = state.gp.get32(rn.as_i32());
                bits imm = sh ? (imm12.zero_extend(32) << 12) : imm12.zero_extend(32);
                auto result = ArmUtils::add_with_carry(operand1, imm, false);
                state.gp.set32(rd.as_i32(), result.first);
                state.p_state.set_nzcv(result.second);
            } else {
                // 64
                bits operand1 = state.gp.get64(rn.as_i32());
                bits imm = sh ? (imm12.zero_extend(64) << 12) : imm12.zero_extend(64);
                auto result = ArmUtils::add_with_carry(operand1, imm, false);
                state.gp.set64(rd.as_i32(), result.first);
                state.p_state.set_nzcv(result.second);
            }
        } else if (op && !S) {
            if (!sf) {
                // 32
                bits operand1 = state.gp.get32(rn.as_i32());
                bits imm = sh ? (imm12.zero_extend(32) << 12) : imm12.zero_extend(32);
                bits operand2 = ~imm;
                auto result = ArmUtils::add_with_carry(operand1, operand2, true);
                state.gp.set32(rd.as_i32(), result.first);
            } else {
                // 64
                bits operand1 = state.gp.get64(rn.as_i32());
                bits imm = sh ? (imm12.zero_extend(64) << 12) : imm12.zero_extend(64);
                bits operand2 = ~imm;
                auto result = ArmUtils::add_with_carry(operand1, operand2, true);
                state.gp.set64(rd.as_i32(), result.first);
            }
        } else if (op && S) {
            // subs
            if (!sf) {
                bits operand1 = state.gp.get32(rn.as_i32());
                bits imm = sh ? (imm12.zero_extend(32) << 12) : imm12.zero_extend(32);
                bits operand2 = ~imm;
                auto result = ArmUtils::add_with_carry(operand1, operand2, true);
                state.gp.set32(rd.as_i32(), result.first);
                state.p_state.set_nzcv(result.second);
            } else {
                bits operand1 = state.gp.get64(rn.as_i32());
                bits imm = sh ? (imm12.zero_extend(64) << 12) : imm12.zero_extend(64);
                bits operand2 = ~imm;
                auto result = ArmUtils::add_with_carry(operand1, operand2, true);
                state.gp.set64(rd.as_i32(), result.first);
                state.p_state.set_nzcv(result.second);
            }
        }
    }

    void Instruction::execute_data_processing_imm_add_sub_imm_with_tags(MachineState &state) const {

    }

    void Instruction::execute_data_processing_imm_logical_imm(MachineState &state) const {
        bits opc = instruction.get_range(29, 31);
        bool sf = instruction.is_set(31);
        bool N = instruction.is_set(22);

        if (opc == 0b00) {
            // and
            bits immr = instruction.get_range(16, 22);
            bits imms = instruction.get_range(10, 16);
            bits rn = instruction.get_range(5, 10);
            bits rd = instruction.get_range(0, 5);

            auto d = rd.as_u32();
            auto n = rn.as_u32();
            int datasize = sf == 1 ? 64 : 32;

            if (!sf && N) {
                // undefined
            }

            bits imm = ArmUtils::decode_bit_mask(datasize, N, imms, immr, true).first;

            bits operand1 = state.gp.get(datasize, n);
            assert(operand1.size == datasize);
            bits result = operand1 & imm;
            state.gp.set(datasize, d, result);
        } else if (opc == 0b01) {
            // orr
            bits immr = instruction.get_range(16, 22);
            bits imms = instruction.get_range(10, 16);
            bits rn = instruction.get_range(5, 10);
            bits rd = instruction.get_range(0, 5);

            auto d = rd.as_u32();
            auto n = rn.as_u32();
            int datasize = sf == 1 ? 64 : 32;

            if (!sf && N) {
                // undefined
            }

            bits imm = ArmUtils::decode_bit_mask(datasize, N, imms, immr, true).first;

            bits operand1 = state.gp.get(datasize, n);
            assert(operand1.size == datasize);
            bits result = operand1 | imm;
            state.gp.set(datasize, d, result);
        } else if (opc == 0b10) {
            // eor
            bits immr = instruction.get_range(16, 22);
            bits imms = instruction.get_range(10, 16);
            bits rn = instruction.get_range(5, 10);
            bits rd = instruction.get_range(0, 5);

            auto d = rd.as_u32();
            auto n = rn.as_u32();
            int datasize = sf == 1 ? 64 : 32;

            if (!sf && N) {
                // undefined
            }

            bits imm = ArmUtils::decode_bit_mask(datasize, N, imms, immr, true).first;

            bits operand1 = state.gp.get(datasize, n);
            assert(operand1.size == datasize);
            bits result = operand1 ^ imm;
            state.gp.set(datasize, d, result);
        } else if (opc == 0b11) {
            // ands
            bits immr = instruction.get_range(16, 22);
            bits imms = instruction.get_range(10, 16);
            bits rn = instruction.get_range(5, 10);
            bits rd = instruction.get_range(0, 5);

            auto d = rd.as_u32();
            auto n = rn.as_u32();
            int datasize = sf == 1 ? 64 : 32;

            if (!sf && N) {
                // undefined
            }

            bits imm = ArmUtils::decode_bit_mask(datasize, N, imms, immr, true).first;

            bits operand1 = state.gp.get(datasize, n);
            assert(operand1.size == datasize);
            bits result = operand1 & imm;
            state.gp.set(datasize, d, result);

            bits nzcv = bits::from_bools({result.is_set(datasize - 1), result == 0, 0, 0});
            state.p_state.set_nzcv(nzcv);
        }
    }

    void Instruction::execute_data_processing_reg(MachineState &state) const {

    }

    // https://developer.arm.com/documentation/ddi0596/2021-12/Index-by-Encoding/Data-Processing----Immediate
    void Instruction::execute_data_processing_imm(MachineState &state) const {
        bits op0 = this->instruction.get_range(23, 26);
        if (op0 == 0b010) {
            execute_data_processing_imm_add_sub_imm(state);
        } else if (op0 == 0b011) {
            execute_data_processing_imm_add_sub_imm_with_tags(state);
        } else if (op0 == 0b100) {
            execute_data_processing_imm_logical_imm(state);
        } else if (op0 == 0b101) {

        } else if (op0 == 0b110) {

        } else if (op0 == 0b111) {

        }
    }

    void Instruction::execute(MachineState &state) const {
        if (instruction.data0 == 0) {
            // nop
            return;
        }
        auto type = get_type();
        if (type == InstructionType::DataProcessingImm) {
            execute_data_processing_imm(state);
        } else if (type == InstructionType::DataProcessingReg) {
            execute_data_processing_reg(state);
        }
    }

    void Instruction::set_bit(int index, bool value) {
        instruction.set_bit(index, value);
    }

    bool Instruction::get_bit(int index) const {
        assert(index < 32);
        return instruction.is_set(index);
    }

    void Instruction::set_range(int low, int high, int64_t value) {
        instruction.set_range(low, high, value);
    }

    void Instruction::set_as_nop() {
        instruction.data0 = 0;
    }

    bool Instruction::is_nop() const {
        return instruction.data0 == 0;
    }
}

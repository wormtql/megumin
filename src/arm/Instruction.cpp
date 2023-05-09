//
// Created by 58413 on 2023/4/14.
//

#include "Instruction.h"
#include "Bitvec.h"
#include "ArmUtils.h"

namespace arm {
    uint64_t Instruction::ID = 0;

    Instruction::Instruction(bits instruction): instruction(instruction) {
        id = ID++;
//        if (id == 8263965) {
//            printf("123");
//        }
    }

    Instruction::Instruction(const Instruction &other) {
        id = ID++;
        instruction = other.instruction;
    }

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
        // todo
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
                assert(false);
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
                assert(false);
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

    void Instruction::execute_data_processing_imm_move_wide_imm(MachineState &state) const {
        bits opc = instruction.get_range(29, 31);
        bool sf = instruction.is_set(31);
        bits hw = instruction.get_range(21, 23);
        bits imm16 = instruction.get_range(5, 21);
        bits rd = instruction.get_range(0, 5);

        int datasize = sf ? 64 : 32;
        if (!sf && hw.is_set(1)) {
            assert(false);
        }

        bits pos_bits = hw.concat(bits{4, 0});
        int pos = pos_bits.as_u32();

        if (opc == 0b00) {
            // movn
            bits result{datasize, 0};
            result.set_range(pos, pos + 16, imm16.as_i64());
            result = ~result;
            state.gp.set(datasize, rd.as_i32(), result);
        } else if (opc == 0b10) {
            // movz
            bits result{datasize, 0};
            result.set_range(pos, pos + 16, imm16.as_i64());
            state.gp.set(datasize, rd.as_i32(), result);
        } else if (opc == 0b11) {
            // movk
            bits result = state.gp.get_ref(rd.as_i32());
            result.set_range(pos, pos + 16, imm16.as_i64());
            state.gp.set(datasize, rd.as_i32(), result);
        } else {
            assert(false);
        }
    }

    void Instruction::execute_data_processing_imm_bitfield(MachineState &state) const {
        bool sf = instruction.is_set(31);
        bits opc = instruction.get_range(29, 31);
        bool N = instruction.is_set(22);
        bits immr = instruction.get_range(16, 22);
        bits imms = instruction.get_range(10, 16);
        bits rn = instruction.get_range(5, 10);
        bits rd = instruction.get_range(0, 5);

        int d = rd.as_i32();
        int n = rn.as_i32();

        int datasize = sf ? 64 : 32;

        bits wmask{datasize, 0};
        bits tmask{datasize, 0};

        assert(!(sf && !N));
        assert(!(!sf && (N || immr.is_set(5) || imms.is_set(5))));

        auto R = immr.as_u32();
        auto S = imms.as_u32();
        auto temp = ArmUtils::decode_bit_mask(datasize, N, imms, immr, false);
        wmask = temp.first;
        tmask = temp.second;

        if (opc == 0b00) {
            // sbfm
            bits src = state.gp.get_ref(n).resize(datasize);
            bits bot = ArmUtilsSharedFunctions::ror(src, R) & wmask;
            bits top = ArmUtils::replicate(src.get_range(S, S + 1), datasize);
            bits result = (top & ~tmask) | (bot & tmask);
            state.gp.set(datasize, d, result);
        } else if (opc == 0b01) {
            // bfm
            bits dst = state.gp.get_ref(d).resize(datasize);
            bits src = state.gp.get_ref(n).resize(datasize);
            bits bot = (dst & ~wmask) | (ArmUtilsSharedFunctions::ror(src, R) & wmask);
            bits result = (dst & ~tmask) | (bot & tmask);
            state.gp.set(datasize, d, result);
        } else if (opc == 0b10) {
            // ubfm
            bits src = state.gp.get_ref(n).resize(datasize);
            bits bot = ArmUtilsSharedFunctions::ror(src, R) & wmask;
            state.gp.set(datasize, d, bot & tmask);
        }
    }

    void Instruction::execute_data_processing_imm_extract(MachineState &state) const {
        bool sf = instruction.is_set(31);
        bits op21 = instruction.get_range(29, 31);
        bool N = instruction.is_set(22);
        bool o0 = instruction.is_set(21);
        bits rm = instruction.get_range(16, 21);
        bits imms = instruction.get_range(10, 16);
        bits rn = instruction.get_range(5, 10);
        bits rd = instruction.get_range(0, 5);

        int d = rd.as_i32();
        int n = rn.as_i32();
        int m = rm.as_i32();
        int datasize = sf ? 64 : 32;

        if (op21 == 0b00 && !o0) {
            // extr
            assert(N == sf);
            if (sf == 0 && imms.is_set(5)) {
                assert(false);
            }

            int lsb = imms.as_u32();
            bits operand1 = state.gp.get_ref(n).resize(datasize);
            bits operand2 = state.gp.get_ref(m).resize(datasize);

            if (lsb == 0) {
                state.gp.set(datasize, d, operand2);
            } else {
                bits low = operand2.get_range(lsb, datasize);
                bits high = operand1.get_range(0, lsb);
                bits result = low.concat(high);
                state.gp.set(datasize, d, result);
            }
        } else {
            assert(false);
        }
    }

    // https://developer.arm.com/documentation/ddi0596/2021-12/Index-by-Encoding/Data-Processing----Register?lang=en#dp_2src
    void Instruction::execute_data_processing_reg(MachineState &state) const {
        bool op0 = instruction.is_set(30);
        bool op1 = instruction.is_set(28);
        bits op2 = instruction.get_range(21, 25);
        bits op3 = instruction.get_range(10, 16);

        if (op0 == 0 && op1 == 1 && op2 == 0b0110) {
            execute_data_processing_reg_2_source(state);
        } else if (op0 == 1 && op1 == 1 && op2 == 0b0110) {
            execute_data_processing_reg_1_source(state);
        }
        else {
            assert(false);
        }
    }

    void Instruction::execute_data_processing_reg_2_source(MachineState &state) const {
        bool sf = instruction.is_set(31);
        bool S = instruction.is_set(29);
        bits rm = instruction.get_range(16, 21);
        bits opc = instruction.get_range(10, 16);
        bits rn = instruction.get_range(5, 10);
        bits rd = instruction.get_range(0, 5);

        int d = rd.as_u32();
        int n = rn.as_u32();
        int m = rm.as_u32();
        int datasize = sf ? 64 : 32;

        if (S == 0) {
            if (opc == 0b000010) {
                // udiv
                bits operand1 = state.gp.get_ref(n).resize(datasize);
                bits operand2 = state.gp.get_ref(m).resize(datasize);
                if (operand2 == 0) {
                    state.gp.set(datasize, d, bits{datasize, 0});
                } else {
                    uint64_t op1 = operand1.as_u64();
                    uint64_t op2 = operand2.as_u64();
                    uint64_t result = op1 / op2;
                    state.gp.set(datasize, d, bits{datasize, static_cast<int64_t>(result)});
                }
            } else if (opc == 0b000011) {
                // sdiv
                bits operand1 = state.gp.get_ref(n).resize(datasize);
                bits operand2 = state.gp.get_ref(m).resize(datasize);
                if (operand2 == 0) {
                    state.gp.set(datasize, d, bits{datasize, 0});
                } else {
                    int64_t op1 = operand1.as_i64();
                    int64_t op2 = operand2.as_i64();
                    int64_t result = op1 / op2;
                    state.gp.set(datasize, d, bits{datasize, result});
                }
            } else if ((opc >> 2) == 0b10) {
                // lslv/lsrv/asrv/rorv
                int shift_type = instruction.get_range(10, 12).as_i32();
                bits operand1 = state.gp.get_ref(n).resize(datasize);
                bits operand2 = state.gp.get_ref(m).resize(datasize);
                bits result = ArmUtils::shift_reg(operand1, shift_type, operand2.as_u32() % datasize);
                state.gp.set(datasize, d, result);
            } else {
                assert(false);
            }
        } else {
            assert(false);
        }

    }

    void Instruction::execute_data_processing_reg_1_source(MachineState &state) const {
        bool sf = instruction.is_set(31);
        bool S = instruction.is_set(29);
        bits opcode2 = instruction.get_range(16, 21);
        bits opcode = instruction.get_range(10, 16);
        bits rn = instruction.get_rn();
        bits rd = instruction.get_rd();

        int n = rn.as_u32();
        int d = rd.as_u32();
        int datasize = sf ? 64 : 32;

        assert(S == 0);

        if (opcode2 == 0 && opcode == 0) {
            // rbit
            bits operand = state.gp.get_ref(n).resize(datasize);
            bits result {datasize, 0};
            for (int i = 0; i < datasize; i++) {
                bool b = operand.is_set(datasize - 1 - i);
                result.set_bit(i, b);
            }
            state.gp.set(datasize, d, result);
        } else if (opcode2 == 0 && (opcode >> 2) == 0) {
            // rev
            bits opc = instruction.get_range(10, 12);
            int container_size = 0;
            if (opc == 0b01) {
                container_size = 16;
            } else if (opc == 0b10) {
                container_size = 32;
            } else if (opc == 0b11) {
                assert(sf == 1);
                container_size = 64;
            } else {
                assert(false);
            }

            bits operand = state.gp.get(datasize, n);

            int containers = datasize / container_size;
            assert(containers >= 1);
            int elements_per_container = container_size / 8;
            int index = 0;
            int rev_index;

            bits result{datasize, 0};

            for (int c = 0; c < containers; c++) {
                rev_index = index + ((elements_per_container - 1) * 8);
                for (int e = 0; e < elements_per_container; e++) {
                    assert(rev_index <= datasize);
                    assert(rev_index + 8 <= datasize);
                    assert(index <= datasize);
                    assert(index + 8 <= datasize);
                    result.set_range(rev_index, rev_index + 8, operand.get_range(index, index + 8).as_i64());
                    index += 8;
                    rev_index -= 8;
                }
            }

            state.gp.set(datasize, d, result);
        } else if (opcode2 == 0 && opcode == 0b000100) {
            // clz
            bits operand1 = state.gp.get(datasize, n);
            int result = ArmUtilsSharedFunctions::count_leading_zero_bits(operand1);
            state.gp.set(datasize, d, bits{datasize, result});
        } else if (opcode2 == 0 && opcode == 0b000101) {
            // cls
            bits operand1 = state.gp.get(datasize, n);
            int result = ArmUtilsSharedFunctions::count_leading_sign_bits(operand1);
            state.gp.set(datasize, d, bits{datasize, result});
        }
        else {
            assert(false);
        }
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
            execute_data_processing_imm_move_wide_imm(state);
        } else if (op0 == 0b110) {
            execute_data_processing_imm_bitfield(state);
        } else if (op0 == 0b111) {
            execute_data_processing_imm_extract(state);
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

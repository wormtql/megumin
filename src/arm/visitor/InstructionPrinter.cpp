//
// Created by 58413 on 2023/6/2.
//

#include "InstructionPrinter.h"
#include "megumin_utils.h"
#include "ArmUtils.h"

namespace arm {
    InstructionPrinter::InstructionPrinter(std::ostream &os)
        :os(os)
    {}

    void InstructionPrinter::print_shift_type(int shift_type) {
        if (shift_type == 0) {
            os << "lsl";
        } else if (shift_type == 1) {
            os << "lsr";
        } else if (shift_type == 2) {
            os << "asr";
        } else if (shift_type == 3) {
            os << "ror";
        }
    }

    void InstructionPrinter::print_cond(int cond) {
        switch (cond) {
            case 0b0000:
                os << "eq";
                break;
            case 0b0001:
                os << "ne";
                break;
            case 0b0010:
                os << "cs";
                break;
            case 0b0011:
                os << "cc";
                break;
            case 0b0100:
                os << "mi";
                break;
            case 0b0101:
                os << "pl";
                break;
            case 0b0110:
                os << "vs";
                break;
            case 0b0111:
                os << "vc";
                break;
            case 0b1000:
                os << "hi";
                break;
            case 0b1001:
                os << "ls";
                break;
            case 0b1010:
                os << "ge";
                break;
            case 0b1011:
                os << "lt";
                break;
            case 0b1100:
                os << "gt";
                break;
            case 0b1101:
                os << "le";
                break;
            case 0b1110:
                os << "al";
                break;
            case 0b1111:
                os << "nv";
                break;
        }
    }

    void InstructionPrinter::visit_nop(const Instruction &instruction) {
        os << "nop";
    }

    void InstructionPrinter::visit_dp_imm_add_sub(const Instruction &instruction) {
        bool sf = instruction.is_set(31);
        bool op = instruction.is_set(30);
        bool S = instruction.is_set(29);
        bool sh = instruction.is_set(22);
        bits imm12 = instruction.get_range(10, 22);
        bits rn = instruction.get_range(5, 10);
        bits rd = instruction.get_range(0, 5);

        if (!op) {
            os << "add";
        } else {
            os << "sub";
        }
        if (S) {
            os << "s";
        }

        os << " ";

        if (sf) os << "x"; else os << "w";
        os << rd.as_u64();
        os << ", ";
        if (sf) os << "x"; else os << "w";
        os << rn.as_u64();
        os << ", ";

        os << "#";
        os << imm12.as_u64();
        if (sh) {
            os << ", lsl #12";
        }
    }

    void InstructionPrinter::visit_dp_imm_add_sub_with_tags(const Instruction &instruction) {
        megumin::megumin_todo();
    }

    void InstructionPrinter::visit_dp_imm_logical(const Instruction &instruction) {
        bool sf = instruction.is_set(31);
        bits opc = instruction.get_range(29, 31);
        bool N = instruction.is_set(22);
        bits immr = instruction.get_range(16, 22);
        bits imms = instruction.get_range(10, 16);
        bits rn = instruction.get_range(5, 10);
        bits rd = instruction.get_range(0, 5);

        int n = rn.as_u64();
        int d = rd.as_u64();

        int datasize = sf ? 64 : 32;

        auto imm = ArmUtils::decode_bit_mask(datasize, N, imms, immr, true).first;
        if (opc == 0b00) {
            os << "and";
        } else if (opc == 0b01) {
            os << "orr";
        } else if (opc == 0b10) {
            os << "eor";
        } else if (opc == 0b11) {
            os << "ands";
        }
        os << " ";

        auto reg_name = sf ? "x" : "w";
        os << reg_name << d << ", ";
        os << reg_name << n << ", ";
        os << "#" << imm.as_u64();
    }

    void InstructionPrinter::visit_dp_imm_move_wide(const Instruction &instruction) {
        bits opc = instruction.get_range(29, 31);
        bool sf = instruction.is_set(31);
        bits hw = instruction.get_range(21, 23);
        bits imm16 = instruction.get_range(5, 21);
        bits rd = instruction.get_range(0, 5);

        if (opc == 0b00) {
            os << "movn";
        } else if (opc == 0b10) {
            os << "movz";
        } else if (opc == 0b11) {
            os << "movk";
        } else {
            assert(false);
        }
        os << " ";

        auto reg = sf ? "x" : "w";
        os << reg << rd.as_u64() << ", ";
        os << "#" << imm16.as_u64();
        if (hw != 0) {
            unsigned int shift = hw.as_u32() * 16;
            os << ", LSL #" << shift;
        }
    }

    void InstructionPrinter::visit_dp_imm_bitfield(const Instruction &instruction) {
        bool sf = instruction.is_set(31);
        bits opc = instruction.get_range(29, 31);
        bool N = instruction.is_set(22);
        bits immr = instruction.get_range(16, 22);
        bits imms = instruction.get_range(10, 16);
        bits rn = instruction.get_range(5, 10);
        bits rd = instruction.get_range(0, 5);

        auto reg = sf ? "x" : "w";

        if (opc == 00) {
            os << "sbfm";
        } else if (opc == 01) {
            os << "bfm";
        } else if (opc == 10) {
            if (sf) {
                if (imms != 0b111111 && imms + 1 == immr) {
                    os << "lsl";
                    os << " ";
                    os << reg << rd.as_i32() << ", ";
                    os << reg << rn.as_i32() << ", ";
                    os << "#" << (63 - imms.as_i32());
                    return;
                }
                if (imms == 0b111111) {
                    os << "lsr";
                    os << " ";
                    os << reg << rd.as_i32() << ", ";
                    os << reg << rn.as_i32() << ", ";
                    os << "#" << immr.as_i32();
                    return;
                }
            } else {
                if (imms != 0b011111 && imms + 1 == immr) {
                    os << "lsl";
                    os << " ";
                    os << reg << rd.as_i32() << ", ";
                    os << reg << rn.as_i32() << ", ";
                    os << "#" << (31 - imms.as_i32());
                    return;
                }
                if (imms == 0b011111) {
                    os << "lsr";
                    os << " ";
                    os << reg << rd.as_i32() << ", ";
                    os << reg << rn.as_i32() << ", ";
                    os << "#" << immr.as_i32();
                    return;
                }
            }
            os << "ubfm";
        } else {
            assert(false);
        }
        os << " ";


        os << reg << rd.as_u64() << ", ";
        os << reg << rn.as_u64() << ", ";
        os << "#" << immr.as_u64() << ", ";
        os << "#" << imms.as_u64();
    }

    void InstructionPrinter::visit_dp_imm_extract(const Instruction &instruction) {
        const bool sf = instruction.is_set(31);
        const bits rm = instruction.get_range(16, 21);
        const bits imms = instruction.get_range(10, 16);
        const bits rn = instruction.get_range(5, 10);
        const bits rd = instruction.get_range(0, 5);

        const auto reg = sf ? "x" : "w";

        os << "extr ";
        os << reg << rd.as_u64() << ", ";
        os << reg << rn.as_u64() << ", ";
        os << reg << rm.as_u64() << ", ";
        os << "#" << imms.as_u64();
    }

    void InstructionPrinter::visit_dp_reg_2source(const Instruction &instruction) {
        bool sf = instruction.is_set(31);
        bits rm = instruction.get_range(16, 21);
        bits opcode = instruction.get_range(10, 16);
        bits rn = instruction.get_range(5, 10);
        bits rd = instruction.get_range(0, 5);

        if (opcode == 0b000010) {
            os << "udiv";
        } else if (opcode == 0b000011) {
            os << "sdiv";
        } else if (opcode == 0b001000) {
            os << "lslv";
        } else if (opcode == 0b001001) {
            os << "lsrv";
        } else if (opcode == 0b001010) {
            os << "asrv";
        } else if (opcode == 0b001011) {
            os << "rorv";
        }
        os << " ";
        auto reg = sf ? "x" : "w";
        os << reg << rd.as_u64() << ", ";
        os << reg << rn.as_u64() << ", ";
        os << reg << rm.as_u64();
    }

    void InstructionPrinter::visit_dp_reg_1source(const Instruction &instruction) {
        bool sf = instruction.is_set(31);
        bits opcode2 = instruction.get_range(16, 21);
        bits opcode = instruction.get_range(10, 16);
        bits rn = instruction.get_range(5, 10);
        bits rd = instruction.get_range(0, 5);

        if (opcode2 == 0 && opcode == 0) {
            os << "rbit";
        } else if (opcode2 == 0 && opcode == 0b000001) {
            os << "rev16";
        } else if (opcode2 == 0 && opcode == 0b000010) {
            if (sf) {
                os << "rev32";
            } else {
                os << "rev";
            }
        } else if (opcode2 == 0 && opcode == 0b000011) {
            if (sf) {
                os << "rev";
            } else {
                assert(false);
            }
        } else if (opcode2 == 0 && opcode == 0b000100) {
            os << "clz";
        } else if (opcode2 == 0 && opcode == 0b000101) {
            os << "cls";
        }
        os << " ";

        auto reg = sf ? "x" : "w";
        os << reg << rd.as_u64() << ", ";
        os << reg << rn.as_u64();
    }

    void InstructionPrinter::visit_fp_simd_dp_1source(const Instruction &instruction) {
        bool M = instruction.is_set(31);
        bool S = instruction.is_set(29);
        bits ptype = instruction.get_range(22, 24);
        bits opcode = instruction.get_range(15, 21);
        bits rn = instruction.get_range(5, 10);
        bits rd = instruction.get_range(0, 5);

        assert(!M);
        assert(!S);

        if (opcode == 0b000000) {
            os << "fmov";
        } else if (opcode == 0b000001) {
            os << "fabs";
        } else if (opcode == 0b000010) {
            os << "fneg";
        } else if (opcode == 0b000011) {
            os << "fsqrt";
        } else if (opcode == 0b000101) {
            os << "fcvt";
        } else if (opcode == 0b000111) {
            os << "fcvt";
        }
        // todo

        char reg = '0';
        if (ptype == 0b00) {
            reg = 'S';
        } else if (ptype == 0b01) {
            reg = 'D';
        } else if (ptype == 0b11) {
            reg = 'H';
        } else {
            assert(false);
        }

        os << " ";
        os << reg << rd.as_u64() << ", ";
        os << reg << rn.as_u64();
    }

    void InstructionPrinter::visit_fp_simd_dp_2source(const Instruction &instruction) {
        bool M = instruction.is_set(31);
        bool S = instruction.is_set(29);
        bits ptype = instruction.get_range(22, 24);
        bits rm = instruction.get_range(16, 21);
        bits opcode = instruction.get_range(12, 16);
        bits rn = instruction.get_range(5, 10);
        bits rd = instruction.get_range(0, 5);

        if (opcode == 0b0000) {
            os << "fmul";
        } else if (opcode == 0b0001) {
            os << "fdiv";
        } else if (opcode == 0b0010) {
            os << "fadd";
        } else if (opcode == 0b0011) {
            os << "fsub";
        } else if (opcode == 0b0100) {
            os << "fmax";
        } else if (opcode == 0b0101) {
            os << "fmin";
        } else if (opcode == 0b0110) {
            os << "fmaxnm";
        } else if (opcode == 0b0111) {
            os << "fminnm";
        } else if (opcode == 0b1000) {
            os << "fnmul";
        }

        char reg = '0';
        if (ptype == 0b00) {
            reg = 'S';
        } else if (ptype == 0b01) {
            reg = 'D';
        } else if (ptype == 0b11) {
            reg = 'H';
        } else {
            assert(false);
        }

        os << " ";
        os << reg << rd.as_u32() << ", ";
        os << reg << rn.as_u32() << ", ";
        os << reg << rm.as_u32();
    }

    void InstructionPrinter::visit_dp_reg_logical_shifted_reg(const Instruction &instruction) {
        bool sf = instruction.is_set(31);
        bits opc = instruction.get_range(29, 31);
        bits shift = instruction.get_range(22, 24);
        bool N = instruction.is_set(21);
        bits rm = instruction.get_rm();
        bits imm6 = instruction.get_range(10, 16);
        bits rn = instruction.get_rn();
        bits rd = instruction.get_rd();

        auto reg = sf ? "x" : "w";

        if (opc == 0b00) {
            os << (N ? "bic" : "and");
        } else if (opc == 0b01) {
            if (N == 0 && shift == 0b00 && imm6 == 0b000000 && rn == 0b11111) {
                // alias to mov
                os << "mov " << reg << rd.as_i32() << ", " << reg << rm.as_i32();
                return;
            }
            os << (N ? "orn" : "orr");
        } else if (opc == 0b10) {
            os << (N ? "eon" : "eor");
        } else if (opc == 0b11) {
            os << (N ? "bics" : "ands");
        }
        os << " ";

        os << reg << rd.as_i32() << ", ";
        os << reg << rn.as_i32() << ", ";
        os << reg << rm.as_i32();
        if (imm6 != 0) {
            os << ", ";
            print_shift_type(shift.as_i32());
            os << " #" << imm6.as_u32();
        }
    }

    void InstructionPrinter::visit_dp_reg_add_sub_shifted_reg(const Instruction &instruction) {
        bool sf = instruction.is_set(31);
        bool op = instruction.is_set(30);
        bool S = instruction.is_set(29);
        bits shift = instruction.get_range(22, 24);
        bits rm = instruction.get_range(16, 21);
        bits imm6 = instruction.get_range(10, 16);
        bits rn = instruction.get_range(5, 10);
        bits rd = instruction.get_range(0, 5);

        if (op == 0) {
            os << (S ? "adds" : "add");
        } else {
            os << (S ? "subs" : "sub");
        }
        os << " ";
        auto reg = sf ? "x" : "w";
        os << reg << rd.as_i32() << ", ";
        os << reg << rn.as_i32() << ", ";
        os << reg << rm.as_i32();

        if (imm6 != 0) {
            os << ", ";
            print_shift_type(shift.as_i32());
            os << " #" << imm6.as_u32();
        }
    }

    void InstructionPrinter::visit_dp_reg_add_sub_with_carry(const Instruction &instruction) {
        bool sf = instruction.is_set(31);
        bool op = instruction.is_set(30);
        bool S = instruction.is_set(29);
        bits rm = instruction.get_rm();
        bits rn = instruction.get_rn();
        bits rd = instruction.get_rd();

        if (op == 0) {
            os << (S ? "adcs" : "adc");
        } else {
            os << (S ? "sbcs" : "sbc");
        }
        os << " ";
        auto reg = sf ? "x" : "w";
        os << reg << rd.as_i32() << ", ";
        os << reg << rn.as_i32() << ", ";
        os << reg << rm.as_i32();
    }

    void InstructionPrinter::visit_dp_reg_cond_select(const Instruction &instruction) {
        bool sf = instruction.is_set(31);
        bool op = instruction.is_set(30);
        bool S = instruction.is_set(29);
        bits rm = instruction.get_range(16, 21);
        bits cond = instruction.get_range(12, 16);
        bits op2 = instruction.get_range(10, 12);
        bits rn = instruction.get_range(5, 10);
        bits rd = instruction.get_range(0, 5);

        if (op == 0 && op2 == 0b00) {
            os << "csel";
        } else if (op == 0 && op2 == 0b01) {
            os << "csinc";
        } else if (op == 1 && op2 == 0b00) {
            os << "csinv";
        } else if (op == 1 && op2 == 0b01) {
            os << "csneg";
        }
        os << " ";

        auto reg = sf ? "x" : "w";
        os << reg << rd.as_i32() << ", ";
        os << reg << rn.as_i32() << ", ";
        os << reg << rm.as_i32() << ", ";
        print_cond(cond.as_i32());
    }
}

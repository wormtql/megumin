//
// Created by 58413 on 2023/5/11.
//

#include <memory>
#include <vector>

#include "RandomFPAndSIMD.h"
#include <Bitvec.h>

using arm::bits;

namespace megumin {
    arm::Instruction RandomFPDataProcessing1::random_instruction(const arm::Program& program, int index) {
        bits instruction{32, 0};

        instruction.set_range(24, 32, 0b00011110);
        instruction.set_bit(21, true);
        instruction.set_range(10, 15, 0b10000);

        static std::vector<int> opcodes = {
                0b000000, // fmov
                0b000001, // fabs
                0b000010, // fneg
                0b000011, // fsqrt
                0b000101, // fcvt
//                0b000111, // fcvt single to half
                0b001000, // frintn
                0b001001, // frintp
                0b001010, // frintm
                0b001011, // frintz
                0b001100, // frinta
//                0b001110, // frintx
//                0b001111, // frinti
        };

        // ptype
        instruction.set_range(22, 24, uniform_int(generator) % 2);
        // opcode
        auto opcode_index = uniform_int(generator) % opcodes.size();
        instruction.set_range(15, 21, opcodes[opcode_index]);
        // rn
        const auto& def_ins = program.get_def_in(index);
        instruction.set_range(5, 10, def_ins.random_fp(generator));
        // rd
        instruction.set_range(0, 5, uniform_int(generator) % (1 << 5));

        return arm::Instruction{instruction};
    }

    arm::Instruction RandomFPDataProcessing1::random_instruction() {
        bits instruction{32, 0};

        instruction.set_range(24, 32, 0b00011110);
        instruction.set_bit(21, true);
        instruction.set_range(10, 15, 0b10000);

        static std::vector<int> opcodes = {
                0b000000, // fmov
                0b000001, // fabs
                0b000010, // fneg
                0b000011, // fsqrt
                0b000101, // fcvt
//                0b000111, // fcvt single to half
                0b001000, // frintn
                0b001001, // frintp
                0b001010, // frintm
                0b001011, // frintz
                0b001100, // frinta
//                0b001110, // frintx
//                0b001111, // frinti
        };

        // ptype
        instruction.set_range(22, 24, uniform_int(generator) % 2);
        // opcode
        auto opcode_index = uniform_int(generator) % opcodes.size();
        instruction.set_range(15, 21, opcodes[opcode_index]);
        // rn
        instruction.set_range(5, 10, uniform_int(generator) % (1 << 5));
        // rd
        instruction.set_range(0, 5, uniform_int(generator) % (1 << 5));

        return arm::Instruction{instruction};
    }
}

// fp 2-source
namespace megumin {
    arm::Instruction RandomFPDataProcessing2::random_instruction(const arm::Program& program, int index) {
        bits instruction{32, 0};
        
        instruction.set_range(24, 32, 0b00011110);
        instruction.set_bit(21, true);
        instruction.set_range(10, 12, 0b10);

        // ptype
        instruction.set_range(22, 24, uniform_int(generator) % 2);
        // rm
        const auto& def_ins = program.get_def_in(index);
        instruction.set_range(16, 21, def_ins.random_fp(generator));
        // opcode
        instruction.set_range(12, 16, uniform_int(generator) % 2 + 2);
        // rn
        instruction.set_range(5, 10, def_ins.random_fp(generator));
        // rd
        instruction.set_range(0, 5, uniform_int(generator) % (1 << 5));

        return arm::Instruction{instruction};
    }

    arm::Instruction RandomFPDataProcessing2::random_instruction() {
        bits instruction{32, 0};

        instruction.set_range(24, 32, 0b00011110);
        instruction.set_bit(21, true);
        instruction.set_range(10, 12, 0b10);

        // ptype
        instruction.set_range(22, 24, uniform_int(generator) % 2);
        // rm
        instruction.set_range(16, 21, uniform_int(generator) % 32);
        // opcode
        instruction.set_range(12, 16, uniform_int(generator) % 2 + 2);
        // rn
        instruction.set_range(5, 10, uniform_int(generator) % 32);
        // rd
        instruction.set_range(0, 5, uniform_int(generator) % (1 << 5));

        return arm::Instruction{instruction};
    }
}

// fp 3-source
namespace megumin {
    arm::Instruction RandomFPDataProcessing3::random_instruction(const arm::Program &program, int index) {
        bits instruction{32, 0};

        instruction.set_range(24, 32, 0b00011111);

        // ptype
        instruction.set_range(22, 24, uniform_int(generator) % 2);
        // o1
        instruction.set_bit(21, uniform_int(generator) % 2);
        // rm
        const auto& def_ins = program.get_def_in(index);
        instruction.set_range(16, 21, def_ins.random_fp(generator));
        // o0
        instruction.set_bit(15, uniform_int(generator) % 2);
        // ra
        instruction.set_range(10, 15, def_ins.random_fp(generator));
        // rn
        instruction.set_range(5, 10, def_ins.random_fp(generator));
        // rd
        instruction.set_range(0, 5, uniform_int(generator) % (1 << 5));

        return arm::Instruction{instruction};
    }

    arm::Instruction RandomFPDataProcessing3::random_instruction() {
        bits instruction{32, 0};

        instruction.set_range(24, 32, 0b00011111);

        // ptype
        instruction.set_range(22, 24, uniform_int(generator) % 2);
        // o1
        instruction.set_bit(21, uniform_int(generator) % 2);
        // rm
        instruction.set_range(16, 21, uniform_int(generator) % 32);
        // o0
        instruction.set_bit(15, uniform_int(generator) % 2);
        // ra
        instruction.set_range(10, 15, uniform_int(generator) % 32);
        // rn
        instruction.set_range(5, 10, uniform_int(generator) % 32);
        // rd
        instruction.set_range(0, 5, uniform_int(generator) % (1 << 5));

        return arm::Instruction{instruction};
    }
}

// fp imm
namespace megumin {
    arm::Instruction RandomFPImm::random_instruction(const arm::Program &program, int index) {
        bits instruction{32, 0};

        instruction.set_range(24, 32, 0b00011110);
        instruction.set_bit(21, true);
        instruction.set_range(5, 13, 0b10000000);

        // ptype
        instruction.set_range(22, 24, uniform_int(generator) % 2);
        // imm8
        instruction.set_range(13, 21, uniform_int(generator) % (1 << 8));
        // rd
        instruction.set_range(0, 5, uniform_int(generator) % (1 << 5));

        return arm::Instruction{instruction};
    }

    arm::Instruction RandomFPImm::random_instruction() {
        bits instruction{32, 0};

        instruction.set_range(24, 32, 0b00011110);
        instruction.set_bit(21, true);
        instruction.set_range(5, 13, 0b10000000);

        // ptype
        instruction.set_range(22, 24, uniform_int(generator) % 2);
        // imm8
        instruction.set_range(13, 21, uniform_int(generator) % (1 << 8));
        // rd
        instruction.set_range(0, 5, uniform_int(generator) % (1 << 5));

        return arm::Instruction{instruction};
    }
}

// fp compare
namespace megumin {
    arm::Instruction RandomFPCompare::random_instruction(const arm::Program &program, int index) {
        bits instruction{32, 0};

        instruction.set_range(24, 32, 0b00011110);
        instruction.set_bit(21, true);
        instruction.set_range(10, 16, 0b001000);
        instruction.set_range(0, 3, 0b000);

        // ptype
        instruction.set_range(22, 24, uniform_int(generator) % 2);
        // rm
        const auto& def_ins = program.get_def_in(index);
        instruction.set_range(16, 21, def_ins.random_fp(generator));
        // rn
        instruction.set_range(5, 10, def_ins.random_fp(generator));
        // opcode2
        instruction.set_range(3, 5, uniform_int(generator) % 4);

        return arm::Instruction{instruction};
    }

    arm::Instruction RandomFPCompare::random_instruction() {
        bits instruction{32, 0};

        instruction.set_range(24, 32, 0b00011110);
        instruction.set_bit(21, true);
        instruction.set_range(10, 16, 0b001000);
        instruction.set_range(0, 3, 0b000);

        // ptype
        instruction.set_range(22, 24, uniform_int(generator) % 2);
        // rm
        instruction.set_range(16, 21, uniform_int(generator) % 32);
        // rn
        instruction.set_range(5, 10, uniform_int(generator) % 32);
        // opcode2
        instruction.set_range(3, 5, uniform_int(generator) % 4);

        return arm::Instruction{instruction};
    }
}

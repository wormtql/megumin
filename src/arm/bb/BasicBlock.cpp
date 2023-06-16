//
// Created by wormtql on 2023/6/13.
//

#include "BasicBlock.h"
#include "InstructionPrinter.h"

namespace arm {
    BasicBlock::BasicBlock(std::vector<Instruction> &&instructions, int start, int end)
        : instructions(std::move(instructions)), start(start), end(end)
    {}
}

std::ostream& operator<<(std::ostream& os, const arm::BasicBlock& bb) {
    os << "loc: (" << bb.get_start() << ", " << bb.get_end() << ")\n";

    arm::InstructionPrinter printer{os};

    for (const auto& inst: bb.get_instructions()) {
        printer.visit_instruction(inst);
        os << "\n";
    }

    return os;
}

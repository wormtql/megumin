//
// Created by wormtql on 2023/6/13.
//

#include "BasicBlock.h"

namespace arm {
    BasicBlock::BasicBlock(std::vector<Instruction> &&instructions, int start, int end)
        : instructions(std::move(instructions)), start(start), end(end)
    {}
}

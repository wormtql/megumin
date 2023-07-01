//
// Created by 58413 on 2023/6/27.
//

#ifndef MEGUMIN_INSTRUCTIONEXECUTIONS_H
#define MEGUMIN_INSTRUCTIONEXECUTIONS_H

#include "MachineStateS.h"
#include "InstructionVisitor.h"

namespace arm {
    class InstructionExecutionS: public InstructionVisitor {
    private:
        MachineStateS& state;
    public:
        explicit InstructionExecutionS(MachineStateS& state): state(state) {}

        void visit_dp_imm_add_sub(const Instruction &instruction) override;
    };
}



#endif //MEGUMIN_INSTRUCTIONEXECUTIONS_H

//
// Created by 58413 on 2023/4/19.
//

#ifndef MEGUMIN_INSTRUCTIONPRINT_H
#define MEGUMIN_INSTRUCTIONPRINT_H

#include <iostream>
#include "InstructionDispatch.h"

namespace arm {
    class InstructionPrint: public InstructionDispatch {
    private:
        std::ostream& os;
    public:
        explicit InstructionPrint(std::ostream& os): os(os) {}

        void dispatch_data_processing_imm_add_sub(const Instruction &instruction) override;
    };
}



#endif //MEGUMIN_INSTRUCTIONPRINT_H

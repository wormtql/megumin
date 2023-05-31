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

        void dispatch_data_processing_bitfield(const Instruction &instruction) override;

        void dispatch_data_processing_imm_logical(const Instruction& instruction) override;

        void dispatch_data_processing_extract(const Instruction &instruction) override;

        void dispatch_data_processing_move_wide(const Instruction &instruction) override;

        void dispatch_nop(const Instruction &instruction) override;

        void dispatch_data_processing_1source(const Instruction &instruction) override;

        void dispatch_data_processing_2source(const Instruction &instruction) override;

        void dispatch_fp_data_processing1(const Instruction &instruction) override;

        void dispatch_fp_data_processing2(const Instruction& instruction) override;
    };
}



#endif //MEGUMIN_INSTRUCTIONPRINT_H

//
// Created by 58413 on 2023/4/17.
//

#ifndef MEGUMIN_INSTRUCTIONDISPATCH_H
#define MEGUMIN_INSTRUCTIONDISPATCH_H

#include "Instruction.h"

namespace arm {
    class InstructionDispatch {
    private:

    public:
        InstructionDispatch() = default;
        virtual ~InstructionDispatch() = default;

        void dispatch(const Instruction& instruction);

        virtual bool need_dispatch_data_processing_imm() {
            return true;
        }
        void dispatch_data_processing_imm(const Instruction& instruction);

        virtual bool need_dispatch_data_processing_reg() { return true; }
        void dispatch_data_processing_reg(const Instruction& instruction);

        virtual bool need_dispatch_data_processing_imm_add_sub() {
            return true;
        }
        virtual void dispatch_data_processing_imm_add_sub(const Instruction& instruction) {}

        virtual bool need_dispatch_data_processing_imm_add_sub_with_tags() {
            return true;
        }
        virtual void dispatch_data_processing_imm_add_sub_with_tags(const Instruction& instruction) {}

        virtual bool need_dispatch_data_processing_imm_logical() {
            return true;
        }
        virtual void dispatch_data_processing_imm_logical() {}

        virtual bool need_dispatch_data_processing_move_wide() {
            return true;
        }
        virtual void dispatch_data_processing_move_wide() {}

        virtual bool need_dispatch_data_processing_bitfield() {
            return true;
        }
        virtual void dispatch_data_processing_bitfield() {}

        virtual bool need_dispatch_data_processing_extract() {
            return true;
        }
        virtual void dispatch_data_processing_extract() {}
    };
}


#endif //MEGUMIN_INSTRUCTIONDISPATCH_H

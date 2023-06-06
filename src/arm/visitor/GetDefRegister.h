//
// Created by 58413 on 2023/6/3.
//

#ifndef MEGUMIN_GETDEFREGISTER_H
#define MEGUMIN_GETDEFREGISTER_H

#include "InstructionVisitor.h"
#include "RegSet.h"

namespace arm {
    class GetDefRegister: public InstructionVisitor {
    public:
        Reg result;

        void visit_dp_imm_add_sub(const Instruction &instruction) override;

        void visit_dp_imm_add_sub_with_tags(const Instruction &instruction) override;

        void visit_dp_imm_logical(const Instruction &instruction) override;

        void visit_dp_imm_move_wide(const Instruction &instruction) override;

        void visit_dp_imm_bitfield(const Instruction &instruction) override;

        void visit_dp_imm_extract(const Instruction &instruction) override;

        void visit_dp_reg_2source(const Instruction &instruction) override;

        void visit_dp_reg_1source(const Instruction &instruction) override;

        void visit_fp_simd_dp_1source(const Instruction &instruction) override;

        void visit_fp_simd_dp_2source(const Instruction &instruction) override;
    };
}



#endif //MEGUMIN_GETDEFREGISTER_H
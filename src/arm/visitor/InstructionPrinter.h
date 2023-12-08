//
// Created by 58413 on 2023/6/2.
//

#ifndef MEGUMIN_INSTRUCTIONPRINTER_H
#define MEGUMIN_INSTRUCTIONPRINTER_H

#include <iostream>

#include "InstructionVisitor.h"

namespace arm {
    class InstructionPrinter: public InstructionVisitor {
    private:
        std::ostream& os;

        void print_shift_type(int shift_type);

        void print_cond(int cond);
    public:
        explicit InstructionPrinter(std::ostream& os);

        void visit_nop(const Instruction &instruction) override;

        void visit_dp_imm_add_sub(const Instruction &instruction) override;

        void visit_dp_imm_add_sub_with_tags(const Instruction &instruction) override;

        void visit_dp_imm_logical(const Instruction &instruction) override;

        void visit_dp_imm_move_wide(const Instruction &instruction) override;

        void visit_dp_imm_bitfield(const Instruction &instruction) override;

        void visit_dp_imm_extract(const Instruction &instruction) override;

        void visit_dp_reg_2source(const Instruction &instruction) override;

        void visit_dp_reg_1source(const Instruction &instruction) override;

        void visit_dp_reg_logical_shifted_reg(const Instruction &instruction) override;

        void visit_dp_reg_add_sub_shifted_reg(const Instruction &instruction) override;

        void visit_dp_reg_cond_select(const Instruction &instruction) override;

        void visit_dp_reg_add_sub_with_carry(const Instruction &instruction) override;

        void visit_fp_simd_dp_1source(const Instruction &instruction) override;

        void visit_fp_simd_dp_2source(const Instruction &instruction) override;

        void visit_dp_reg_3source(const Instruction &instruction) override;

        void visit_fp_simd_imm(const Instruction &instruction) override;

        void visit_fp_simd_dp_3source(const Instruction &instruction) override;

        void visit_fp_compare(const Instruction &instruction) override;
    };
}



#endif //MEGUMIN_INSTRUCTIONPRINTER_H

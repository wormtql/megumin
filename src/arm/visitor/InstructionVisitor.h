#ifndef INSTRUCTION_VISITOR_H
#define INSTRUCTION_VISITOR_H

#include "Instruction.h"

namespace arm {
    class InstructionVisitor {
    private:
    public:
        virtual ~InstructionVisitor() = default;

        void visit_instruction(const Instruction& instruction);
        void visit_data_processing_imm(const Instruction& instruction);
        void visit_data_processing_reg(const Instruction& instruction);
        void visit_fp_and_simd(const Instruction& instruction);

        virtual void visit_nop(const Instruction& instruction) {};
        virtual void visit_dp_imm_add_sub(const Instruction& instruction) {};
        virtual void visit_dp_imm_add_sub_with_tags(const Instruction& instruction) {};
        virtual void visit_dp_imm_logical(const Instruction& instruction) {};
        virtual void visit_dp_imm_move_wide(const Instruction& instruction) {};
        virtual void visit_dp_imm_bitfield(const Instruction& instruction) {};
        virtual void visit_dp_imm_extract(const Instruction& instruction) {};

        virtual void visit_dp_reg_2source(const Instruction& instruction) {};
        virtual void visit_dp_reg_1source(const Instruction& instruction) {};
        virtual void visit_dp_reg_logical_shifted_reg(const Instruction& instruction) {};
        virtual void visit_dp_reg_add_sub_shifted_reg(const Instruction& instruction) {};
        virtual void visit_dp_reg_add_sub_with_carry(const Instruction& instruction) {};

        virtual void visit_fp_simd_dp_1source(const Instruction& instruction) {};
        virtual void visit_fp_simd_dp_2source(const Instruction& instruction) {};

    };
}

#endif
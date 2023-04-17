//
// Created by 58413 on 2023/4/17.
//

#ifndef MEGUMIN_INSTRUCTIONDISPATCH_H
#define MEGUMIN_INSTRUCTIONDISPATCH_H


namespace arm {
    class InstructionDispatch {
    private:

    public:
        InstructionDispatch() = default;
        virtual ~InstructionDispatch() = default;

//        void dispatch()

        virtual bool dispatch_data_processing_imm() {
            return true;
        }

        virtual bool dispatch_data_processing_imm_add_sub() {
            return true;
        }

        virtual bool dispatch_data_processing_imm_add_sub_with_tags() {
            return true;
        }

        virtual bool dispatch_data_processing_imm_logical() {
            return true;
        }

        virtual bool dispatch_data_processing_move_wide() {
            return true;
        }

        virtual bool dispatch_data_processing_bitfield() {
            return true;
        }

        virtual bool dispatch_data_processing_extract() {
            return true;
        }
    };
}


#endif //MEGUMIN_INSTRUCTIONDISPATCH_H

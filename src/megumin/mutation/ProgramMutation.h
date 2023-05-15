//
// Created by 58413 on 2023/4/18.
//

#ifndef MEGUMIN_PROGRAMMUTATION_H
#define MEGUMIN_PROGRAMMUTATION_H

#include <Program.h>

namespace megumin {
    class ProgramMutation {
    public:
        virtual ~ProgramMutation() = default;

        [[nodiscard]] virtual arm::Program mutate(const arm::Program& program) = 0;
    };
}



#endif //MEGUMIN_PROGRAMMUTATION_H

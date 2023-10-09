//
// Created by 58413 on 2023/10/9.
//

#ifndef MEGUMIN_SEARCHRESULT_H
#define MEGUMIN_SEARCHRESULT_H

#include <Program.h>
#include <bb/BasicBlock.h>

namespace megumin {
    class SearchResult {
    public:
        SearchResult(const arm::Program& target_program, const arm::Program& rewrite_program, int start, int end);

        arm::Program target_program;
        arm::Program rewrite_program;
        int bb_start_line;
        int bb_end_line;
    };
}

#endif //MEGUMIN_SEARCHRESULT_H

//
// Created by 58413 on 2023/10/9.
//

#include "SearchResult.h"

namespace megumin {
    SearchResult::SearchResult(const arm::Program &target_program, const arm::Program &rewrite_program, int start,
                               int end)
            : target_program(target_program), rewrite_program(rewrite_program), bb_start_line(start), bb_end_line(end) {

    }
}

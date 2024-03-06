//
// Created by 58413 on 2023/6/3.
//

#include "MutationResult.h"

namespace megumin {
    MutationResult MutationResult::failed_result() {
        MutationResult result;
        result.success = false;
        return result;
    }
}

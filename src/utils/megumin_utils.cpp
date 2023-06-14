//
// Created by 58413 on 2023/6/2.
//

#include "megumin_utils.h"
#include <iostream>

namespace megumin {
    void megumin_assert(bool expression, std::optional<std::string> message) {
        if (!expression) {
            if (message.has_value()) {
                std::cerr << "assertion failed: " << message.value() << std::endl;
            }
        }
    }

    void megumin_todo() {
        megumin_assert(false, "todo");
    }

    string trim(const string& s) {
        const char* WhiteSpace = " \t\v\r\n";
        std::size_t start = s.find_first_not_of(WhiteSpace);
        std::size_t end = s.find_last_not_of(WhiteSpace);
        return start == end ? std::string() : s.substr(start, end - start + 1);
    }
}

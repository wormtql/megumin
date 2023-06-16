//
// Created by wormtql on 2023/6/13.
//

#ifndef MEGUMIN_BBEXTRACTOR_H
#define MEGUMIN_BBEXTRACTOR_H

#include <string>
#include <vector>

#include "BasicBlock.h"

using namespace std;

namespace arm {
    class BBExtractor {
    private:
        std::string filename;
        int max_bb = 10;

        static string extract_opcode(const string& s);
        static bool is_load_store(const string& s);
        static bool is_branch(const string& s);
        static bool is_other_bb_break(const string& s);
        static string remove_comment(const string& s);
    public:
        explicit BBExtractor(const string& filename) {
            this->filename = filename;
        }

        std::vector<BasicBlock> extract_basic_blocks();
        inline void set_max_bb(int m) {
            max_bb = m;
        }
    };
}



#endif //MEGUMIN_BBEXTRACTOR_H

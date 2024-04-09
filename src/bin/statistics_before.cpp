//
// Created by 58413 on 2024/4/3.
//

#include <argparse/argparse.hpp>

#include <fstream>
#include <iostream>
#include <vector>
#include <map>

#include <bb/BBExtractor.h>
#include <megumin_utils.h>
#include <visitor/CanHandleThisInst.h>

using namespace std;
using namespace arm;
using namespace megumin;

bool can_process(const BasicBlock& bb) {
    CanHandleThisInst can;
    for (const auto& inst: bb.get_instructions()) {
        can.result = false;
        can.visit_instruction(inst);

        if (!can.result) {
            return false;
        }
    }
    return true;
}

int main(int argc, char* argv[]) {
    argparse::ArgumentParser program{"megumin statistics"};
    program.add_argument("--file");

    program.parse_args(argc, argv);

    auto input_filename = program.get<string>("--file");
    ifstream input_file{input_filename};

    BBExtractor bb_extractor{input_filename};
    bb_extractor.set_max_bb(-1);
    auto bbs = bb_extractor.extract_basic_blocks();

    vector<BasicBlock> viable_bbs;
    map<int, int> length_to_count;
    for (const auto& bb: bbs) {
        if (can_process(bb)) {
            viable_bbs.push_back(bb);
            length_to_count[bb.size()]++;
        }
    }

    int total_length = 0;
    for (const auto& bb: viable_bbs) {
        total_length += bb.size();
    }
    double average_bb_size = static_cast<double>(total_length) / viable_bbs.size();

    cout << "viable bbs: " << viable_bbs.size() << endl;
    cout << "average bb size: " << average_bb_size << endl;
    for (const auto entry: length_to_count) {
        cout << entry.first << ": " << entry.second << endl;
    }

    return 0;
}
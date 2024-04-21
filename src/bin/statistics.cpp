#include <argparse/argparse.hpp>

#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <map>

#include <megumin_utils.h>

using namespace std;

struct OptItem {
    vector<string> target_program;
    vector<string> rewrite_program;
    int start, end;
};

vector<OptItem> extract_opt_item(istream& is) {
    vector<OptItem> opt_items;
    string line;
    while (std::getline(is, line)) {
        line = megumin::trim(line);

        if (line == "[optimization success]") {
            std::getline(is, line);
            line = megumin::trim(line);
            int start, end;
            sscanf(line.c_str(), "%d, %d", &start, &end);

            vector<string> target_program;
            std::getline(is, line);
            line = megumin::trim(line);
            while (line != ">>>") {
                target_program.push_back(line);

                std::getline(is, line);
                line = megumin::trim(line);
            }

            vector<string> rewrite_program;
            std::getline(is, line);
            line = megumin::trim(line);
            while (line != "") {
                rewrite_program.push_back(line);

                std::getline(is, line);
                line = megumin::trim(line);
            }

            OptItem opt_item = OptItem {
                    .target_program = std::move(target_program),
                    .rewrite_program = std::move(rewrite_program),
                    .start = start,
                    .end = end
            };

            opt_items.emplace_back(std::move(opt_item));
        }
    }
    return std::move(opt_items);
}

vector<OptItem>::size_type get_vbb_instruction_count(const vector<OptItem>& items) {
    vector<OptItem>::size_type result = 0;
    for (const auto& item: items) {
        result += item.target_program.size();
    }
    return result;
}

int get_nop_count(const vector<OptItem>& items) {
    int result = 0;
    for (const auto& item: items) {
        for (const auto& inst: item.rewrite_program) {
            if (inst == "nop") {
                result++;
            }
        }
    }
    return result;
}

map<int, int> get_basic_block_length_map(const vector<OptItem>& items) {
    map<int, int> result;
    for (const auto& item: items) {
        result[item.target_program.size()]++;
    }
    return std::move(result);
}

int main(int argc, char* argv[]) {
    argparse::ArgumentParser program{"megumin optimization statistics"};
    program.add_argument("--file");

    program.parse_args(argc, argv);

    string input_filename = program.get<string>("--file");
    ifstream input_file{input_filename};
    if (!input_file.is_open()) {
        cerr << "cannot open " + input_filename << endl;
        return 0;
    }

    vector<OptItem> opt_items = extract_opt_item(input_file);
    cout << "optimization count: " << opt_items.size() << endl;
    int nop_count = get_nop_count(opt_items);
    cout << "nop count: " << nop_count << endl;
    int vbb_instruction_count = get_vbb_instruction_count(opt_items);
    cout << "vbb instruction count: " << vbb_instruction_count << endl;

    map<int, int> bb_length_map = get_basic_block_length_map(opt_items);
    cout << "length count:" << endl;
    for (const auto& item: bb_length_map) {
        cout << "    Length of " << item.first << ": " << "count is " << item.second << endl;
    }
    return 0;
}

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <argparse/argparse.hpp>

#include <megumin_utils.h>

using namespace std;

struct OptItem {
    vector<string> target_program;
    vector<string> rewrite_program;
    int start, end;
};

void apply_opts(vector<string>& assembly, const vector<OptItem>& opt_items) {
    for (const OptItem& opt_item: opt_items) {
        for (int i = opt_item.start, j = 0; i < opt_item.end; i++, j++) {
            assembly[i] = "\t" + opt_item.rewrite_program[j] + "\n";
        }
    }

    // remove nop
    auto it = assembly.begin();
    while (it != assembly.end()) {
        string op = megumin::trim(*it);
        if (op == "nop") {
            it = assembly.erase(it);
        } else {
            it++;
        }
    }
}

int main(int argc, char* argv[]) {
    argparse::ArgumentParser program{"megumin apply optimizations"};
    program.add_argument("--optimizations");
    program.add_argument("--source-file");
    program.add_argument("--output-file");

    program.parse_args(argc, argv);

    ifstream optimization_file{program.get<std::string>("--optimizations")};
    ifstream source_file{program.get<string>("--source-file")};

    if (!optimization_file.is_open()) {
        cout << "cannot open file " << program.get<string>("--optimizations") << endl;
        return 0;
    }

    if (!source_file.is_open()) {
        cout << "cannot open file " << program.get<string>("--source-file") << endl;
    }

    string output_filename;
    if (program.is_used("--output-file")) {
        output_filename = program.get<string>("--output-file");
    } else {
        string source_filename = program.get<string>("--source-file");
        auto dot_position = source_filename.find_last_of('.');
        if (dot_position == string::npos) {
            output_filename = source_filename + "_opt";
        } else {
            string name1 = source_filename.substr(0, dot_position);
            string extension = source_filename.substr(dot_position + 1);
            output_filename = name1 + "_opt." + extension;
        }
    }

    ofstream output_file{output_filename};
    if (!output_file.is_open()) {
        cout << "cannot open file " << output_filename << endl;
    }

    vector<OptItem> opt_items;
    string line;
    while (std::getline(optimization_file, line)) {
        line = megumin::trim(line);

        if (line == "[optimization success]") {
            std::getline(optimization_file, line);
            line = megumin::trim(line);
            int start, end;
            sscanf(line.c_str(), "%d, %d", &start, &end);

            vector<string> target_program;
            std::getline(optimization_file, line);
            line = megumin::trim(line);
            while (line != ">>>") {
                target_program.push_back(line);

                std::getline(optimization_file, line);
                line = megumin::trim(line);
            }

            vector<string> rewrite_program;
            std::getline(optimization_file, line);
            line = megumin::trim(line);
            while (line != "") {
                rewrite_program.push_back(line);

                std::getline(optimization_file, line);
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

    vector<string> source_asm;
    while (std::getline(source_file, line)) {
        source_asm.push_back(line + "\n");
    }

    apply_opts(source_asm, opt_items);
    
    for (int i = 0; i < source_asm.size(); i++) {
        output_file << source_asm[i];
    }

    return 0;
}

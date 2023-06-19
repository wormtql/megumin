#include <iostream>
#include <bitset>
#include <map>

#include <MachineState.h>
#include <Bitvec.h>
#include <Instruction.h>
#include <bb/BasicBlock.h>
#include <bb/BBExtractor.h>

using namespace std;
using namespace arm;

int main() {
    BBExtractor extractor{R"(E:\CLionProjects\megumin\test_files\pocketfft-aarch64.s)"};
    extractor.set_max_bb(100);
    auto bbs = extractor.extract_basic_blocks();

    map<int, int> counts;
    for (const auto& bb: bbs) {
        int size = bb.size();
        counts[size]++;
        if (size == 10) {
            cout << bb << endl;
        }
    }

    for (const auto& p: counts) {
        cout << p.first << ": " << p.second << endl;
    }


    return 0;
}

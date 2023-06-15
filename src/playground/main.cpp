#include <iostream>
#include <bitset>

#include <MachineState.h>
#include <Bitvec.h>
#include <Instruction.h>
#include <bb/BasicBlock.h>
#include <bb/BBExtractor.h>

using namespace arm;

int main() {
    BBExtractor extractor{R"(E:\CLionProjects\megumin\test_files\pocketfft-aarch64.s)"};
    extractor.extract_basic_blocks();

    return 0;
}

//
// Created by 58413 on 2023/11/22.
//

#include <megumin_utils.h>
#include <Program.h>
#include <verify/BruteForceVerifier.h>
#include <verify/SymbolicVerifier.h>
#include <filesystem>
#include <iostream>

using namespace std;

int main() {
    const string source_program_code = megumin::read_file_to_string(R"(E:\CLionProjects\megumin\src\bin\target.s)");
    const string rewrite_program_code = megumin::read_file_to_string(R"(E:\CLionProjects\megumin\src\bin\rewrite.s)");

    arm::Program source_program = megumin::aarch64_asm(source_program_code).value();
    arm::Program rewrite_program = megumin::aarch64_asm(rewrite_program_code).value();

    arm::MachineState state;
    state.set_gp(64, 0, arm::bits::from_u64(0x179a378850d59a45), false);
    state.set_gp(64, 1, arm::bits::from_u64(0xfffff7ffd7a8), false);
    state.set_gp(64, 4, arm::bits::from_u64(0xfffff7ffc3a4), false);
    state.set_gp(64, 6, arm::bits::from_u64(0xfffff7ffb000), false);
    state.set_gp(64, 7, arm::bits::from_u64(0x3b142a5c), false);
    state.set_gp(64, 9, arm::bits::from_u64(0x1), false);
    state.set_gp(64, 10, arm::bits::from_u64(0x655f1fc3), false);
    state.set_gp(64, 11, arm::bits::from_u64(0x3577bc45), false);
    state.set_gp(64, 12, arm::bits::from_u64(0x18), false);
    state.set_gp(64, 13, arm::bits::from_u64(0x655f1fc3), false);
    state.set_gp(64, 14, arm::bits::from_u64(0x3526f556a3d399), false);
    state.set_gp(64, 15, arm::bits::from_u64(0x50c6ef218f48), false);
    state.set_gp(64, 16, arm::bits::from_u64(0xfffff7c0c6f8), false);
    state.set_gp(64, 17, arm::bits::from_u64(0xfffff7fc93e0), false);
    state.set_gp(64, 18, arm::bits::from_u64(0x846), false);
    state.set_gp(64, 19, arm::bits::from_u64(0xaaaaaaaaaf58), false);
    state.set_gp(64, 21, arm::bits::from_u64(0xaaaaaaaaa9f4), false);
    state.set_gp(64, 29, arm::bits::from_u64(0xfffffffff340), false);
    state.set_gp(64, 30, arm::bits::from_u64(0xaaaaaaaaab38), false);
    state.set_sp(64, arm::bits::from_u64(0xfffffffff2c0));

    source_program.execute(state);


    megumin::BruteForceVerifier brute_force_verifier{1000};
    auto result = brute_force_verifier.verify(source_program, rewrite_program);

    cout << result.success << endl;

    megumin::SymbolicVerifier sym_verifier{};
    result = sym_verifier.verify(source_program, rewrite_program);

    cout << result.success << endl;
}
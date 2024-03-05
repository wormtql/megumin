//
// Created by 58413 on 2023/6/15.
//

#include "megumin_utils.h"
#include <iostream>
#include <optional>

#ifdef MEGUMIN_IS_WINDOWS
#include <Windows.h>
#include <keystone/keystone.h>
#endif

using namespace std;

namespace megumin {
#ifdef MEGUMIN_IS_WINDOWS
    std::optional<std::vector<arm::Instruction>> aarch64_asm_basic_block(const std::string& code) {
        HMODULE hDll = LoadLibrary(TEXT("keystone"));
        if (!hDll || hDll == INVALID_HANDLE_VALUE) {
            cout << "cannot load dll\n";
        }

        auto f = GetProcAddress(hDll, "ks_open");

        using KS_OPEN_TYPE = decltype(&ks_open);
        using KS_ASM_TYPE = decltype(&ks_asm);
        using KS_FREE_TYPE = decltype(&ks_free);
        using KS_CLOSE_TYPE = decltype(&ks_close);
        using KS_ERRNO_TYPE = decltype(&ks_errno);

        auto megumin_ks_open = (KS_OPEN_TYPE) f;
        auto megumin_ks_asm = (KS_ASM_TYPE) GetProcAddress(hDll, "ks_asm");
        auto megumin_ks_free = (KS_FREE_TYPE) GetProcAddress(hDll, "ks_free");
        auto megumin_ks_close = (KS_CLOSE_TYPE) GetProcAddress(hDll, "ks_close");
        auto megumin_ks_errno = (KS_ERRNO_TYPE) GetProcAddress(hDll, "ks_errno");

        ks_engine *ks;
        ks_err err;
        size_t count;
        unsigned char *encode;
        size_t size;

        err = megumin_ks_open(KS_ARCH_ARM64, KS_MODE_LITTLE_ENDIAN, &ks);
        if (err != KS_ERR_OK) {
            assert(false);
            return {};
        }

        if (megumin_ks_asm(ks, code.c_str(), 0, &encode, &size, &count) != KS_ERR_OK) {
            megumin_ks_free(encode);
            megumin_ks_close(ks);
            return {};
        }

        std::vector<arm::Instruction> result;
        for (int i = 0; i < count; i++) {
            unsigned char* e = encode + 4 * i;
            arm::Instruction instr{(void*)e};
            result.push_back(instr);
        }

        megumin_ks_free(encode);
        megumin_ks_close(ks);

        return result;
    }

    std::optional<arm::Program> aarch64_asm(const std::string& code) {
        std::vector<arm::Instruction> instructions = aarch64_asm_basic_block(code).value();

        arm::Program program{1};
        for (const auto& inst: instructions) {
            program.add_instruction(0, inst);
        }

        return program;
//
//        arm::Program result;
//
//        return result;
    }
#endif
}
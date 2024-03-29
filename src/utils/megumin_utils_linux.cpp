//
// Created by 58413 on 2023/6/15.
//

#include "megumin_utils.h"
#include <iostream>

#ifndef MEGUMIN_IS_WINDOWS
#include <keystone/keystone.h>
#endif

namespace megumin {
#ifndef MEGUMIN_IS_WINDOWS
    std::optional<arm::Program> aarch64_asm(const string& code) {
        ks_engine *ks;
        ks_err err;
        size_t count;
        unsigned char *encode;
        size_t size;

        err = ks_open(KS_ARCH_ARM64, KS_MODE_LITTLE_ENDIAN, &ks);
        if (err != KS_ERR_OK) {
            assert(false);
        }

        if (ks_asm(ks, code.c_str(), 0, &encode, &size, &count) != KS_ERR_OK) {
            // printf("ERROR: ks_asm() failed & count = %lu, error = %u\n",
            //        count, ks_errno(ks));
            ks_free(encode);
            ks_close(ks);
            // assert(false);
            return {};
        }

        arm::Program program;

        for (int i = 0; i < count; i++) {
            unsigned char* e = encode + 4 * i;
            arm::Instruction instr{(void*)e};
            program.add_instruction(instr);
        }
        for (int i = 0; i < size; i++) {
            // printf("%02x ", encode[i]);
        }
        // printf("\n");
        // printf("Compiled: %lu bytes, statements: %lu\n", size, count);

        ks_free(encode);
        ks_close(ks);

        return program;
    }
#endif
}
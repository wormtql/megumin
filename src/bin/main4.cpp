#include <iostream>

#include "verify/BruteForceVerifier.h"
#include "verify/SymbolicVerifier.h"
#include "megumin_utils.h"

using namespace std;

int main() {
//    megumin::BruteForceVerifier verifier{10000};
    megumin::SymbolicVerifier verifier;

    auto p1 = megumin::aarch64_asm(R"(add x12, x10, x25
add x13, x24, x25
add x14, x9, x12
subs x31, x0, x13
csinc w13, w31, w31, cs
subs x31, x24, x14
csinc w15, w31, w31, cs
sub x12, x12, x8
and w15, w13, w15
sub x13, x10, x8
add x12, x9, x12
add x13, x9, x13
subs x31, x0, x12
mov x11, x31
csinc w12, w31, w31, cs
subs x31, x13, x14
csinc w13, w31, w31, cs)").value();
    auto p2 = megumin::aarch64_asm(R"(sdiv x11, x0, x10
add x12, x10, x25
adds x14, x9, x12
csinc w13, w31, w25, hi
csinc w15, w10, w31, cc
clz x12, x9)").value();
//    auto p2 = megumin::aarch64_asm(R"(sdiv x11, x0, x10)").value();

    auto result = verifier.verify(p1, p2);
    cout << result << endl;

    return 0;
}
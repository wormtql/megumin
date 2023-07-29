#include <iostream>

#include "verify/BruteForceVerifier.h"
#include "verify/SymbolicVerifier.h"
#include "megumin_utils.h"

using namespace std;

int main() {
//    megumin::BruteForceVerifier verifier{10000};
    megumin::SymbolicVerifier verifier;

    auto p1 = megumin::aarch64_asm(R"(sdiv w9, w20, w8
subs w31, w9, #2
sdiv w8, w21, w10
add w10, w25, w9
sbfm w11, w8, #1, #31
udiv w10, w10, w9)").value();
    auto p2 = megumin::aarch64_asm(R"(lslv x11, x31, x31
sbcs w8, w31, w31
csinc w8, w20, w31, vs
and w9, w8, w8
extr w10, w1, w25, #1)").value();

    auto result = verifier.verify(p1, p2);
    cout << result << endl;

    return 0;
}
#include <iostream>

#include "verify/BruteForceVerifier.h"
#include "verify/SymbolicVerifier.h"
#include "megumin_utils.h"

using namespace std;

int main() {
//    megumin::BruteForceVerifier verifier{10000};
    megumin::SymbolicVerifier verifier;

    auto p1 = megumin::aarch64_asm(R"(add x8, sp, #656
add x19, x8, #1060
movz w8, #2
ands w31, w9, #1
lslv w2, w8, w10
csinv w3, w11, w31, ne
mov x0, x23
mov x1, x19)").value();
    auto p2 = megumin::aarch64_asm(R"(csneg x0, x23, x31, mi
add x19, sp, #1716
cls x2, x19
csel x1, x19, x31, mi
adcs w3, w11, w31
cls x8, x10)").value();

    auto result = verifier.verify(p1, p2);
    cout << result << endl;

    return 0;
}
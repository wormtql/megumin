#include <iostream>

#include "verify/BruteForceVerifier.h"
#include "megumin_utils.h"

using namespace std;

int main() {
    megumin::BruteForceVerifier verifier{10000};

    auto p1 = megumin::aarch64_asm(R"(subs x31, x20, x8
mov x8, x11
add x8, x8, x11
subs x31, x8, x10
csel x9, x10, x31, hi
sub x8, x8, x9
add x9, x8, x11
subs x31, x9, x10
csel x10, x10, x31, hi
sub x9, x9, x10)").value();
    auto p2 = megumin::aarch64_asm(R"(adds x8, x11, x11
add x9, x8, x11
subs x10, x9, x10
csneg x10, x31, x31, vs)").value();

    auto result = verifier.verify(p1, p2);
    cout << result << endl;

    return 0;
}
#include <iostream>
#include <string>
#include <cassert>
#include "megumin_utils.h"

using namespace std;

int main() {
    auto program = megumin::aarch64_asm("add x1, x2, x3");
    program.print();
}
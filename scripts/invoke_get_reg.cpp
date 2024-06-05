#include <iostream>

using namespace std;

uint64_t get_x0() {
    __asm__("ret");
}

uint64_t get_x1() {
    __asm__("mov x0, x1; ret");
}

uint64_t get_x2() {
    __asm__("mov x0, x2; ret");
}

uint64_t get_x3() {
    __asm__("mov x0, x3; ret");
}

uint64_t get_x4() {
    __asm__("mov x0, x4; ret");
}

uint64_t get_x5() {
    __asm__("mov x0, x5; ret");
}

uint64_t get_x6() {
    __asm__("mov x0, x6; ret");
}

uint64_t get_x7() {
    __asm__("mov x0, x7; ret");
}

uint64_t get_x8() {
    __asm__("mov x0, x8; ret");
}

uint64_t get_x9() {
    __asm__("mov x0, x9; ret");
}

uint64_t get_x10() {
    __asm__("mov x0, x10; ret");
}

uint64_t get_x11() {
    __asm__("mov x0, x11; ret");
}

uint64_t get_x12() {
    __asm__("mov x0, x12; ret");
}

uint64_t get_x13() {
    __asm__("mov x0, x13; ret");
}

uint64_t get_x14() {
    __asm__("mov x0, x14; ret");
}

uint64_t get_x15() {
    __asm__("mov x0, x15; ret");
}

uint64_t get_x16() {
    __asm__("mov x0, x16; ret");
}

uint64_t get_x17() {
    __asm__("mov x0, x17; ret");
}

uint64_t get_x18() {
    __asm__("mov x0, x18; ret");
}

uint64_t get_x19() {
    __asm__("mov x0, x19; ret");
}

uint64_t get_x20() {
    __asm__("mov x0, x20; ret");
}

uint64_t get_x21() {
    __asm__("mov x0, x21; ret");
}

uint64_t get_x22() {
    __asm__("mov x0, x22; ret");
}

uint64_t get_x23() {
    __asm__("mov x0, x23; ret");
}

uint64_t get_x24() {
    __asm__("mov x0, x24; ret");
}

uint64_t get_x25() {
    __asm__("mov x0, x25; ret");
}

uint64_t get_x26() {
    __asm__("mov x0, x26; ret");
}

uint64_t get_x27() {
    __asm__("mov x0, x27; ret");
}

uint64_t get_x28() {
    __asm__("mov x0, x28; ret");
}

uint64_t get_x29() {
    __asm__("mov x0, x29; ret");
}

uint64_t get_x30() {
    __asm__("mov x0, x30; ret");
}

void get_regs(uint64_t* ptr) {
    ptr[0] = get_x0();
    ptr[1] = get_x1();
    ptr[2] = get_x2();
    ptr[3] = get_x3();
    ptr[4] = get_x4();
    ptr[5] = get_x5();
    ptr[6] = get_x6();
    ptr[7] = get_x7();
    ptr[8] = get_x8();
    ptr[9] = get_x9();
    ptr[10] = get_x10();
    ptr[11] = get_x11();
    ptr[12] = get_x12();
    ptr[13] = get_x13();
    ptr[14] = get_x14();
    ptr[15] = get_x15();
    ptr[16] = get_x16();
    ptr[17] = get_x17();
    ptr[18] = get_x18();
    ptr[19] = get_x19();
    ptr[20] = get_x20();
    ptr[21] = get_x21();
    ptr[22] = get_x22();
    ptr[23] = get_x23();
    ptr[24] = get_x24();
    ptr[25] = get_x25();
    ptr[26] = get_x26();
    ptr[27] = get_x27();
    ptr[28] = get_x28();
    ptr[29] = get_x29();
    ptr[30] = get_x30();
}

int main() {
    uint64_t regs[32] = {0};
    get_regs(regs);
    for (int i = 0; i <= 30; i++) {
        cout << regs[i] << endl;
    }
    // cout << get_x29() << endl;
}

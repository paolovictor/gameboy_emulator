#include <assert.h>
#include "z80.h"

#define TEST(name) { printf("Test '%s'...", #name); name(); printf(" Passed!\n"); } 

void test_ld_hla() {
    Z80 z80;

    z80.a = 0x56;
    z80.h = 0x12;
    z80.l = 0x34;

    LDD_HLA(&z80);

    assert(z80.mem[0x1234] == 0x56);
    assert(z80.h == 0x12);
    assert(z80.l == 0x33);
}

void test_inc_hl() {
    Z80 z80;

    z80.h = 0x00;
    z80.l = 0x00;

    INC_HL(&z80);

    assert(z80.h == 0x00);
    assert(z80.l == 0x01);

    z80.h = 0x00;
    z80.l = 0xFF;

    INC_HL(&z80);

    assert(z80.h == 0x01);
    assert(z80.l == 0x00);
    assert(z80.f & FLAG_HALF_CARRY);

    z80.h = 0xFF;
    z80.l = 0xFF;

    INC_HL(&z80);

    assert(z80.h == 0x00);
    assert(z80.l == 0x00);
    assert(z80.f & FLAG_CARRY);
}

int main(int argc, char* argv[]) {
    TEST(test_ld_hla);
    TEST(test_inc_hl);

    return 0;
}

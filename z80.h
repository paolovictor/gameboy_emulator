#include <stdint.h>
#include <stdio.h>

#define FLAG_CARRY      0x10 // Last op overflowed?
#define FLAG_HALF_CARRY 0x20 // Last op lower byte overflowed 15?
#define FLAG_OPERATION  0x40 // Last op was a subtraction?
#define FLAG_ZERO       0x80 // Last op result was zero?

#define MERGE(a, b) ((a << 8) + b)

struct Z80 {
    uint16_t a, b, c, d, e, f, h, l;

    uint16_t sp, pc;

    uint16_t m, t;

    uint8_t mem[65536];
};

void init(Z80* z80) {
    z80->a = 0;
    z80->b = 0;
    z80->c = 0;
    z80->d = 0;
    z80->e = 0;
    z80->f = 0;
    z80->h = 0;
    z80->l = 0;
}

/**
 * Base memory functions
 */

uint8_t read_byte(Z80* z80, uint16_t position) {
    return z80->mem[position];
}

uint8_t fetch_byte(Z80* z80) {
    z80->pc += 1;
    return read_byte(z80, z80->pc);
}
   
/**
 * General-use load functions
 */

void LD(Z80* z80, uint16_t* r) {
    *r = fetch_byte(z80);
}

void LD(uint16_t* r1, uint16_t* r2) {
    *r1 = *r2;
}

void LD(Z80* z80, uint16_t* r1, uint16_t at) {
    *r1 = z80->mem[at];
}

void LD(Z80* z80, uint16_t at, uint16_t* r) {
    z80->mem[at] = *r;
}

void LD(Z80* z80, uint16_t at) {
    z80->mem[at] = fetch_byte(z80);
}

void DEC_HL(Z80* z80) {
    z80->l = (z80->l - 1) & 0xFF;
    z80->h = (z80->l == 0xFF) ? (z80->h - 1) & 0xFF: z80->h;
}

void INC_HL(Z80* z80) {
    z80->l = (z80->l + 1) & 0xFF;
    z80->h = ((z80->l == 0) ? (z80->h + 1) : z80->h) & 0xFF;
    
    if(z80->l == 0) z80->f |= FLAG_HALF_CARRY;
    if(z80->h == 0 && z80->l == 0) z80->f |= FLAG_CARRY;
}

/**
 * 8-bit loads
 */

void LD_A(Z80* z80) { LD(z80, &z80->a); };
void LD_B(Z80* z80) { LD(z80, &z80->b); };
void LD_C(Z80* z80) { LD(z80, &z80->c); };
void LD_D(Z80* z80) { LD(z80, &z80->d); };
void LD_E(Z80* z80) { LD(z80, &z80->e); };
void LD_H(Z80* z80) { LD(z80, &z80->h); };
void LD_L(Z80* z80) { LD(z80, &z80->l); };
void LD_HL(Z80* z80) { LD(z80, MERGE(z80->h, z80->l)); };
void LD_BC(Z80* z80) { LD(z80, MERGE(z80->b, z80->c)); };
void LD_DE(Z80* z80) { LD(z80, MERGE(z80->d, z80->e)); };

void LD_A_A(Z80* z80) { LD(&z80->a, &z80->a); };
void LD_A_B(Z80* z80) { LD(&z80->a, &z80->b); };
void LD_A_C(Z80* z80) { LD(&z80->a, &z80->c); };
void LD_A_D(Z80* z80) { LD(&z80->a, &z80->d); };
void LD_A_E(Z80* z80) { LD(&z80->a, &z80->e); };
void LD_A_H(Z80* z80) { LD(&z80->a, &z80->h); };
void LD_A_L(Z80* z80) { LD(&z80->a, &z80->l); };
void LD_A_HL(Z80* z80) { LD(z80, &z80->a, MERGE(z80->h, z80->l)); };
void LD_A_BC(Z80* z80) { LD(z80, &z80->a, MERGE(z80->b, z80->c)); };
void LD_A_DE(Z80* z80) { LD(z80, &z80->a, MERGE(z80->d, z80->e)); };
void LD_A_NN(Z80* z80) { LD(z80, &z80->a, MERGE(fetch_byte(z80), fetch_byte(z80))); };
void LD_A_rC(Z80* z80) { LD(z80, &z80->a, 0xFF00 + z80->c); };
void LD_rC_A(Z80* z80) { LD(z80, 0xFF00 + z80->c, &z80->a); };

void LD_B_A(Z80* z80) { LD(&z80->b, &z80->a); };
void LD_B_B(Z80* z80) { LD(&z80->b, &z80->b); };
void LD_B_C(Z80* z80) { LD(&z80->b, &z80->c); };
void LD_B_D(Z80* z80) { LD(&z80->b, &z80->d); };
void LD_B_E(Z80* z80) { LD(&z80->b, &z80->e); };
void LD_B_H(Z80* z80) { LD(&z80->b, &z80->h); };
void LD_B_L(Z80* z80) { LD(&z80->b, &z80->l); };
void LD_B_HL(Z80* z80) { LD(z80, &z80->b, MERGE(z80->h, z80->l)); };

void LD_C_A(Z80* z80) { LD(&z80->c, &z80->a); };
void LD_C_B(Z80* z80) { LD(&z80->c, &z80->b); };
void LD_C_C(Z80* z80) { LD(&z80->c, &z80->c); };
void LD_C_D(Z80* z80) { LD(&z80->c, &z80->d); };
void LD_C_E(Z80* z80) { LD(&z80->c, &z80->e); };
void LD_C_H(Z80* z80) { LD(&z80->c, &z80->h); };
void LD_C_L(Z80* z80) { LD(&z80->c, &z80->l); };
void LD_C_HL(Z80* z80) { LD(z80, &z80->c, MERGE(z80->h, z80->l)); };

void LD_D_A(Z80* z80) { LD(&z80->d, &z80->a); };
void LD_D_B(Z80* z80) { LD(&z80->d, &z80->b); };
void LD_D_C(Z80* z80) { LD(&z80->d, &z80->c); };
void LD_D_D(Z80* z80) { LD(&z80->d, &z80->d); };
void LD_D_E(Z80* z80) { LD(&z80->d, &z80->e); };
void LD_D_H(Z80* z80) { LD(&z80->d, &z80->h); };
void LD_D_L(Z80* z80) { LD(&z80->d, &z80->l); };
void LD_D_HL(Z80* z80) { LD(z80, &z80->d, MERGE(z80->h, z80->l)); };

void LD_E_A(Z80* z80) { LD(&z80->e, &z80->a); };
void LD_E_B(Z80* z80) { LD(&z80->e, &z80->b); };
void LD_E_C(Z80* z80) { LD(&z80->e, &z80->c); };
void LD_E_D(Z80* z80) { LD(&z80->e, &z80->d); };
void LD_E_E(Z80* z80) { LD(&z80->e, &z80->e); };
void LD_E_H(Z80* z80) { LD(&z80->e, &z80->h); };
void LD_E_L(Z80* z80) { LD(&z80->e, &z80->l); };
void LD_E_HL(Z80* z80) { LD(z80, &z80->e, MERGE(z80->h, z80->l)); };

void LD_L_A(Z80* z80) { LD(&z80->l, &z80->a); };
void LD_L_B(Z80* z80) { LD(&z80->l, &z80->b); };
void LD_L_C(Z80* z80) { LD(&z80->l, &z80->c); };
void LD_L_D(Z80* z80) { LD(&z80->l, &z80->d); };
void LD_L_E(Z80* z80) { LD(&z80->l, &z80->e); };
void LD_L_H(Z80* z80) { LD(&z80->l, &z80->h); };
void LD_L_L(Z80* z80) { LD(&z80->l, &z80->l); };
void LD_L_HL(Z80* z80) { LD(z80, &z80->l, MERGE(z80->h, z80->l)); };

void LD_H_A(Z80* z80) { LD(&z80->h, &z80->a); };
void LD_H_B(Z80* z80) { LD(&z80->h, &z80->b); };
void LD_H_C(Z80* z80) { LD(&z80->h, &z80->c); };
void LD_H_D(Z80* z80) { LD(&z80->h, &z80->d); };
void LD_H_E(Z80* z80) { LD(&z80->h, &z80->e); };
void LD_H_H(Z80* z80) { LD(&z80->h, &z80->h); };
void LD_H_L(Z80* z80) { LD(&z80->h, &z80->l); };
void LD_H_HL(Z80* z80) { LD(z80, &z80->h, MERGE(z80->h, z80->l)); };

void LD_HL_A(Z80* z80) { LD(z80, MERGE(z80->h, z80->l), &z80->a); };
void LD_HL_B(Z80* z80) { LD(z80, MERGE(z80->h, z80->l), &z80->b); };
void LD_HL_C(Z80* z80) { LD(z80, MERGE(z80->h, z80->l), &z80->c); };
void LD_HL_D(Z80* z80) { LD(z80, MERGE(z80->h, z80->l), &z80->d); };
void LD_HL_E(Z80* z80) { LD(z80, MERGE(z80->h, z80->l), &z80->e); };
void LD_HL_H(Z80* z80) { LD(z80, MERGE(z80->h, z80->l), &z80->h); };
void LD_HL_L(Z80* z80) { LD(z80, MERGE(z80->h, z80->l), &z80->l); };

void LDD_AHL(Z80* z80) { LD_A_HL(z80); DEC_HL(z80); }
void LDD_HLA(Z80* z80) { LD_HL_A(z80); DEC_HL(z80); }
void LDI_AHL(Z80* z80) { LD_A_HL(z80); INC_HL(z80); }
void LDI_HLA(Z80* z80) { LD_HL_A(z80); DEC_HL(z80); }

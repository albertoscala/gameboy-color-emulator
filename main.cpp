#include <iostream>
using namespace std;

typedef unsigned char uint8_t;

// Defining 8-bits registers
typedef struct registers {
    uint8_t a;
    uint8_t b;
    uint8_t c;
    uint8_t d;
    uint8_t e;
    uint8_t f;
    uint8_t h;
    uint8_t l;
};

// Defining virtual 16-bits registers

void generic_set(uint8_t *high, uint8_t *low, unsigned short value) {
    // Zeroing LS 8-bits and shift it back then cast it
    *high = (uint8_t) ((value & 0xFF00) >> 8);
    // printf("high: %b\n", *high);

    // Not sure how useful is this since I already truncate with cast the 8 MS bits
    *low = (uint8_t) (value & 0xFF);
    // printf("low: %b\n", *low);
}

unsigned short generic_get(uint8_t high, uint8_t low) {
    // Get the first part, shift to make space and OR it with lower part
    return (((unsigned short) high) << 8 | ((unsigned short) low));
}

// AF

void set_af(registers *r, unsigned short value) {
    generic_set(&r->a, &r->f, value);
}

unsigned short get_af(registers *r) {
    return generic_get(r->a, r->f);
}

// BC

void set_bc(registers *r, unsigned short value) {
    generic_set(&r->b, &r->c, value);
}

unsigned short get_bc(registers *r) {
    return generic_get(r->b, r->c);
}

// DE

void set_de(registers *r, unsigned short value) {
    generic_set(&r->d, &r->e, value);
}

unsigned short get_de(registers *r) {
    return generic_get(r->d, r->e);
}

// HL

void set_hl(registers *r, unsigned short value) {
    generic_set(&r->h, &r->l, value);
}

unsigned short get_hl(registers *r) {
    return generic_get(r->h, r->l);
}

int main() {
    
    registers *r = (registers *) malloc(sizeof(registers));
    
    short prova = 1922;

    printf("%b\n", prova);

    set_bc(r, prova);

    printf("%b %b\n", r->b, r->c);

    short ret = get_bc(r);

    cout << (ret == prova) << endl;
    
    return 0;
}

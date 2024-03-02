#include <iostream>
using namespace std;

typedef unsigned char uint8_t;

// Defining 8-bits registers

enum Register {
    A = 0,
    B = 1,
    C = 2,
    D = 3,
    E = 4,
    F = 5,
    H = 6,
    L = 7
};

typedef struct Registers {
    uint8_t register_t[8];
};

// Defining flag register
const uint8_t ZERO_FLAG_BYTE_POSITION       = 7;
const uint8_t SUBTRACT_FLAG_BYTE_POSITION   = 6;
const uint8_t HALF_CARRY_FLAG_BYTE_POSITION = 5;
const uint8_t CARRY_FLAG_BYTE_POSITION      = 4;

typedef struct FlagRegister {
    bool zero;
    bool subtract;
    bool half_carry;
    bool carry;
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

void set_af(Registers *r, unsigned short value) {
    generic_set(&r->register_t[A], &r->register_t[F], value);
}

unsigned short get_af(Registers *r) {
    return generic_get(r->register_t[A], r->register_t[F]);
}

// BC

void set_bc(Registers *r, unsigned short value) {
    generic_set(&r->register_t[B], &r->register_t[C], value);
}

unsigned short get_bc(Registers *r) {
    return generic_get(r->register_t[B], r->register_t[C]);
}

// DE

void set_de(Registers *r, unsigned short value) {
    generic_set(&r->register_t[D], &r->register_t[E], value);
}

unsigned short get_de(Registers *r) {
    return generic_get(r->register_t[D], r->register_t[E]);
}

// HL

void set_hl(Registers *r, unsigned short value) {
    generic_set(&r->register_t[H], &r->register_t[L], value);
}

unsigned short get_hl(Registers *r) {
    return generic_get(r->register_t[H], r->register_t[L]);
}

// 8-bit number flag register

void set_flag_register(FlagRegister *fr, uint8_t value) {
    fr->zero = ((value >> ZERO_FLAG_BYTE_POSITION) & 0b1) != 0;
    fr->subtract = ((value >> SUBTRACT_FLAG_BYTE_POSITION) & 0b1) != 0;
    fr->half_carry = ((value >> HALF_CARRY_FLAG_BYTE_POSITION) & 0b1) != 0;
    fr->carry = ((value >> CARRY_FLAG_BYTE_POSITION) & 0b1) != 0;

    cout << fr->zero << "\t" << fr->subtract << "\t" << fr->half_carry << "\t" << fr->carry << endl;
}

uint8_t get_flag_register(FlagRegister *fr) {
    return (
        (fr->zero ? 1 : 0) << ZERO_FLAG_BYTE_POSITION | \
        (fr->subtract ? 1 : 0) << SUBTRACT_FLAG_BYTE_POSITION | \
        (fr->half_carry ? 1 : 0) << HALF_CARRY_FLAG_BYTE_POSITION | \
        (fr->carry ? 1 : 0) << CARRY_FLAG_BYTE_POSITION
    );
}

// Instruction set

enum OPCode {
    ADD,
    ADDHL,
    ADC,
    SUB,
    SBC,
    AND,
    OR,
    XOR,
    CP,
    INC,
    DEC,
    CCF,
    SCF,
    RRA,
    RLA,
    RRCA,
    RRLA,
    CPL,
    BIT,
    RESET,
    SET,
    SRL,
    RR,
    RL,
    RRC,
    RLC,
    SRA,
    SLA,
    SWAP
};

typedef struct Instruction {
    OPCode opc;
    Register arithmetic_target;
};

uint8_t exec_ADD(Registers *r, uint8_t value) {
    //TODO: Implement overflow addition
    uint8_t result = r->register_t[A] + value;    

    return result; 
}

void execute_instruction(Registers *r, Instruction *i) {
    switch (i->opc) {
        case ADD: {
            uint8_t value = r->register_t[i->arithmetic_target];
            uint8_t result = exec_ADD(r, value);
            r->register_t[A] = result;
            break;
        }
            
        default:
            break;
    }
}

int main() {
    
    // Test high low decomp recomp val

    Registers *r = (Registers *) malloc(sizeof(Registers));
    
    short prova = 1922;

    printf("%b\n", prova);

    set_bc(r, prova);

    printf("%b %b\n", r->register_t[B], r->register_t[C]);

    short ret = get_bc(r);

    cout << (ret == prova) << endl;
    
    // Test flag reg right value

    uint8_t test = 0b10110000;

    printf("test: %b\n", test);

    FlagRegister *fr = (FlagRegister *) malloc(sizeof(FlagRegister));

    set_flag_register(fr, test);

    uint8_t test2 = get_flag_register(fr);

    printf("test: %b\n", test2);

    return 0;
}

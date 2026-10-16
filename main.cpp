#include <cstdint>
#include <iostream>  
#include <format>
#include <string.h>
#include "chip-8.h"

u_int8_t vram[64 * 32];
u_int8_t memory[3840] = {0};
u_int16_t PC = 0;
u_int16_t opcode;

void read_opcode(){
    switch (opcode){
        case CLS:
            memset(vram, 0, 64*32);
        default:
            break;
    }
}



int main(){
    std::cout << std::format("0x{:04X}\n", CLS);
    return 0;
}
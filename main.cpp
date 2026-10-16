#include <cstdint>
#include <iostream>  
#include <format>
#include <string.h>
#include "chip-8.h"



typedef struct{
    u_int8_t memory[4096] = {0};
    u_int8_t vram[256];
    u_int16_t PC = 0;
    u_int16_t opcode; 
    u_int16_t SP;
    u_int16_t stack[16];
} State;


State state = {0, 0, 0, 0};
void read_opcode(){
    if (state.PC + 1 >= 4096) {
        return; // or handle invalid memory read
    }
    state.opcode = (state.memory[state.PC] << 8) | state.memory[state.PC + 1];
    state.PC += 2;
    switch (state.opcode){
        case CLS:
            memset(state.vram, 0, 64*32);
            break;
        case RET:
            state.SP--;
            state.PC = state.SP;
            break;
        case 0x8000:
            switch (state.opcode & 0x000F){
                case 0x0:
                    break;
                case 0x1:
                    break;
                case 0x2:
                    break;
                case 0x3:
                    break;
                case 0x4:
                    break;
                case 0x5:
                    break;
                case 0x6:
                    break;
                case 0x7:
                    break;
            }
        default:
            break;
    }
}

void load_rom(){

}

int main(){
    std::cout << std::format("0x{:04X}\n", CLS);
    return 0;
}
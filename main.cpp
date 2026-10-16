#include <cstdint>
#include <iostream>  
#include <format>
#include <string.h>
#include "chip-8.h"



struct State{
    u_int8_t memory[4096] = {0};
    u_int8_t vram[256];
    u_int8_t V[16] = {0};
    u_int16_t PC = 0;
    u_int16_t opcode; 
    u_int16_t SP;
    u_int16_t stack[16];
};


State state = {0, 0, 0, 0};
void read_opcode(){
    if (state.PC + 1 >= 4096) {
        return; 
    }

    state.opcode = (state.memory[state.PC] << 8) | state.memory[state.PC + 1];
    state.PC += 2;

    uint8_t x = (state.opcode & 0x0F00) >> 8;
    uint8_t y = (state.opcode & 0x00F0) >> 4;
    uint8_t nn = state.opcode & 0x00FF;
    uint16_t nnn = state.opcode & 0x0FFF;

    switch (state.opcode & 0xF000){
        case 0x0000:
            if (state.opcode == CLS) {
                memset(state.vram, 0, 64*32);
            } else if (state.opcode == RET) { 
                state.SP--;
                state.PC = state.stack[state.SP];
            }
            break;
        case 0x1000: 
            state.PC = nnn;
            break;
        case 0x2000: 
            state.stack[state.SP] = state.PC;
            state.SP++;
            state.PC = nnn;
            break;
        case 0x3000: 
            if (state.V[x] == nn){
                state.PC += 2;
            }
            break;
        case 0x4000: 
            if (state.V[x] != nn){
                state.PC += 2;
            }
            break;
        case 0x5000: 
            if (state.V[x] == state.V[y]){
                state.PC += 2;
            }
            break;
        case 0x6000: 
            state.V[x] = nn;
            break;
        case 0x7000: 
            state.V[x] = state.V[x] + nn;
            break;
        case 0x8000: 
            switch (state.opcode & 0x000F){
                case 0x0: 
                    state.V[x] = state.V[y]; 
                    break;
                case 0x1:
                    state.V[x] = state.V[x] | state.V[y];
                    break;
                case 0x2:
                    state.V[x] = state.V[x] & state.V[y];
                    break;
                case 0x3:
                    state.V[x] = state.V[x] ^ state.V[y];
                    break;
                case 0x4:
                    state.V[0xF] = state.V[x] + state.V[y];
                    state.V[x] = state.V[0xF] & state.V[x];
                    break;
                case 0x5:
                    break;
                case 0x6:
                    break;
                case 0x7:
                    break;
            }
            break;
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
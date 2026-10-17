#include <cstdint>
#include <iostream>  
#include <format>
#include <string.h>
#include "chip-8.h"
#include "raylib.h"


struct State{
    u_int8_t memory[4096] = {0};
    u_int8_t vram[256] = {0};
    u_int8_t V[16] = {0};
    u_int16_t I = 0;
    u_int16_t PC = 0;
    u_int16_t opcode = 0;
    u_int16_t SP = 0;
    u_int16_t stack[16] = {0};
    bool keys[16] = {0};
    u_int8_t delay_timer = 0;
    u_int8_t sound_timer = 0;
};

State state{};
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
                memset(state.vram, 0, sizeof(state.vram));
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
                    {
                        uint16_t sum = static_cast<uint16_t>(state.V[x]) + static_cast<uint16_t>(state.V[y]);
                        state.V[0xF] = (sum > 0xFF) ? 1 : 0;
                        state.V[x] = static_cast<uint8_t>(sum);
                    }
                    break;
                case 0x5:
                    {
                        state.V[0xF] = (state.V[x] >= state.V[y]) ? 1 : 0;
                        state.V[x] = static_cast<uint8_t>(state.V[x] - state.V[y]);
                    }
                    break;
                case 0x6:
                    state.V[0xF] = state.V[x] & 0x1;
                    state.V[x] >>= 1;
                    break;
                case 0x7:
                    state.V[0xF] = (state.V[y] >= state.V[x]) ? 1 : 0;
                    state.V[x] = static_cast<uint8_t>(state.V[y] - state.V[x]);
                    break;
                case 0xE:
                    state.V[0xF] = (state.V[x] >> 7) & 0x1;
                    state.V[x] <<= 1;
                    break;
            }
            break;
        case 0x9000:
            if (state.V[x] != state.V[y]) {
                state.PC += 2;
            }
            break;
        case 0xA000:
            state.I = nnn;
            break;
        case 0xB000:
            state.PC = static_cast<uint16_t>(state.V[0] + nnn);
            break;
        case 0xC000:
            state.V[x] = static_cast<uint8_t>((std::rand() % 256) & nn);
            break;
        case 0xD000:
            {
                uint8_t height = state.opcode & 0x000F;
                state.V[0xF] = 0;
                for (uint8_t row = 0; row < height; ++row) {
                    uint8_t sprite = state.memory[state.I + row];
                    for (uint8_t bit = 0; bit < 8; ++bit) {
                        if ((sprite & (0x80 >> bit)) != 0) {
                            // draw logic goes here later
                        }
                    }
                }
            }
            break;
        case 0xE000:
            switch (state.opcode & 0x00FF) {
                case 0x9E:
                    if (state.keys[state.V[x]]) {
                        state.PC += 2;
                    }
                    break;
                case 0xA1:
                    if (!state.keys[state.V[x]]) {
                        state.PC += 2;
                    }
                    break;
            }
            break;
        case 0xF000:
            switch (state.opcode & 0x00FF) {
                case 0x07:
                    state.V[x] = state.delay_timer;
                    break;
                case 0x0A:
                    // wait for key press - placeholder until input loop exists
                    break;
                case 0x15:
                    state.delay_timer = state.V[x];
                    break;
                case 0x18:
                    state.sound_timer = state.V[x];
                    break;
                case 0x1E:
                    state.I += state.V[x];
                    break;
                case 0x29:
                    state.I = static_cast<uint16_t>(state.V[x] * 5);
                    break;
                case 0x33:
                    state.memory[state.I] = state.V[x] / 100;
                    state.memory[state.I + 1] = (state.V[x] / 10) % 10;
                    state.memory[state.I + 2] = state.V[x] % 10;
                    break;
                case 0x55:
                    for (uint8_t i = 0; i <= x; ++i) {
                        state.memory[state.I + i] = state.V[i];
                    }
                    break;
                case 0x65:
                    for (uint8_t i = 0; i <= x; ++i) {
                        state.V[i] = state.memory[state.I + i];
                    }
                    break;
            }
            break;
        default:
            break;
    }
}
void load_rom(){

}

void input_loop(){
    state.keys[0] = IsKeyDown(KEY_ONE);
    state.keys[1] = IsKeyDown(KEY_TWO);
    state.keys[2] = IsKeyDown(KEY_THREE);
    state.keys[3] = IsKeyDown(KEY_FOUR);

    state.keys[4] = IsKeyDown(KEY_Q);
    state.keys[5] = IsKeyDown(KEY_W);
    state.keys[6] = IsKeyDown(KEY_E);
    state.keys[7] = IsKeyDown(KEY_R);

    state.keys[8] = IsKeyDown(KEY_A);
    state.keys[9] = IsKeyDown(KEY_S);
    state.keys[10] = IsKeyDown(KEY_D);
    state.keys[11] = IsKeyDown(KEY_F);

    state.keys[12] = IsKeyDown(KEY_Z);
    state.keys[13] = IsKeyDown(KEY_X);
    state.keys[14] = IsKeyDown(KEY_C);
    state.keys[15] = IsKeyDown(KEY_V);
}
int main(){
    std::cout << std::format("0x{:04X}\n", CLS);
    return 0;
}
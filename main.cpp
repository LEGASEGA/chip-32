#include <cstdint>
#include <iostream>  
#include <format>
#include "chip-8.h"

u_int8_t memory[4096] = {0};
u_int16_t PC = 0;


/*switch decode_opcode(){
    case 
}*/



int main(){
    std::cout << std::format("0x{:04X}\n", CLS);
}
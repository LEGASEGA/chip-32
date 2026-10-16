CXX = g++
CXXFLAGS = -Wall -std=c++20

all: chip8_emulator


chip8_emulator: main.o
	$(CXX) $(CXXFLAGS) -o chip32_emulator main.o

main.o: main.cpp chip-8.h
	$(CXX) $(CXXFLAGS) -c main.cpp

.PHONY: clean
clean:
	rm -f *.o chip8_emulator

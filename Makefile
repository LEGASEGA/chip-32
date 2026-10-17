CXX = /usr/bin/clang++
CXXFLAGS = -std=c++20 -stdlib=libc++ -Wall -Wextra -g
SRC = main.cpp
TARGET = chip32_emulator

# Homebrew paths and macOS frameworks from tasks.json
INCLUDES = -I/Users/ljem/.homebrew/include
LDFLAGS = -L/Users/ljem/.homebrew/lib -lraylib -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) $(INCLUDES) $(LDFLAGS) -o $(TARGET)

clean:
	rm -f $(TARGET)
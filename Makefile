# Portable CHIP-32 build.
# Requires a C++20 compiler, pkg-config, and Raylib development files.
#
# Examples:
#   make
#   make clean
#   make CXX=clang++
#   make PKG_CONFIG=/path/to/pkg-config

CXX = c++
PKG_CONFIG ?= pkg-config

CXXFLAGS ?= -std=c++20 -Wall -Wextra -g
CPPFLAGS ?=
LDFLAGS ?=

SRC = main.cpp
TARGET = chip32_emulator

RAYLIB_CFLAGS := $(shell $(PKG_CONFIG) --cflags raylib 2>/dev/null)
RAYLIB_LIBS := $(shell $(PKG_CONFIG) --libs raylib 2>/dev/null)

.PHONY: all clean check-deps

all: $(TARGET)

check-deps:
	@command -v $(PKG_CONFIG) >/dev/null 2>&1 || { \
		echo "Error: pkg-config was not found. Install pkg-config and Raylib."; \
		exit 1; \
	}
	@$(PKG_CONFIG) --exists raylib 2>/dev/null || { \
		echo "Error: Raylib was not found by pkg-config."; \
		echo "Install Raylib development files, or configure PKG_CONFIG so it can find raylib.pc."; \
		exit 1; \
	}

$(TARGET): $(SRC) | check-deps
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(RAYLIB_CFLAGS) $(SRC) $(LDFLAGS) $(RAYLIB_LIBS) -o $@

clean:
	rm -f $(TARGET)

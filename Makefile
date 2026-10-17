# Portable CHIP-32 build: supports the author's local Raylib install and
# standard pkg-config based installations on macOS/Linux.

CXX = $(shell command -v clang++ 2>/dev/null || command -v c++)
PKG_CONFIG ?= pkg-config

CXXFLAGS ?= -std=c++20 -Wall -Wextra -g
CPPFLAGS ?=
LDFLAGS ?=

SRC = main.cpp
TARGET = chip32_emulator

# First try the normal pkg-config setup used by most package managers.
RAYLIB_CFLAGS := $(shell $(PKG_CONFIG) --cflags raylib 2>/dev/null)
RAYLIB_LIBS := $(shell $(PKG_CONFIG) --libs raylib 2>/dev/null)

# Backwards-compatible fallback for the author's original custom Homebrew
# prefix (typically ~/.homebrew), if raylib.pc isn't discoverable.
LOCAL_RAYLIB_PREFIX ?= $(HOME)/.homebrew
ifeq ($(strip $(RAYLIB_LIBS)),)
ifneq ($(wildcard $(LOCAL_RAYLIB_PREFIX)/include/raylib.h),)
RAYLIB_CFLAGS := -I$(LOCAL_RAYLIB_PREFIX)/include
RAYLIB_LIBS := -L$(LOCAL_RAYLIB_PREFIX)/lib -lraylib
endif
endif

# Raylib on macOS may need these system frameworks when not provided by
# pkg-config. They are not passed to Linux linkers.
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
ifeq ($(strip $(shell $(PKG_CONFIG) --libs raylib 2>/dev/null)),)
RAYLIB_LIBS += -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
endif
endif

.PHONY: all clean check-deps

all: $(TARGET)

check-deps:
	@command -v $(CXX) >/dev/null 2>&1 || { \
		echo "Error: no C++ compiler found (install clang++ or g++)."; \
		exit 1; \
	}
	@$(PKG_CONFIG) --exists raylib 2>/dev/null || test -f "$(LOCAL_RAYLIB_PREFIX)/include/raylib.h" || { \
		echo "Error: Raylib was not found."; \
		echo "Install Raylib with your package manager, or set LOCAL_RAYLIB_PREFIX to its install prefix."; \
		exit 1; \
	}

$(TARGET): $(SRC) | check-deps
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(RAYLIB_CFLAGS) $(SRC) $(LDFLAGS) $(RAYLIB_LIBS) -o $@

clean:
	rm -f $(TARGET)

# Compiler
CC = gcc

# Include and library paths
CFLAGS = -Iinclude
LDFLAGS = -lSDL2

# Source files and output
SRC = src/main.c src/display.c src/cpu.c
OUT_DIR = build
OUT = $(OUT_DIR)/chip8

# Default target
all: $(OUT)

$(OUT): $(SRC)
	@mkdir -p $(OUT_DIR)
	$(CC) $(SRC) -o $(OUT) $(CFLAGS) $(LDFLAGS)

# Clean build
clean:
	rm -rf $(OUT_DIR)
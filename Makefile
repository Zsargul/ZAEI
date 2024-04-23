# Compiler
CC := gcc

# Source directories
SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin
SUB_DIRS := util

# Final binary
TARGET := $(BIN_DIR)/zaei

# Flags (Libraries and includes blank for now)
CFLAGS := -g -Wall -O2

# ----------------------------
# Compile and link into binary
# ----------------------------
SOURCES := $(shell find $(SRC_DIR) -name "*.c")
OBJECTS := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SOURCES))

# Default target
all: $(TARGET)

# Create directories and compiling/linking rules
$(shell mkdir -p $(OBJ_DIR) $(BIN_DIR))
$(shell mkdir -p $(addprefix $(OBJ_DIR)/, $(SUB_DIRS)))

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean
clean:
	rm -rf $(OBJ_DIR)/* $(BIN_DIR)/*

.PHONY: all clean

# Makefile for Dynamic Memory Management Visualizer
# Ubuntu/Linux version

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -g -Iinclude -DLOG_COLORS
LDFLAGS = -lm

# Target executable
TARGET = memory_visualizer

# Directories
SRC_DIR = src
BUILD_DIR = build

# Source files
SRCS = $(SRC_DIR)/main.c \
       $(SRC_DIR)/core/memory_manager.c \
       $(SRC_DIR)/core/paging.c \
       $(SRC_DIR)/core/segmentation.c \
       $(SRC_DIR)/core/virtual_memory.c \
       $(SRC_DIR)/algorithms/fifo.c \
       $(SRC_DIR)/algorithms/lru.c \
       $(SRC_DIR)/algorithms/optimal.c \
       $(SRC_DIR)/visualization/console_gui.c \
       $(SRC_DIR)/visualization/memory_display.c \
       $(SRC_DIR)/utils/helpers.c \
       $(SRC_DIR)/utils/logger.c

# Object files
OBJS = $(SRCS:%.c=$(BUILD_DIR)/%.o)

# Header files
HEADERS = $(wildcard include/*.h) \
          $(wildcard $(SRC_DIR)/*/*.h)

# Default target
all: $(BUILD_DIR) $(TARGET)

# Create build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)/core
	mkdir -p $(BUILD_DIR)/algorithms
	mkdir -p $(BUILD_DIR)/visualization
	mkdir -p $(BUILD_DIR)/utils

# Link object files to create executable
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)
	@echo "✅ Build successful! Run ./$(TARGET) to start"

# Compile source files to object files
$(BUILD_DIR)/%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build files
clean:
	rm -rf $(BUILD_DIR) $(TARGET) *.log
	@echo "✅ Cleaned build files"

# Run the program
run: $(TARGET)
	./$(TARGET)

# Debug mode
debug: CFLAGS += -DDEBUG -O0
debug: clean $(TARGET)

# Release mode
release: CFLAGS += -O2 -DNDEBUG
release: clean $(TARGET)

# Build and run
all-run: all run

# Create tags for code navigation
tags:
	ctags -R .

# Run valgrind for memory leak detection
valgrind: $(TARGET)
	valgrind --leak-check=full --show-leak-kinds=all ./$(TARGET)

# Run gdb debugger
gdb: $(TARGET)
	gdb ./$(TARGET)

# Install dependencies (Ubuntu)
install-deps:
	sudo apt update
	sudo apt install -y gcc make valgrind gdb tree

# Create project structure
init:
	mkdir -p src/{core,algorithms,visualization,utils}
	mkdir -p include data docs tests scripts
	@echo "✅ Project structure created"

# Help
help:
	@echo "Available targets:"
	@echo "  make all       - Build the program (default)"
	@echo "  make clean     - Remove build files"
	@echo "  make run       - Build and run the program"
	@echo "  make debug     - Build with debug flags"
	@echo "  make release   - Build with optimization"
	@echo "  make valgrind  - Run with memory leak detection"
	@echo "  make gdb       - Run with debugger"
	@echo "  make install-deps - Install dependencies"
	@echo "  make init      - Create project structure"
	@echo "  make help      - Show this help"

# Phony targets
.PHONY: all clean run debug release valgrind gdb install-deps init help

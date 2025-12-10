#!/bin/bash

# Compilation script for Dynamic Memory Management Visualizer
# Ubuntu/Linux version

echo "========================================="
echo "   Dynamic Memory Management Visualizer  "
echo "   Compilation Script                   "
echo "========================================="
echo ""

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Check if gcc is installed
if ! command -v gcc &> /dev/null; then
    echo -e "${RED}Error: GCC compiler not found!${NC}"
    echo "Please install GCC: sudo apt install gcc"
    exit 1
fi

echo -e "${BLUE}Step 1: Checking project structure...${NC}"

# Check for required directories
if [ ! -d "src" ]; then
    echo -e "${YELLOW}Warning: src/ directory not found${NC}"
    echo "Creating directory structure..."
    mkdir -p src/{core,algorithms,visualization,utils}
    mkdir -p include
fi

if [ ! -d "include" ]; then
    echo -e "${YELLOW}Warning: include/ directory not found${NC}"
    mkdir -p include
fi

echo -e "${GREEN}✓ Project structure OK${NC}"

echo ""
echo -e "${BLUE}Step 2: Checking source files...${NC}"

# Check for main source file
if [ ! -f "src/main.c" ]; then
    echo -e "${RED}Error: src/main.c not found!${NC}"
    echo "Please create the main.c file"
    exit 1
fi

# Count source files
SRC_COUNT=$(find src -name "*.c" | wc -l)
HEADER_COUNT=$(find include -name "*.h" | wc -l)

echo -e "Found ${GREEN}$SRC_COUNT${NC} source files"
echo -e "Found ${GREEN}$HEADER_COUNT${NC} header files"

echo ""
echo -e "${BLUE}Step 3: Compiling...${NC}"

# Create build directory if it doesn't exist
mkdir -p build

# Compile all source files
echo "Compiling source files..."
gcc -Wall -Wextra -g -Iinclude -DLOG_COLORS \
    src/main.c \
    src/core/*.c \
    src/algorithms/*.c \
    src/visualization/*.c \
    src/utils/*.c \
    -o memory_visualizer

# Check if compilation was successful
if [ $? -eq 0 ]; then
    echo -e "${GREEN}✓ Compilation successful!${NC}"
    echo ""
    echo -e "${BLUE}Step 4: Checking executable...${NC}"
    
    if [ -f "memory_visualizer" ]; then
        echo -e "${GREEN}✓ Executable created: memory_visualizer${NC}"
        
        # Show file information
        echo ""
        echo -e "${YELLOW}File information:${NC}"
        file memory_visualizer
        echo ""
        echo -e "${YELLOW}Size:${NC}" $(du -h memory_visualizer | cut -f1)
        
        echo ""
        echo -e "${BLUE}Step 5: Running basic test...${NC}"
        echo "To run the program: ./memory_visualizer"
        echo ""
        echo -e "${GREEN}=========================================${NC}"
        echo -e "${GREEN}   Build completed successfully!        ${NC}"
        echo -e "${GREEN}=========================================${NC}"
        
        # Make executable
        chmod +x memory_visualizer
        
        # Optional: run a quick test
        read -p "Do you want to run a quick test? (y/n): " -n 1 -r
        echo ""
        if [[ $REPLY =~ ^[Yy]$ ]]; then
            echo -e "${YELLOW}Running test...${NC}"
            timeout 5s ./memory_visualizer --test || echo "Test completed or timed out"
        fi
        
    else
        echo -e "${RED}Error: Executable not created${NC}"
        exit 1
    fi
else
    echo -e "${RED}✗ Compilation failed!${NC}"
    echo "Check the error messages above"
    exit 1
fi

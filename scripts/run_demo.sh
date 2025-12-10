#!/bin/bash

# Demo script for Dynamic Memory Management Visualizer

echo "========================================="
echo "   MEMORY MANAGEMENT VISUALIZER DEMO    "
echo "========================================="
echo ""

# Colors
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

# Check if executable exists
if [ ! -f "memory_visualizer" ]; then
    echo -e "${YELLOW}Executable not found. Compiling first...${NC}"
    ./scripts/compile.sh
    if [ $? -ne 0 ]; then
        echo -e "${RED}Compilation failed. Cannot run demo.${NC}"
        exit 1
    fi
fi

echo -e "${GREEN}Starting demo...${NC}"
echo "This demo will show:"
echo "1. Memory system initialization"
echo "2. Process creation"
echo "3. Page allocation"
echo "4. Memory access with page faults"
echo "5. Statistics display"
echo ""
echo -e "${YELLOW}Press Enter to continue...${NC}"
read

# Create a demo input file
cat > demo_input.txt << 'EOF'
1
2
Browser
3
0
0
3
0
1
3
0
2
3
0
3
4
0
4096
8
10
EOF

echo -e "${BLUE}Running automated demo...${NC}"
echo "========================================="

# Run with demo input
timeout 30s ./memory_visualizer < demo_input.txt

echo ""
echo "========================================="
echo -e "${GREEN}Demo completed!${NC}"
echo ""
echo "To run the full interactive program:"
echo "  ./memory_visualizer"
echo ""
echo "Available options in the program:"
echo "  1. Initialize Memory System"
echo "  2. Create New Process"
echo "  3. Allocate Pages"
echo "  4. Access Memory (simulate page faults)"
echo "  5. Paging System Operations"
echo "  6. Segmentation System Operations"
echo "  7. Page Replacement Algorithms"
echo "  8. Display Statistics"
echo "  9. Run Complete Demo"
echo "  10. Exit"
echo ""
echo -e "${YELLOW}Press Enter to run interactive mode...${NC}"
read

# Run interactive mode
./memory_visualizer

# Cleanup
rm -f demo_input.txt

#!/bin/bash

echo "Compiling with corrected include paths..."

# Create list of all source files
FILES=""
for dir in src src/core src/algorithms src/visualization src/utils; do
    if [ -d "$dir" ]; then
        for file in "$dir"/*.c; do
            if [ -f "$file" ]; then
                FILES="$FILES $file"
                echo "Adding: $file"
            fi
        done
    fi
done

echo ""
echo "Compiling..."
gcc -Wall -Wextra -Iinclude -o memory_visualizer $FILES

if [ $? -eq 0 ]; then
    echo "✅ Compilation successful!"
    echo ""
    echo "To run: ./memory_visualizer"
    
    # Test run
    echo ""
    read -p "Do you want to test run? (y/n): " choice
    if [ "$choice" = "y" ] || [ "$choice" = "Y" ]; then
        timeout 10s ./memory_visualizer || echo "Test completed"
    fi
else
    echo "❌ Compilation failed!"
    echo "Remaining errors:"
    gcc -Wall -Wextra -Iinclude -o memory_visualizer $FILES 2>&1 | head -20
fi

#!/bin/bash

echo "Fixing compilation warnings..."

# 1. Add time.h to optimal.c
if grep -q "#include <time.h>" src/algorithms/optimal.c; then
    echo "time.h already included in optimal.c"
else
    # Add after the first #include line
    sed -i '1a #include <time.h>' src/algorithms/optimal.c
    echo "Added time.h to optimal.c"
fi

# 2. Add string.h to memory_display.c
if grep -q "#include <string.h>" src/visualization/memory_display.c; then
    echo "string.h already included in memory_display.c"
else
    sed -i '1a #include <string.h>' src/visualization/memory_display.c
    echo "Added string.h to memory_display.c"
fi

echo "✅ Warnings fixed!"

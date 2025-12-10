#!/bin/bash

echo "Fixing ALL include paths with correct relative paths..."

# 1. Fix main.c - Should include from src/core/
sed -i 's|#include "memory_manager.h"|#include "core/memory_manager.h"|g' src/main.c
sed -i 's|#include "../include/common_defs.h"|#include "include/common_defs.h"|g' src/main.c

# 2. Fix memory_manager.c - Should include from current directory
sed -i 's|#include "../../algorithms/fifo.h"|#include "../algorithms/fifo.h"|g' src/core/memory_manager.c
sed -i 's|#include "../../algorithms/lru.h"|#include "../algorithms/lru.h"|g' src/core/memory_manager.c
sed -i 's|#include "../../algorithms/optimal.h"|#include "../algorithms/optimal.h"|g' src/core/memory_manager.c

# 3. Fix paging.c, segmentation.c, virtual_memory.c
for file in src/core/paging.c src/core/segmentation.c src/core/virtual_memory.c; do
    sed -i 's|#include "memory_manager.h"|#include "memory_manager.h"|g' "$file"
done

# 4. Fix algorithm files - Include from src/core/
for file in src/algorithms/fifo.c src/algorithms/lru.c src/algorithms/optimal.c; do
    sed -i 's|#include "../../core/memory_manager.h"|#include "../core/memory_manager.h"|g' "$file"
done

# 5. Fix memory_display.c
sed -i 's|#include "../../core/memory_manager.h"|#include "../core/memory_manager.h"|g' src/visualization/memory_display.c

echo "✅ All include paths fixed!"

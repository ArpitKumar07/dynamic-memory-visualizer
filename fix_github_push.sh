#!/bin/bash

echo "=== FIXING GITHUB PUSH ERROR ==="
echo ""

cd ~/dynamic-memory-visualizer

echo "1. Checking current state..."
git status
echo ""

echo "2. Adding all files..."
git add .
echo ""

echo "3. Creating first commit..."
git commit -m "Initial commit: Memory Management Visualizer

A complete C project simulating:
- Paging memory management
- Segmentation system  
- Virtual memory with page replacement
- FIFO, LRU algorithms
- Interactive console interface
- Visual memory display"
echo ""

echo "4. Creating main branch..."
git branch -M main
echo ""

echo "5. Pushing to GitHub..."
git push -u origin main
echo ""

echo "✅ DONE! Check: https://github.com/ArpitKumar07/dynamic-memory-visualizer"

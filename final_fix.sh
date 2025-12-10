#!/bin/bash

echo "=== FINAL FIX FOR GITHUB PUSH ==="
cd ~/dynamic-memory-visualizer

echo ""
echo "1. Creating .gitignore..."
cat > .gitignore << 'GITIGNORE'
# Compiled files
*.exe
*.out
*.o
memory_visualizer

# Build directories
build/

# IDE files
.vscode/

# Log files
*.log

# System files
.DS_Store
GITIGNORE

echo ""
echo "2. Pulling from GitHub..."
git pull origin main --allow-unrelated-histories

echo ""
echo "3. Adding all files..."
git add --all

echo ""
echo "4. Committing..."
git commit -m "Memory Management Visualizer - Complete Project

Complete implementation of:
• Paging memory management system
• Segmentation with address translation
• Virtual memory simulation
• Page replacement algorithms (FIFO, LRU, Optimal)
• Interactive console interface
• Memory visualization
• Statistics tracking

Ready for academic submission."

echo ""
echo "5. Pushing to GitHub..."
git push origin main

echo ""
echo "✅ Project uploaded successfully!"
echo "Visit: https://github.com/ArpitKumar07/dynamic-memory-visualizer"

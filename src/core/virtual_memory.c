/**
 * virtual_memory.c
 * Virtual Memory implementation
 */

#include <stdio.h>
#include <stdlib.h>
#include "virtual_memory.h"
#include "../../include/common_defs.h"
#include "memory_manager.h"

// Swap space simulation
#define SWAP_SPACE_SIZE 100
typedef struct {
    int pid;
    int page_number;
    int in_swap;
} SwapEntry;

SwapEntry swap_space[SWAP_SPACE_SIZE];

// Initialize virtual memory
void init_virtual_memory(int virtual_pages, int physical_frames) {
    printf("\nInitializing Virtual Memory System...\n");
    printf("=====================================\n");
    printf("Virtual pages: %d\n", virtual_pages);
    printf("Physical frames: %d\n", physical_frames);
    printf("Swap space: %d pages\n", SWAP_SPACE_SIZE);
    
    // Initialize swap space
    for(int i = 0; i < SWAP_SPACE_SIZE; i++) {
        swap_space[i].pid = -1;
        swap_space[i].page_number = -1;
        swap_space[i].in_swap = 0;
    }
    
    printf("✓ Virtual memory system ready\n");
    printf("✓ Demand paging enabled\n");
    printf("✓ Swap space initialized\n");
}

// Simulate virtual memory access
void simulate_virtual_memory_access(int pid, int address) {
    printf("\nVirtual Memory Access Simulation:\n");
    printf("=================================\n");
    
    int page_number = address / PAGE_SIZE;
    
    printf("Process %d accessing address %d (Page %d)\n", 
           pid, address, page_number);
    
    // Check if page is in physical memory
    if(is_page_in_memory(pid, page_number)) {
        printf("✓ Page is in physical memory (Page Hit)\n");
        
        // Update access info
        int frame_id = processes[pid].page_table[page_number].frame_number;
        physical_memory[frame_id].last_access = current_time;
        physical_memory[frame_id].reference_bit = 1;
        
        printf("Accessed Frame: %d\n", frame_id);
    } else {
        printf("✗ Page is not in physical memory (Page Fault)\n");
        printf("Checking swap space...\n");
        
        // Check if page is in swap space
        int in_swap = 0;
        for(int i = 0; i < SWAP_SPACE_SIZE; i++) {
            if(swap_space[i].pid == pid && 
               swap_space[i].page_number == page_number &&
               swap_space[i].in_swap) {
                in_swap = 1;
                break;
            }
        }
        
        if(in_swap) {
            printf("Page found in swap space\n");
            printf("Swapping in page from disk...\n");
            swap_in(pid, page_number);
        } else {
            printf("Page not found anywhere (First access)\n");
            printf("Loading page from executable file...\n");
            
            // Allocate page
            allocate_page(pid, page_number);
        }
    }
}

// Display virtual memory state
void display_virtual_memory_state() {
    printf("\nVirtual Memory System State:\n");
    printf("============================\n");
    
    printf("Physical Memory (%d frames):\n", MAX_FRAMES);
    printf("Frame  Page  Process  Status\n");
    printf("-----  ----  -------  ------\n");
    
    int used_frames = 0;
    for(int i = 0; i < MAX_FRAMES; i++) {
        if(!physical_memory[i].is_free) {
            printf("%5d  %4d  %7d  LOADED\n",
                   i,
                   physical_memory[i].page_number,
                   physical_memory[i].process_id);
            used_frames++;
        } else {
            printf("%5d  %4s  %7s  FREE\n", i, "--", "--");
        }
    }
    
    printf("\nMemory Usage: %d/%d frames (%.1f%%)\n",
           used_frames, MAX_FRAMES,
           (used_frames * 100.0) / MAX_FRAMES);
    
    // Show swap space usage
    int swap_used = 0;
    for(int i = 0; i < SWAP_SPACE_SIZE; i++) {
        if(swap_space[i].in_swap) {
            swap_used++;
        }
    }
    
    printf("Swap Space: %d/%d pages used (%.1f%%)\n",
           swap_used, SWAP_SPACE_SIZE,
           (swap_used * 100.0) / SWAP_SPACE_SIZE);
}

// Swap in a page from disk
void swap_in(int pid, int page_number) {
    printf("\nSwap In Operation:\n");
    printf("------------------\n");
    
    // Find the page in swap space
    int swap_index = -1;
    for(int i = 0; i < SWAP_SPACE_SIZE; i++) {
        if(swap_space[i].pid == pid && 
           swap_space[i].page_number == page_number &&
           swap_space[i].in_swap) {
            swap_index = i;
            break;
        }
    }
    
    if(swap_index == -1) {
        printf("Error: Page not found in swap space\n");
        return;
    }
    
    printf("1. Locate page in swap space (slot %d)\n", swap_index);
    printf("2. Find free frame in physical memory\n");
    
    int frame_id = find_free_frame();
    
    if(frame_id == -1) {
        printf("3. No free frames - need to swap out a page\n");
        // In real implementation, select victim and swap out
        printf("4. Select victim page\n");
        printf("5. Swap out victim page\n");
        
        // For demo, we'll just allocate
        frame_id = 0;  // Use frame 0 for demo
    } else {
        printf("3. Free frame found: %d\n", frame_id);
    }
    
    printf("4. Read page from swap space into frame %d\n", frame_id);
    printf("5. Update page table\n");
    printf("6. Clear swap space entry\n");
    
    // Update physical memory
    physical_memory[frame_id].frame_id = frame_id;
    physical_memory[frame_id].page_number = page_number;
    physical_memory[frame_id].process_id = pid;
    physical_memory[frame_id].load_time = current_time;
    physical_memory[frame_id].last_access = current_time;
    physical_memory[frame_id].is_free = false;
    
    // Update page table
    processes[pid].page_table[page_number].frame_number = frame_id;
    processes[pid].page_table[page_number].present = true;
    processes[pid].page_table[page_number].valid = true;
    
    // Clear swap space
    swap_space[swap_index].in_swap = 0;
    
    printf("✓ Page %d swapped in successfully\n", page_number);
    current_time++;
}

// Swap out a page to disk
void swap_out(int pid, int page_number) {
    printf("\nSwap Out Operation:\n");
    printf("-------------------\n");
    
    // Find free slot in swap space
    int swap_index = -1;
    for(int i = 0; i < SWAP_SPACE_SIZE; i++) {
        if(!swap_space[i].in_swap) {
            swap_index = i;
            break;
        }
    }
    
    if(swap_index == -1) {
        printf("Error: No free space in swap area\n");
        return;
    }
    
    printf("1. Select victim page: Process %d, Page %d\n", pid, page_number);
    
    int frame_id = processes[pid].page_table[page_number].frame_number;
    
    printf("2. Check if page is dirty (modified)\n");
    
    if(physical_memory[frame_id].dirty) {
        printf("3. Page is dirty - write to swap space\n");
    } else {
        printf("3. Page is clean - no need to write\n");
    }
    
    printf("4. Free frame %d\n", frame_id);
    
    // Update swap space
    swap_space[swap_index].pid = pid;
    swap_space[swap_index].page_number = page_number;
    swap_space[swap_index].in_swap = 1;
    
    // Update physical memory
    physical_memory[frame_id].is_free = true;
    physical_memory[frame_id].page_number = -1;
    physical_memory[frame_id].process_id = -1;
    
    // Update page table
    processes[pid].page_table[page_number].present = false;
    processes[pid].page_table[page_number].frame_number = -1;
    
    printf("5. Update swap space (slot %d)\n", swap_index);
    printf("✓ Page %d swapped out successfully\n", page_number);
}

// Display swap space
void display_swap_space() {
    printf("\nSwap Space Contents:\n");
    printf("====================\n");
    printf("Slot  Process  Page  Status\n");
    printf("----  -------  ----  ------\n");
    
    int used_slots = 0;
    for(int i = 0; i < SWAP_SPACE_SIZE; i++) {
        if(swap_space[i].in_swap) {
            printf("%4d  %7d  %4d  USED\n",
                   i,
                   swap_space[i].pid,
                   swap_space[i].page_number);
            used_slots++;
        }
    }
    
    if(used_slots == 0) {
        printf("No pages in swap space\n");
    }
    
    printf("\nTotal: %d/%d slots used\n", used_slots, SWAP_SPACE_SIZE);
}

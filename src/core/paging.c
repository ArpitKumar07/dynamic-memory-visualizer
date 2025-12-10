/**
 * paging.c
 * Paging system implementation
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "paging.h"
#include "../../include/common_defs.h"
#include "memory_manager.h"

// Initialize paging system
void init_paging_system(int total_pages, int page_size) {
    printf("\nInitializing Paging System...\n");
    printf("=============================\n");
    printf("Total pages: %d\n", total_pages);
    printf("Page size: %d bytes\n", page_size);
    printf("Virtual address space: %d KB\n", 
           (total_pages * page_size) / 1024);
    printf("Page table size: %d entries\n", total_pages);
    printf("✓ Paging system ready\n");
}

// Translate paging address
void translate_paging_address(int pid, int logical_addr) {
    if(pid < 0 || pid >= MAX_PROCESSES || processes[pid].pid == -1) {
        printf("Error: Invalid process ID %d\n", pid);
        return;
    }
    
    int page_number = logical_addr / PAGE_SIZE;
    int offset = logical_addr % PAGE_SIZE;
    
    printf("\nAddress Translation (Paging):\n");
    printf("-----------------------------\n");
    printf("Logical Address: %d\n", logical_addr);
    printf("Page Number: %d\n", page_number);
    printf("Offset: %d\n", offset);
    
    if(page_number >= MAX_PAGES) {
        printf("Error: Page number %d exceeds maximum (%d)\n", 
               page_number, MAX_PAGES - 1);
        return;
    }
    
    if(processes[pid].page_table[page_number].present) {
        int frame_number = processes[pid].page_table[page_number].frame_number;
        int physical_addr = (frame_number * PAGE_SIZE) + offset;
        
        printf("✓ Page is in memory\n");
        printf("Frame Number: %d\n", frame_number);
        printf("Physical Address: %d\n", physical_addr);
        printf("Translation successful!\n");
    } else {
        printf("✗ PAGE FAULT: Page %d not in memory\n", page_number);
        printf("Page needs to be loaded from disk\n");
        
        // Simulate page fault handling
        simulate_paging_fault(pid, page_number);
    }
}

// Display page table
void display_page_table(int pid) {
    if(pid < 0 || pid >= MAX_PROCESSES || processes[pid].pid == -1) {
        printf("Error: Invalid process ID %d\n", pid);
        return;
    }
    
    printf("\nPage Table for Process %d (%s):\n", 
           pid, processes[pid].name);
    printf("========================================\n");
    printf("Page  Frame  Valid  Present  Referenced  Modified\n");
    printf("----  -----  -----  -------  ----------  --------\n");
    
    for(int i = 0; i < 20; i++) {  // Show first 20 entries
        printf("%4d  %5d  %5s  %7s  %10s  %8s\n",
               i,
               processes[pid].page_table[i].frame_number,
               processes[pid].page_table[i].valid ? "Yes" : "No",
               processes[pid].page_table[i].present ? "Yes" : "No",
               processes[pid].page_table[i].referenced ? "Yes" : "No",
               processes[pid].page_table[i].modified ? "Yes" : "No");
    }
    
    // Count present pages
    int present_count = 0;
    for(int i = 0; i < MAX_PAGES; i++) {
        if(processes[pid].page_table[i].present) {
            present_count++;
        }
    }
    
    printf("\nSummary: %d/%d pages in memory (%d%%)\n",
           present_count, MAX_PAGES,
           (present_count * 100) / MAX_PAGES);
}

// Simulate page fault
void simulate_paging_fault(int pid, int page_number) {
    printf("\nSimulating Page Fault Handling...\n");
    printf("---------------------------------\n");
    
    printf("1. Trap to operating system\n");
    printf("2. Check page table - Page %d not present\n", page_number);
    printf("3. Find free frame in physical memory\n");
    
    int frame_id = find_free_frame();
    
    if(frame_id == -1) {
        printf("4. No free frames - Need page replacement\n");
        printf("5. Select victim page using replacement algorithm\n");
        
        // In real implementation, this would call the algorithm
        printf("6. Write victim page to disk if modified\n");
        printf("7. Read requested page from disk\n");
    } else {
        printf("4. Free frame found: Frame %d\n", frame_id);
        printf("5. Read page %d from disk into frame %d\n", 
               page_number, frame_id);
    }
    
    printf("6. Update page table entry\n");
    printf("7. Update frame table\n");
    printf("8. Restart the instruction\n");
    
    printf("\n✓ Page fault handling simulated\n");
    
    // Actually allocate the page
    allocate_page(pid, page_number);
}

// Handle page fault (actual implementation)
void handle_page_fault(int pid, int page_number) {
    printf("Handling page fault for Page %d (Process %d)\n", 
           page_number, pid);
    
    // Try to allocate the page
    if(allocate_page(pid, page_number) == 0) {
        printf("Page fault resolved successfully\n");
    } else {
        printf("Failed to resolve page fault\n");
    }
}

// Generate reference string for testing
int* generate_reference_string(int length) {
    int* ref_string = (int*)malloc(length * sizeof(int));
    if(ref_string == NULL) {
        printf("Error: Memory allocation failed\n");
        return NULL;
    }
    
    srand(time(NULL));
    
    // Generate references with some locality
    int current = rand() % MAX_PAGES;
    for(int i = 0; i < length; i++) {
        // 70% chance to stay close to current page
        if(rand() % 100 < 70) {
            current = (current + (rand() % 5) - 2) % MAX_PAGES;
            if(current < 0) current = 0;
        } else {
            current = rand() % MAX_PAGES;
        }
        ref_string[i] = current;
    }
    
    return ref_string;
}

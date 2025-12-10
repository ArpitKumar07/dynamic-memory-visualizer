/**
 * memory_manager.c
 * Memory Manager implementation
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "memory_manager.h"
#include "../../include/common_defs.h"
#include "../algorithms/fifo.h"
#include "../algorithms/lru.h"
#include "../algorithms/optimal.h"

// Global variables (defined in main.c, declared here as extern)
extern MemoryFrame physical_memory[MAX_FRAMES];
extern Process processes[MAX_PROCESSES];
extern Statistics stats;
extern int current_time;
extern int algorithm_type;

// Initialize memory system
void init_memory_system() {
    printf("\nInitializing Memory System...\n");
    printf("==============================\n");
    
    // Initialize physical memory frames
    for(int i = 0; i < MAX_FRAMES; i++) {
        physical_memory[i].frame_id = i;
        physical_memory[i].page_number = -1;
        physical_memory[i].process_id = -1;
        physical_memory[i].load_time = -1;
        physical_memory[i].last_access = -1;
        physical_memory[i].reference_bit = 0;
        physical_memory[i].dirty = false;
        physical_memory[i].is_free = true;
    }
    
    // Initialize processes
    for(int i = 0; i < MAX_PROCESSES; i++) {
        processes[i].pid = -1;
        processes[i].name[0] = '\0';
        processes[i].page_faults = 0;
        processes[i].memory_usage = 0;
        
        // Initialize page table
        for(int j = 0; j < MAX_PAGES; j++) {
            processes[i].page_table[j].page_number = j;
            processes[i].page_table[j].frame_number = -1;
            processes[i].page_table[j].valid = false;
            processes[i].page_table[j].present = false;
            processes[i].page_table[j].referenced = false;
            processes[i].page_table[j].modified = false;
            processes[i].page_table[j].protection = 0;
        }
        
        // Initialize segment table
        for(int j = 0; j < MAX_SEGMENTS; j++) {
            processes[i].segment_table[j].segment_id = j;
            processes[i].segment_table[j].base_address = -1;
            processes[i].segment_table[j].limit = 0;
            processes[i].segment_table[j].valid = false;
            processes[i].segment_table[j].protection = 0;
        }
    }
    
    // Initialize statistics
    reset_statistics();
    
    current_time = 0;
    
    printf("✓ Physical Memory: %d frames (%d KB total)\n", 
           MAX_FRAMES, (MAX_FRAMES * PAGE_SIZE) / 1024);
    printf("✓ Virtual Memory: %d pages\n", MAX_PAGES);
    printf("✓ Processes array initialized\n");
    printf("✓ Statistics reset\n");
}

// Reset simulation
void reset_simulation() {
    init_memory_system();
    printf("Simulation reset to initial state.\n");
}

// Initialize a process
void init_process(int pid, const char* name) {
    if(pid < 0 || pid >= MAX_PROCESSES) {
        printf("Error: Invalid process ID %d\n", pid);
        return;
    }
    
    processes[pid].pid = pid;
    strncpy(processes[pid].name, name, 49);
    processes[pid].name[49] = '\0';
    processes[pid].page_faults = 0;
    processes[pid].memory_usage = 0;
    processes[pid].page_count = 0;
    
    printf("Process %d '%s' initialized.\n", pid, name);
}

// Allocate a page to a process
int allocate_page(int pid, int page_number) {
    if(pid < 0 || pid >= MAX_PROCESSES || processes[pid].pid == -1) {
        printf("Error: Invalid or non-existent process %d\n", pid);
        return -1;
    }
    
    if(page_number < 0 || page_number >= MAX_PAGES) {
        printf("Error: Invalid page number %d\n", page_number);
        return -1;
    }
    
    // Check if page is already allocated
    if(processes[pid].page_table[page_number].present) {
        printf("Page %d is already allocated to process %d\n", page_number, pid);
        return 0;
    }
    
    // Find a free frame
    int frame_id = find_free_frame();
    if(frame_id == -1) {
        printf("No free frames available! Need page replacement.\n");
        
        // Call page replacement algorithm
        int replaced_frame;
        switch(algorithm_type) {
            case 0: // FIFO
                replaced_frame = fifo_replace_page();
                break;
            case 1: // LRU
                replaced_frame = lru_replace_page();
                break;
            case 2: // Optimal
                // For demo, we'll use FIFO as default
                replaced_frame = fifo_replace_page();
                break;
            default:
                replaced_frame = fifo_replace_page();
        }
        
        // Free the replaced frame
        int old_pid = physical_memory[replaced_frame].process_id;
        int old_page = physical_memory[replaced_frame].page_number;
        
        if(old_pid != -1 && old_page != -1) {
            processes[old_pid].page_table[old_page].present = false;
            processes[old_pid].page_table[old_page].frame_number = -1;
            processes[old_pid].memory_usage -= PAGE_SIZE / 1024;
        }
        
        frame_id = replaced_frame;
        printf("Frame %d freed by replacement algorithm\n", frame_id);
    }
    
    // Allocate the frame
    physical_memory[frame_id].page_number = page_number;
    physical_memory[frame_id].process_id = pid;
    physical_memory[frame_id].load_time = current_time;
    physical_memory[frame_id].last_access = current_time;
    physical_memory[frame_id].is_free = false;
    
    // Update page table
    processes[pid].page_table[page_number].frame_number = frame_id;
    processes[pid].page_table[page_number].present = true;
    processes[pid].page_table[page_number].valid = true;
    processes[pid].memory_usage += PAGE_SIZE / 1024;
    processes[pid].page_count++;
    
    current_time++;
    
    printf("✓ Page %d allocated to Frame %d for Process %d\n", 
           page_number, frame_id, pid);
    
    return 0;
}

// Deallocate a page
int deallocate_page(int pid, int page_number) {
    if(pid < 0 || pid >= MAX_PROCESSES) {
        return -1;
    }
    
    if(!processes[pid].page_table[page_number].present) {
        printf("Page %d is not allocated to process %d\n", page_number, pid);
        return -1;
    }
    
    int frame_id = processes[pid].page_table[page_number].frame_number;
    
    // Free the frame
    physical_memory[frame_id].is_free = true;
    physical_memory[frame_id].page_number = -1;
    physical_memory[frame_id].process_id = -1;
    
    // Update page table
    processes[pid].page_table[page_number].present = false;
    processes[pid].page_table[page_number].frame_number = -1;
    processes[pid].memory_usage -= PAGE_SIZE / 1024;
    processes[pid].page_count--;
    
    printf("Page %d deallocated from Frame %d\n", page_number, frame_id);
    
    return 0;
}

// Access memory (simulate memory access)
int access_memory(int pid, int address) {
    if(pid < 0 || pid >= MAX_PROCESSES || processes[pid].pid == -1) {
        printf("Error: Invalid process %d\n", pid);
        return -1;
    }
    
    int page_number = address / PAGE_SIZE;
    int offset = address % PAGE_SIZE;
    
    // Update statistics
    stats.total_references++;
    
    printf("\nMemory Access Request:\n");
    printf("  Process: %d (%s)\n", pid, processes[pid].name);
    printf("  Address: %d\n", address);
    printf("  Page: %d, Offset: %d\n", page_number, offset);
    
    // Check if page is in memory
    if(!processes[pid].page_table[page_number].present) {
        // PAGE FAULT
        printf("  ✗ PAGE FAULT: Page %d not in memory\n", page_number);
        
        processes[pid].page_faults++;
        stats.total_page_faults++;
        update_statistics(1);  // 1 = fault
        
        // Handle page fault by allocating the page
        allocate_page(pid, page_number);
        
        // Update reference
        processes[pid].page_table[page_number].referenced = true;
        int frame_id = processes[pid].page_table[page_number].frame_number;
        physical_memory[frame_id].last_access = current_time;
        physical_memory[frame_id].reference_bit = 1;
        
    } else {
        // PAGE HIT
        printf("  ✓ PAGE HIT: Page %d is in memory\n", page_number);
        
        stats.total_hits++;
        update_statistics(0);  // 0 = hit
        
        // Update access info
        processes[pid].page_table[page_number].referenced = true;
        int frame_id = processes[pid].page_table[page_number].frame_number;
        physical_memory[frame_id].last_access = current_time;
        physical_memory[frame_id].reference_bit = 1;
    }
    
    // Calculate physical address
    int frame_id = processes[pid].page_table[page_number].frame_number;
    int physical_address = (frame_id * PAGE_SIZE) + offset;
    
    current_time++;
    
    return physical_address;
}

// Find a free frame
int find_free_frame() {
    for(int i = 0; i < MAX_FRAMES; i++) {
        if(physical_memory[i].is_free) {
            return i;
        }
    }
    return -1;  // No free frames
}

// Check if page is in memory
int is_page_in_memory(int pid, int page_number) {
    if(pid < 0 || pid >= MAX_PROCESSES) {
        return 0;
    }
    return processes[pid].page_table[page_number].present ? 1 : 0;
}

// Create a new process
int create_process(const char* name) {
    // Find free process slot
    for(int i = 0; i < MAX_PROCESSES; i++) {
        if(processes[i].pid == -1) {
            init_process(i, name);
            return i;
        }
    }
    
    printf("Error: Maximum number of processes reached (%d)\n", MAX_PROCESSES);
    return -1;
}

// Terminate a process
void terminate_process(int pid) {
    if(pid < 0 || pid >= MAX_PROCESSES || processes[pid].pid == -1) {
        printf("Error: Invalid process ID %d\n", pid);
        return;
    }
    
    printf("Terminating Process %d (%s)...\n", pid, processes[pid].name);
    
    // Deallocate all pages
    for(int i = 0; i < MAX_PAGES; i++) {
        if(processes[pid].page_table[i].present) {
            deallocate_page(pid, i);
        }
    }
    
    // Reset process
    processes[pid].pid = -1;
    processes[pid].name[0] = '\0';
    processes[pid].page_faults = 0;
    processes[pid].memory_usage = 0;
    
    printf("Process %d terminated.\n", pid);
}

// Display process information
void display_process_info(int pid) {
    if(pid < 0 || pid >= MAX_PROCESSES || processes[pid].pid == -1) {
        printf("Error: Invalid process ID %d\n", pid);
        return;
    }
    
    printf("\nProcess Information:\n");
    printf("====================\n");
    printf("PID: %d\n", processes[pid].pid);
    printf("Name: %s\n", processes[pid].name);
    printf("Page Faults: %d\n", processes[pid].page_faults);
    printf("Memory Usage: %d KB\n", processes[pid].memory_usage);
    printf("Pages Allocated: %d\n", processes[pid].page_count);
    
    printf("\nAllocated Pages:\n");
    for(int i = 0; i < MAX_PAGES; i++) {
        if(processes[pid].page_table[i].present) {
            printf("  Page %d -> Frame %d\n", 
                   i, processes[pid].page_table[i].frame_number);
        }
    }
}

// Update statistics
void update_statistics(int was_fault) {
    (void)was_fault;

    if(stats.total_references > 0) {
        stats.hit_ratio = (double)stats.total_hits / stats.total_references * 100.0;
        stats.fault_ratio = (double)stats.total_page_faults / stats.total_references * 100.0;
        stats.avg_access_time = 100.0 + (stats.total_page_faults * 10000000.0 / stats.total_references);
    }
}

// Display statistics
void display_statistics() {
    printf("\n==================== STATISTICS ====================\n");
    printf("Total Memory References: %d\n", stats.total_references);
    printf("Page Hits: %d\n", stats.total_hits);
    printf("Page Faults: %d\n", stats.total_page_faults);
    printf("Hit Ratio: %.2f%%\n", stats.hit_ratio);
    printf("Fault Ratio: %.2f%%\n", stats.fault_ratio);
    printf("Average Access Time: %.2f ns\n", stats.avg_access_time);
    printf("====================================================\n");
    
    printf("\nProcess Statistics:\n");
    printf("PID  Name           Faults  Memory(KB)  Pages\n");
    printf("---  ------------   ------  ----------  -----\n");
    for(int i = 0; i < MAX_PROCESSES; i++) {
        if(processes[i].pid != -1) {
            printf("%3d  %-12s  %6d  %10d  %5d\n",
                   processes[i].pid,
                   processes[i].name,
                   processes[i].page_faults,
                   processes[i].memory_usage,
                   processes[i].page_count);
        }
    }
}

// Reset statistics
void reset_statistics() {
    stats.total_page_faults = 0;
    stats.total_hits = 0;
    stats.total_references = 0;
    stats.hit_ratio = 0.0;
    stats.fault_ratio = 0.0;
    stats.avg_access_time = 0.0;
}

// Get physical address from logical address
int get_physical_address(int pid, int logical_addr) {
    if(pid < 0 || pid >= MAX_PROCESSES) {
        return -1;
    }
    
    int page_number = logical_addr / PAGE_SIZE;
    int offset = logical_addr % PAGE_SIZE;
    
    if(!processes[pid].page_table[page_number].present) {
        return -1;  // Page not in memory
    }
    
    int frame_id = processes[pid].page_table[page_number].frame_number;
    return (frame_id * PAGE_SIZE) + offset;
}

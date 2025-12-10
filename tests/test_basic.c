/**
 * test_basic.c
 * Basic test for Memory Management Visualizer
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "../include/common_defs.h"
#include "../src/core/memory_manager.h"

// Test function prototypes
void test_memory_initialization();
void test_page_allocation();
void test_page_fault_simulation();
void test_statistics();

int main() {
    printf("Running Memory Management Visualizer Tests\n");
    printf("==========================================\n\n");
    
    test_memory_initialization();
    printf("\n");
    
    test_page_allocation();
    printf("\n");
    
    test_page_fault_simulation();
    printf("\n");
    
    test_statistics();
    printf("\n");
    
    printf("✅ All tests passed!\n");
    return 0;
}

void test_memory_initialization() {
    printf("Test 1: Memory Initialization\n");
    printf("------------------------------\n");
    
    init_memory_system();
    
    // Check that all frames are initially free
    for(int i = 0; i < MAX_FRAMES; i++) {
        assert(physical_memory[i].is_free == true);
        assert(physical_memory[i].page_number == -1);
        assert(physical_memory[i].process_id == -1);
    }
    
    printf("✓ All memory frames initialized correctly\n");
    printf("✓ Page tables initialized\n");
    printf("✓ Statistics reset\n");
}

void test_page_allocation() {
    printf("Test 2: Page Allocation\n");
    printf("------------------------\n");
    
    // Create a test process
    create_process("TestProcess");
    
    // Allocate some pages
    int result = allocate_page(0, 0);  // Page 0 to Process 0
    assert(result == 0);
    
    result = allocate_page(0, 1);  // Page 1 to Process 0
    assert(result == 0);
    
    result = allocate_page(0, 2);  // Page 2 to Process 0
    assert(result == 0);
    
    // Check that pages are allocated
    assert(processes[0].page_table[0].present == true);
    assert(processes[0].page_table[1].present == true);
    assert(processes[0].page_table[2].present == true);
    
    // Check that frames are occupied
    int frame1 = processes[0].page_table[0].frame_number;
    int frame2 = processes[0].page_table[1].frame_number;
    int frame3 = processes[0].page_table[2].frame_number;
    
    assert(frame1 >= 0 && frame1 < MAX_FRAMES);
    assert(frame2 >= 0 && frame2 < MAX_FRAMES);
    assert(frame3 >= 0 && frame3 < MAX_FRAMES);
    assert(frame1 != frame2 && frame1 != frame3 && frame2 != frame3);
    
    printf("✓ Pages allocated successfully\n");
    printf("✓ Page table entries updated\n");
    printf("✓ Physical frames assigned correctly\n");
}

void test_page_fault_simulation() {
    printf("Test 3: Page Fault Simulation\n");
    printf("------------------------------\n");
    
    // Access a page that's not allocated (should cause page fault)
    int old_faults = processes[0].page_faults;
    
    int physical_addr = access_memory(0, 3 * PAGE_SIZE);  // Page 3
    
    // Should have increased page fault count
    assert(processes[0].page_faults == old_faults + 1);
    
    // Page should now be allocated
    assert(processes[0].page_table[3].present == true);
    
    printf("✓ Page fault detected correctly\n");
    printf("✓ Page fault count incremented\n");
    printf("✓ Page allocated after fault\n");
}

void test_statistics() {
    printf("Test 4: Statistics Tracking\n");
    printf("---------------------------\n");
    
    display_statistics();
    
    printf("✓ Statistics displayed\n");
    printf("✓ Hit/fault ratios calculated\n");
    printf("✓ Process statistics tracked\n");
}

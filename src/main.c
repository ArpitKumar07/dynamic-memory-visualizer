/**
 * main.c
 * Dynamic Memory Management Visualizer - Main Program
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "include/common_defs.h"
#include "core/memory_manager.h"
#include "core/paging.h"
#include "core/segmentation.h"
#include "core/virtual_memory.h"
#include "algorithms/fifo.h"
#include "algorithms/lru.h"
#include "algorithms/optimal.h"
#include "visualization/console_gui.h"
#include "visualization/memory_display.h"
#include "utils/helpers.h"
#include "utils/logger.h"

// Global variables
MemoryFrame physical_memory[MAX_FRAMES];
Process processes[MAX_PROCESSES];
Statistics stats = {0};
int current_time = 0;
int algorithm_type = 0;  // Default: FIFO
int simulation_running = 0;

int main() {
    int choice;
    int pid, page_num, address;
    char process_name[50];
    
    // Seed random number generator
    srand(time(NULL));
    
    printf("\n");
    printf("╔════════════════════════════════════════════════════════════╗\n");
    printf("║          DYNAMIC MEMORY MANAGEMENT VISUALIZER             ║\n");
    printf("║                    Academic Project                        ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n");
    printf("\n");
    
    // Initialize logger
    init_logger("memory_simulation.log");
    LOG_INFO("Memory Management Visualizer started");
    
    do {
        display_main_menu();
        choice = get_integer_input("Enter your choice (1-10): ", 1, 10);
        
        switch(choice) {
            case 1: // Initialize Memory System
                init_memory_system();
                simulation_running = 1;
                printf("\n");
                print_success("Memory system initialized successfully!");
                printf("  - Physical frames: %d\n", MAX_FRAMES);
                printf("  - Virtual pages: %d\n", MAX_PAGES);
                printf("  - Page size: %d bytes\n", PAGE_SIZE);
                wait_for_enter();
                break;
                
            case 2: // Create Process
                if(!simulation_running) {
                    print_error("Please initialize memory system first!");
                    wait_for_enter();
                    break;
                }
                
                get_string_input("Enter process name: ", process_name, 50);
                pid = create_process(process_name);
                if(pid != -1) {
                    print_success("Process created successfully!");
                    printf("  - Process ID: %d\n", pid);
                    printf("  - Process Name: %s\n", process_name);
                }
                wait_for_enter();
                break;
                
            case 3: // Allocate Pages
                if(!simulation_running) {
                    print_error("Please initialize memory system first!");
                    wait_for_enter();
                    break;
                }
                
                pid = get_integer_input("Enter Process ID: ", 0, MAX_PROCESSES-1);
                page_num = get_integer_input("Enter page number to allocate: ", 0, MAX_PAGES-1);
                
                if(allocate_page(pid, page_num) == 0) {
                    print_success("Page allocated successfully!");
                    display_memory_map();
                } else {
                    print_error("Page allocation failed!");
                }
                wait_for_enter();
                break;
                
            case 4: // Access Memory (Page Fault Simulation)
                if(!simulation_running) {
                    print_error("Please initialize memory system first!");
                    wait_for_enter();
                    break;
                }
                
                pid = get_integer_input("Enter Process ID: ", 0, MAX_PROCESSES-1);
                address = get_integer_input("Enter memory address to access: ", 0, MAX_PAGES * PAGE_SIZE - 1);
                
                int result = access_memory(pid, address);
                if(result >= 0) {
                    printf("\n");
                    print_success("Memory access successful!");
                    printf("  - Logical address: %d\n", address);
                    printf("  - Physical address: %d\n", result);
                    printf("  - Page number: %d\n", address / PAGE_SIZE);
                }
                wait_for_enter();
                break;
                
            case 5: // Paging System
                display_paging_menu();
                int paging_choice = get_integer_input("Enter choice: ", 1, 5);
                
                switch(paging_choice) {
                    case 1: // Initialize Paging
                        init_paging_system(MAX_PAGES, PAGE_SIZE);
                        print_success("Paging system initialized");
                        break;
                    case 2: // Address Translation
                        pid = get_integer_input("Process ID: ", 0, MAX_PROCESSES-1);
                        address = get_integer_input("Logical address: ", 0, 99999);
                        translate_paging_address(pid, address);
                        break;
                    case 3: // Display Page Table
                        pid = get_integer_input("Process ID: ", 0, MAX_PROCESSES-1);
                        display_page_table(pid);
                        break;
                    case 4: // Simulate Page Fault
                        pid = get_integer_input("Process ID: ", 0, MAX_PROCESSES-1);
                        page_num = get_integer_input("Page number: ", 0, MAX_PAGES-1);
                        simulate_paging_fault(pid, page_num);
                        break;
                    case 5: // Back
                        break;
                }
                wait_for_enter();
                break;
                
            case 6: // Segmentation System
                display_segmentation_menu();
                int seg_choice = get_integer_input("Enter choice: ", 1, 5);
                
                switch(seg_choice) {
                    case 1:
                        init_segmentation_system(MAX_SEGMENTS);
                        break;
                    case 2:
                        pid = get_integer_input("Process ID: ", 0, MAX_PROCESSES-1);
                        int seg_num = get_integer_input("Segment number: ", 0, MAX_SEGMENTS-1);
                        int offset = get_integer_input("Offset: ", 0, SEGMENT_SIZE-1);
                        translate_segmented_address(pid, seg_num, offset);
                        break;
                    case 3:
                        pid = get_integer_input("Process ID: ", 0, MAX_PROCESSES-1);
                        display_segment_table(pid);
                        break;
                    case 4:
                        pid = get_integer_input("Process ID: ", 0, MAX_PROCESSES-1);
                        seg_num = get_integer_input("Segment number: ", 0, MAX_SEGMENTS-1);
                        int base = get_integer_input("Base address: ", 0, 99999);
                        int limit = get_integer_input("Limit: ", 1, SEGMENT_SIZE);
                        create_segment(pid, seg_num, base, limit);
                        break;
                }
                wait_for_enter();
                break;
                
            case 7: // Page Replacement Algorithms
                display_algorithms_menu();
                int algo_choice = get_integer_input("Select algorithm: ", 1, 4);
                
                switch(algo_choice) {
                    case 1:
                        algorithm_type = 0;
                        init_fifo();
                        print_success("FIFO algorithm selected");
                        break;
                    case 2:
                        algorithm_type = 1;
                        init_lru();
                        print_success("LRU algorithm selected");
                        break;
                    case 3:
                        algorithm_type = 2;
                        init_optimal();
                        print_success("Optimal algorithm selected");
                        break;
                    case 4:
                        // Run algorithm demo
                        printf("\nRunning algorithm comparison...\n");
                        // Demo code would go here
                        break;
                }
                wait_for_enter();
                break;
                
            case 8: // Display Statistics
                if(!simulation_running) {
                    print_error("Please run simulation first!");
                } else {
                    display_statistics();
                    printf("\nCurrent memory state:\n");
                    display_memory_grid();
                }
                wait_for_enter();
                break;
                
            case 9: // Run Demo Simulation
                printf("\n");
                printf("════════════════════════════════════════════════════════════\n");
                printf("                RUNNING DEMO SIMULATION                   \n");
                printf("════════════════════════════════════════════════════════════\n");
                
                // Initialize
                init_memory_system();
                
                // Create processes
                int p1 = create_process("Browser");
                int p2 = create_process("Text Editor");
                
                // Allocate some pages
                allocate_page(p1, 0);
                allocate_page(p1, 1);
                allocate_page(p1, 2);
                allocate_page(p2, 3);
                allocate_page(p2, 4);
                
                // Access memory to generate page faults
                printf("\nSimulating memory accesses...\n");
                for(int i = 0; i < 10; i++) {
                    int addr = random_int(0, MAX_PAGES * PAGE_SIZE - 1);
                    access_memory(p1, addr);
                    delay(500);  // Half second delay
                }
                
                // Show results
                display_statistics();
                display_memory_map();
                
                print_success("Demo simulation completed!");
                wait_for_enter();
                break;
                
            case 10: // Exit
                printf("\n");
                print_info("Thank you for using Memory Management Visualizer!");
                printf("Statistics from this session:\n");
                display_statistics();
                
                // Close logger
                close_logger();
                
                printf("\nExiting program...\n");
                break;
                
            default:
                print_error("Invalid choice! Please try again.");
                wait_for_enter();
        }
    } while(choice != 10);
    
    return 0;
}

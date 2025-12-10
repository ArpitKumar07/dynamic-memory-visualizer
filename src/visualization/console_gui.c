/**
 * console_gui.c
 * Console-based user interface implementation
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "console_gui.h"

// Clear screen
void clear_screen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

// Print header
void print_header(const char* title) {
    printf("\n");
    printf("╔════════════════════════════════════════════════════════════╗\n");
    printf("║%*s%*s║\n", 
           (int)((60 + strlen(title)) / 2), title,
           (int)(60 - (60 + strlen(title)) / 2), "");
    printf("╚════════════════════════════════════════════════════════════╝\n");
}

// Print footer
void print_footer() {
    printf("\n════════════════════════════════════════════════════════════\n");
}

// Wait for Enter key
void wait_for_enter() {
    printf("\nPress Enter to continue...");
    fflush(stdout);
    
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Display main menu
void display_main_menu() {
    clear_screen();
    print_header("DYNAMIC MEMORY MANAGEMENT VISUALIZER");
    
    printf("\nMAIN MENU:\n");
    printf("════════════════════════════════════════════════════════════\n");
    printf("1.  Initialize Memory System\n");
    printf("2.  Create New Process\n");
    printf("3.  Allocate Pages to Process\n");
    printf("4.  Access Memory (Simulate Page Faults)\n");
    printf("5.  Paging System Operations\n");
    printf("6.  Segmentation System Operations\n");
    printf("7.  Page Replacement Algorithms\n");
    printf("8.  Display Statistics & Memory State\n");
    printf("9.  Run Demo Simulation\n");
    printf("10. Exit Program\n");
    printf("════════════════════════════════════════════════════════════\n");
}

// Display paging menu
void display_paging_menu() {
    clear_screen();
    print_header("PAGING SYSTEM OPERATIONS");
    
    printf("\nPAGING MENU:\n");
    printf("════════════════════════════════════════════════════════════\n");
    printf("1. Initialize Paging System\n");
    printf("2. Translate Logical Address\n");
    printf("3. Display Page Table\n");
    printf("4. Simulate Page Fault\n");
    printf("5. Back to Main Menu\n");
    printf("════════════════════════════════════════════════════════════\n");
}

// Display segmentation menu
void display_segmentation_menu() {
    clear_screen();
    print_header("SEGMENTATION SYSTEM OPERATIONS");
    
    printf("\nSEGMENTATION MENU:\n");
    printf("════════════════════════════════════════════════════════════\n");
    printf("1. Initialize Segmentation System\n");
    printf("2. Translate Segmented Address\n");
    printf("3. Display Segment Table\n");
    printf("4. Create New Segment\n");
    printf("5. Back to Main Menu\n");
    printf("════════════════════════════════════════════════════════════\n");
}

// Display virtual memory menu
void display_virtual_memory_menu() {
    clear_screen();
    print_header("VIRTUAL MEMORY OPERATIONS");
    
    printf("\nVIRTUAL MEMORY MENU:\n");
    printf("════════════════════════════════════════════════════════════\n");
    printf("1. Initialize Virtual Memory\n");
    printf("2. Simulate Virtual Memory Access\n");
    printf("3. Display Virtual Memory State\n");
    printf("4. Display Swap Space\n");
    printf("5. Back to Main Menu\n");
    printf("════════════════════════════════════════════════════════════\n");
}

// Display algorithms menu
void display_algorithms_menu() {
    clear_screen();
    print_header("PAGE REPLACEMENT ALGORITHMS");
    
    printf("\nALGORITHMS MENU:\n");
    printf("════════════════════════════════════════════════════════════\n");
    printf("1. FIFO (First-In-First-Out)\n");
    printf("2. LRU (Least Recently Used)\n");
    printf("3. Optimal (Theoretical Best)\n");
    printf("4. Run Algorithm Comparison Demo\n");
    printf("5. Back to Main Menu\n");
    printf("════════════════════════════════════════════════════════════\n");
    
    printf("\nAlgorithm Information:\n");
    printf("──────────────────────\n");
    printf("FIFO:  Simple, replaces oldest page\n");
    printf("LRU:   Replaces least recently used page\n");
    printf("Optimal: Replaces page not used for longest time (theoretical)\n");
}

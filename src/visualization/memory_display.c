/**
#include <string.h>
 * memory_display.c
 * Memory visualization functions implementation
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "memory_display.h"
#include "../../include/common_defs.h"
#include "../core/memory_manager.h"

// Display memory as a map
void display_memory_map() {
    printf("\n");
    printf("┌────────────────────────────────────────────────────────────┐\n");
    printf("│                    PHYSICAL MEMORY MAP                    │\n");
    printf("├────────────────────────────────────────────────────────────┤\n");
    
    int frames_per_row = 5;
    int rows = (MAX_FRAMES + frames_per_row - 1) / frames_per_row;
    
    for(int row = 0; row < rows; row++) {
        printf("│ ");
        for(int col = 0; col < frames_per_row; col++) {
            int frame_idx = row * frames_per_row + col;
            if(frame_idx < MAX_FRAMES) {
                printf("Frame %2d: ", frame_idx);
                if(physical_memory[frame_idx].is_free) {
                    printf("[  FREE  ]  ");
                } else {
                    printf("[P%2d:P%2d]  ",
                           physical_memory[frame_idx].process_id,
                           physical_memory[frame_idx].page_number);
                }
            } else {
                printf("                   ");
            }
        }
        printf("│\n");
    }
    
    printf("└────────────────────────────────────────────────────────────┘\n");
    
    printf("\nLegend: [Ppid:Ppg] = Process ID : Page Number\n");
    printf("       [  FREE  ]  = Available memory frame\n");
    
    // Show usage statistics
    int used_frames = 0;
    for(int i = 0; i < MAX_FRAMES; i++) {
        if(!physical_memory[i].is_free) {
            used_frames++;
        }
    }
    
    printf("\nMemory Usage: %d/%d frames (%.1f%% full)\n",
           used_frames, MAX_FRAMES,
           (used_frames * 100.0) / MAX_FRAMES);
}

// Display memory as a grid
void display_memory_grid() {
    printf("\n");
    printf("┌───────────────────────── PHYSICAL MEMORY GRID ─────────────────────────┐\n");
    
    printf("│ Frame │ Page │ Process │ Load Time │ Last Access │ Status  │\n");
    printf("├───────┼──────┼─────────┼───────────┼─────────────┼─────────┤\n");
    
    for(int i = 0; i < MAX_FRAMES; i++) {
        printf("│ %5d │ ", i);
        
        if(physical_memory[i].is_free) {
            printf("%4s │ %7s │ %9s │ %11s │ %-7s │\n",
                   "--", "--", "--", "--", "FREE");
        } else {
            printf("%4d │ %7d │ %9d │ %11d │ %-7s │\n",
                   physical_memory[i].page_number,
                   physical_memory[i].process_id,
                   physical_memory[i].load_time,
                   physical_memory[i].last_access,
                   "USED");
        }
    }
    
    printf("└───────┴──────┴─────────┴───────────┴─────────────┴─────────┘\n");
}

// Display page table visually
void display_page_table_visual(int pid) {
    if(pid < 0 || pid >= MAX_PROCESSES || processes[pid].pid == -1) {
        printf("Error: Invalid process ID %d\n", pid);
        return;
    }
    
    printf("\n");
    printf("┌───────────────────── PAGE TABLE VISUALIZATION ─────────────────────┐\n");
    printf("│ Process: %d (%s)                                                  │\n", 
           pid, processes[pid].name);
    printf("├──────┬───────┬───────┬─────────┬─────────┬──────────┬─────────────┤\n");
    printf("│ Page │ Frame │ Valid │ Present │ Refer'd │ Modified │ Protection  │\n");
    printf("├──────┼───────┼───────┼─────────┼─────────┼──────────┼─────────────┤\n");
    
    for(int i = 0; i < 15; i++) {  // Show first 15 pages
        PageTableEntry* entry = &processes[pid].page_table[i];
        
        printf("│ %4d │ ", i);
        
        if(entry->frame_number == -1) {
            printf("%5s │ ", "--");
        } else {
            printf("%5d │ ", entry->frame_number);
        }
        
        printf("%5s │ ", entry->valid ? "Yes" : "No");
        printf("%7s │ ", entry->present ? "Yes" : "No");
        printf("%8s │ ", entry->referenced ? "Yes" : "No");
        printf("%8s │ ", entry->modified ? "Yes" : "No");
        
        // Protection bits: R=Read, W=Write, X=Execute
        char prot[4] = "---";
        if(entry->protection & 4) prot[0] = 'R';
        if(entry->protection & 2) prot[1] = 'W';
        if(entry->protection & 1) prot[2] = 'X';
        printf("%11s │\n", prot);
    }
    
    printf("└──────┴───────┴───────┴─────────┴─────────┴──────────┴─────────────┘\n");
    
    // Show summary
    int present_count = 0;
    for(int i = 0; i < MAX_PAGES; i++) {
        if(processes[pid].page_table[i].present) {
            present_count++;
        }
    }
    
    printf("\nSummary: %d/%d pages resident in memory\n", 
           present_count, MAX_PAGES);
}

// Display segment table visually
void display_segment_table_visual(int pid) {
    if(pid < 0 || pid >= MAX_PROCESSES || processes[pid].pid == -1) {
        printf("Error: Invalid process ID %d\n", pid);
        return;
    }
    
    printf("\n");
    printf("┌─────────────────── SEGMENT TABLE VISUALIZATION ───────────────────┐\n");
    printf("│ Process: %d (%s)                                                │\n", 
           pid, processes[pid].name);
    printf("├─────┬─────────────┬─────────┬───────┬─────────────────────────────┤\n");
    printf("│ Seg │ Base Addr   │ Limit   │ Valid │ Protection                  │\n");
    printf("├─────┼─────────────┼─────────┼───────┼─────────────────────────────┤\n");
    
    for(int i = 0; i < MAX_SEGMENTS; i++) {
        SegmentTableEntry* seg = &processes[pid].segment_table[i];
        
        printf("│ %3d │ ", i);
        
        if(seg->base_address == -1) {
            printf("%11s │ ", "--");
        } else {
            printf("%11d │ ", seg->base_address);
        }
        
        printf("%7d │ ", seg->limit);
        printf("%5s │ ", seg->valid ? "Yes" : "No");
        
        // Protection display
        char prot_str[20];
        int prot = seg->protection;
        sprintf(prot_str, "%s%s%s",
                (prot & 4) ? "Read " : "",
                (prot & 2) ? "Write " : "",
                (prot & 1) ? "Execute" : "");
        if(prot_str[0] == '\0') strcpy(prot_str, "None");
        
        printf("%-27s │\n", prot_str);
    }
    
    printf("└─────┴─────────────┴─────────┴───────┴─────────────────────────────┘\n");
}

// Animate page fault
void animate_page_fault(int pid, int page_number) {
    printf("\n");
    printf("PAGE FAULT ANIMATION for Page %d (Process %d)\n", page_number, pid);
    printf("════════════════════════════════════════════════════════════\n");
    
    for(int step = 1; step <= 5; step++) {
        printf("\nStep %d: ", step);
        
        switch(step) {
            case 1:
                printf("CPU generates address for Page %d\n", page_number);
                printf("       Checking page table...\n");
                break;
            case 2:
                printf("PAGE FAULT! Page %d not in memory\n", page_number);
                printf("       Trap to operating system\n");
                break;
            case 3:
                printf("OS checks if page exists on disk\n");
                printf("       Locating page in storage...\n");
                break;
            case 4:
                printf("Finding free frame in physical memory\n");
                printf("       Checking memory availability...\n");
                break;
            case 5:
                printf("Loading page into memory\n");
                printf("       Updating page table...\n");
                break;
        }
        
        fflush(stdout);
        usleep(800000);  // 800ms delay for animation
    }
    
    printf("\n✓ Page fault handling complete!\n");
}

// Animate page replacement
void animate_page_replacement(int old_page, int new_page) {
    printf("\n");
    printf("PAGE REPLACEMENT ANIMATION\n");
    printf("════════════════════════════════════════════════════════════\n");
    
    printf("Replacing Page %d with Page %d\n", old_page, new_page);
    
    for(int step = 1; step <= 4; step++) {
        printf("\nStep %d: ", step);
        
        switch(step) {
            case 1:
                printf("Select victim page (Page %d)\n", old_page);
                break;
            case 2:
                printf("Check if victim page is modified\n");
                break;
            case 3:
                printf("Write victim page to disk if needed\n");
                break;
            case 4:
                printf("Load new page (Page %d) into memory\n", new_page);
                break;
        }
        
        fflush(stdout);
        usleep(800000);  // 800ms delay
    }
    
    printf("\n✓ Page replacement complete!\n");
}

// Show access pattern
void show_access_pattern(int* pattern, int length) {
    if(length <= 0 || pattern == NULL) {
        printf("Error: Invalid access pattern\n");
        return;
    }
    
    printf("\n");
    printf("MEMORY ACCESS PATTERN (Length: %d)\n", length);
    printf("════════════════════════════════════════════════════════════\n");
    
    int cols = 10;
    for(int i = 0; i < length; i++) {
        if(i % cols == 0) {
            printf("\n%4d-%-4d: ", i, i + cols - 1 < length ? i + cols - 1 : length - 1);
        }
        printf("%3d ", pattern[i]);
    }
    
    printf("\n\nLegend: Numbers represent page numbers being accessed\n");
}

/**
 * lru.c
 * Least Recently Used page replacement algorithm implementation
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "lru.h"
#include "../../include/common_defs.h"
#include "../core/memory_manager.h"

// LRU stack implementation
typedef struct {
    int frames[MAX_FRAMES];
    int timestamps[MAX_FRAMES];
    int size;
} LRUStack;

LRUStack lru_stack;

// Initialize LRU algorithm
void init_lru() {
    lru_stack.size = 0;
    
    // Initialize with free frames
    for(int i = 0; i < MAX_FRAMES; i++) {
        if(physical_memory[i].is_free) {
            lru_stack.frames[lru_stack.size] = i;
            lru_stack.timestamps[lru_stack.size] = -1;
            lru_stack.size++;
        }
    }
    
    printf("LRU algorithm initialized\n");
    printf("Stack size: %d frames\n", lru_stack.size);
}

// Replace a page using LRU
int lru_replace_page() {
    if(lru_stack.size == 0) {
        printf("LRU Error: Stack is empty\n");
        return -1;
    }
    
    // Find frame with smallest timestamp (least recently used)
    int lru_index = 0;
    int min_time = INT_MAX;
    
    for(int i = 0; i < lru_stack.size; i++) {
        int frame_id = lru_stack.frames[i];
        if(!physical_memory[frame_id].is_free && 
           physical_memory[frame_id].last_access < min_time) {
            min_time = physical_memory[frame_id].last_access;
            lru_index = i;
        }
    }
    
    int frame_to_replace = lru_stack.frames[lru_index];
    
    printf("\nLRU Page Replacement:\n");
    printf("====================\n");
    printf("Selected frame: %d (least recently used)\n", frame_to_replace);
    printf("Page in frame: %d (Process %d)\n", 
           physical_memory[frame_to_replace].page_number,
           physical_memory[frame_to_replace].process_id);
    printf("Last accessed at time: %d\n", min_time);
    printf("Current time: %d\n", current_time);
    
    // Move the replaced frame to the end (it will be reused)
    for(int i = lru_index; i < lru_stack.size - 1; i++) {
        lru_stack.frames[i] = lru_stack.frames[i + 1];
        lru_stack.timestamps[i] = lru_stack.timestamps[i + 1];
    }
    lru_stack.frames[lru_stack.size - 1] = frame_to_replace;
    lru_stack.timestamps[lru_stack.size - 1] = current_time;
    
    return frame_to_replace;
}

// Update LRU when a page is accessed
void update_lru(int frame_id) {
    // Find the frame in the stack
    for(int i = 0; i < lru_stack.size; i++) {
        if(lru_stack.frames[i] == frame_id) {
            // Move to most recently used position (end of stack)
            int temp_frame = lru_stack.frames[i];
            int temp_time = lru_stack.timestamps[i];
            
            for(int j = i; j < lru_stack.size - 1; j++) {
                lru_stack.frames[j] = lru_stack.frames[j + 1];
                lru_stack.timestamps[j] = lru_stack.timestamps[j + 1];
            }
            
            lru_stack.frames[lru_stack.size - 1] = temp_frame;
            lru_stack.timestamps[lru_stack.size - 1] = current_time;
            
            return;
        }
    }
}

// Access a page using LRU
void lru_access_page(int pid, int page_number) {
    printf("\nLRU Algorithm Processing:\n");
    printf("=========================\n");
    printf("Process %d accessing page %d\n", pid, page_number);
    
    // Check if page is in memory
    if(is_page_in_memory(pid, page_number)) {
        printf("Page is in memory - updating LRU stack\n");
        
        int frame_id = processes[pid].page_table[page_number].frame_number;
        update_lru(frame_id);
        
        printf("Frame %d moved to MRU position\n", frame_id);
    } else {
        printf("Page fault occurred\n");
        
        // Find free frame
        int free_frame = find_free_frame();
        
        if(free_frame != -1) {
            printf("Free frame found: %d\n", free_frame);
            
            // Add to LRU stack
            if(lru_stack.size < MAX_FRAMES) {
                lru_stack.frames[lru_stack.size] = free_frame;
                lru_stack.timestamps[lru_stack.size] = current_time;
                lru_stack.size++;
            }
            
            printf("Frame %d added to LRU stack\n", free_frame);
        } else {
            printf("No free frames - need replacement\n");
            int replaced_frame = lru_replace_page();
            printf("Frame %d selected for replacement\n", replaced_frame);
        }
    }
}

// Display LRU stack
void display_lru_stack() {
    printf("\nLRU Stack (LRU -> MRU):\n");
    printf("=======================\n");
    
    if(lru_stack.size == 0) {
        printf("Stack is empty\n");
        return;
    }
    
    printf("Position  Frame  Page  Process  Last Access\n");
    printf("--------  -----  ----  -------  -----------\n");
    
    for(int i = 0; i < lru_stack.size; i++) {
        int frame_id = lru_stack.frames[i];
        
        printf("%8d  %5d  ", i + 1, frame_id);
        
        if(physical_memory[frame_id].is_free) {
            printf("%4s  %7s  %11s\n", "--", "--", "--");
        } else {
            printf("%4d  %7d  %11d\n",
                   physical_memory[frame_id].page_number,
                   physical_memory[frame_id].process_id,
                   physical_memory[frame_id].last_access);
        }
    }
    
    printf("\nStack size: %d frames\n", lru_stack.size);
    printf("MRU: Most Recently Used, LRU: Least Recently Used\n");
}

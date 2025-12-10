/**
#include <time.h>
 * optimal.c
 * Optimal page replacement algorithm implementation
 */

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "optimal.h"
#include "../../include/common_defs.h"
#include "../core/memory_manager.h"

// Initialize Optimal algorithm
void init_optimal() {
    printf("Optimal algorithm initialized\n");
    printf("Note: Optimal requires knowledge of future references\n");
}

// Replace a page using Optimal algorithm
int optimal_replace_page(int pid, int* future_refs, int ref_count) {
    printf("\nOptimal Page Replacement:\n");
    printf("=========================\n");
    
    // Find frame that won't be used for longest time
    int selected_frame = -1;
    int max_future_use = -1;
    
    for(int i = 0; i < MAX_FRAMES; i++) {
        if(!physical_memory[i].is_free) {
            int frame_pid = physical_memory[i].process_id;
            int frame_page = physical_memory[i].page_number;
            
            // Find when this page will be used next
            int next_use = INT_MAX;
            
            for(int j = 0; j < ref_count; j++) {
                // We need to check if this reference is for the same page
                // In real implementation, we'd need more context
                if(future_refs[j] == frame_page) {
                    next_use = j;
                    break;
                }
            }
            
            printf("Frame %d (Page %d, Process %d): ", 
                   i, frame_page, frame_pid);
            
            if(next_use == INT_MAX) {
                printf("Never used again\n");
                // This page won't be used again - perfect candidate
                selected_frame = i;
                break;
            } else {
                printf("Next use at reference %d\n", next_use);
                
                if(next_use > max_future_use) {
                    max_future_use = next_use;
                    selected_frame = i;
                }
            }
        }
    }
    
    if(selected_frame != -1) {
        printf("\nSelected frame: %d\n", selected_frame);
        printf("Page %d (Process %d) will be replaced\n",
               physical_memory[selected_frame].page_number,
               physical_memory[selected_frame].process_id);
        
        if(max_future_use == INT_MAX) {
            printf("This page will never be used again (optimal choice)\n");
        } else {
            printf("This page won't be used for %d more references\n", 
                   max_future_use);
        }
    } else {
        printf("Error: Could not select a frame for replacement\n");
        selected_frame = 0;  // Default to frame 0
    }
    
    return selected_frame;
}

// Predict future references (simulated)
int* predict_future_references(int pid, int lookahead) {
    if(lookahead <= 0) {
        return NULL;
    }
    
    int* future_refs = (int*)malloc(lookahead * sizeof(int));
    if(future_refs == NULL) {
        printf("Error: Memory allocation failed\n");
        return NULL;
    }
    
    printf("\nPredicting future references for Process %d:\n", pid);
    printf("Lookahead: %d references\n", lookahead);
    
    // Simple prediction: assume some locality of reference
    srand(time(NULL));
    
    int current_page = 0;
    if(processes[pid].page_count > 0) {
        // Start with a random page from process
        current_page = rand() % MAX_PAGES;
    }
    
    for(int i = 0; i < lookahead; i++) {
        // 80% chance to stay near current page, 20% chance to jump
        if(rand() % 100 < 80) {
            current_page = (current_page + (rand() % 3) - 1) % MAX_PAGES;
            if(current_page < 0) current_page = 0;
        } else {
            current_page = rand() % MAX_PAGES;
        }
        future_refs[i] = current_page;
        
        printf("Reference %d: Page %d\n", i + 1, current_page);
    }
    
    return future_refs;
}

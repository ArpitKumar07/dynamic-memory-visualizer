/**
 * fifo.c
 * First-In-First-Out page replacement algorithm implementation
 */

#include <stdio.h>
#include <stdlib.h>
#include "fifo.h"
#include "../../include/common_defs.h"
#include "../core/memory_manager.h"

// FIFO queue implementation
typedef struct {
    int frames[MAX_FRAMES];
    int front;
    int rear;
    int count;
} FIFOQueue;

FIFOQueue fifo_queue;

// Initialize FIFO algorithm
void init_fifo() {
    fifo_queue.front = 0;
    fifo_queue.rear = -1;
    fifo_queue.count = 0;
    
    // Initialize queue with free frames
    for(int i = 0; i < MAX_FRAMES; i++) {
        if(physical_memory[i].is_free) {
            fifo_queue.rear = (fifo_queue.rear + 1) % MAX_FRAMES;
            fifo_queue.frames[fifo_queue.rear] = i;
            fifo_queue.count++;
        }
    }
    
    printf("FIFO algorithm initialized\n");
    printf("Queue size: %d frames\n", fifo_queue.count);
}

// Replace a page using FIFO
int fifo_replace_page() {
    if(fifo_queue.count == 0) {
        printf("FIFO Error: Queue is empty\n");
        return -1;
    }
    
    // Get the front frame (oldest)
    int frame_to_replace = fifo_queue.frames[fifo_queue.front];
    
    printf("\nFIFO Page Replacement:\n");
    printf("=====================\n");
    printf("Selected frame: %d (oldest in memory)\n", frame_to_replace);
    printf("Page in frame: %d (Process %d)\n", 
           physical_memory[frame_to_replace].page_number,
           physical_memory[frame_to_replace].process_id);
    printf("Loaded at time: %d\n", physical_memory[frame_to_replace].load_time);
    
    // Remove from front
    fifo_queue.front = (fifo_queue.front + 1) % MAX_FRAMES;
    fifo_queue.count--;
    
    // Add to rear (it will be reused with new page)
    fifo_queue.rear = (fifo_queue.rear + 1) % MAX_FRAMES;
    fifo_queue.frames[fifo_queue.rear] = frame_to_replace;
    fifo_queue.count++;
    
    return frame_to_replace;
}

// Access a page using FIFO
void fifo_access_page(int pid, int page_number) {
    printf("\nFIFO Algorithm Processing:\n");
    printf("==========================\n");
    printf("Process %d accessing page %d\n", pid, page_number);
    
    // Check if page is in memory
    if(is_page_in_memory(pid, page_number)) {
        printf("Page is already in memory - no action needed\n");
    } else {
        printf("Page fault occurred\n");
        
        // Find free frame
        int free_frame = find_free_frame();
        
        if(free_frame != -1) {
            printf("Free frame found: %d\n", free_frame);
            
            // Add to FIFO queue
            fifo_queue.rear = (fifo_queue.rear + 1) % MAX_FRAMES;
            fifo_queue.frames[fifo_queue.rear] = free_frame;
            fifo_queue.count++;
            
            printf("Frame %d added to FIFO queue\n", free_frame);
        } else {
            printf("No free frames - need replacement\n");
            int replaced_frame = fifo_replace_page();
            printf("Frame %d selected for replacement\n", replaced_frame);
        }
    }
}

// Display FIFO queue
void display_fifo_queue() {
    printf("\nFIFO Queue (Oldest -> Newest):\n");
    printf("==============================\n");
    
    if(fifo_queue.count == 0) {
        printf("Queue is empty\n");
        return;
    }
    
    printf("Position  Frame  Page  Process  Load Time\n");
    printf("--------  -----  ----  -------  ---------\n");
    
    int pos = 1;
    for(int i = fifo_queue.front, j = 0; j < fifo_queue.count; 
        i = (i + 1) % MAX_FRAMES, j++) {
        int frame_id = fifo_queue.frames[i];
        
        printf("%8d  %5d  ", pos++, frame_id);
        
        if(physical_memory[frame_id].is_free) {
            printf("%4s  %7s  %9s\n", "--", "--", "--");
        } else {
            printf("%4d  %7d  %9d\n",
                   physical_memory[frame_id].page_number,
                   physical_memory[frame_id].process_id,
                   physical_memory[frame_id].load_time);
        }
    }
    
    printf("\nQueue size: %d frames\n", fifo_queue.count);
}

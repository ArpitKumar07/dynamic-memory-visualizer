/**
 * segmentation.c
 * Segmentation system implementation
 */

#include <stdio.h>
#include <stdlib.h>
#include "segmentation.h"
#include "../../include/common_defs.h"
#include "memory_manager.h"

// Initialize segmentation system
void init_segmentation_system(int total_segments) {
    printf("\nInitializing Segmentation System...\n");
    printf("==================================\n");
    printf("Total segments: %d\n", total_segments);
    printf("Segment size: %d bytes\n", SEGMENT_SIZE);
    printf("✓ Segmentation system ready\n");
}

// Translate segmented address
void translate_segmented_address(int pid, int segment_num, int offset) {
    if(pid < 0 || pid >= MAX_PROCESSES || processes[pid].pid == -1) {
        printf("Error: Invalid process ID %d\n", pid);
        return;
    }
    
    if(segment_num < 0 || segment_num >= MAX_SEGMENTS) {
        printf("Error: Invalid segment number %d\n", segment_num);
        return;
    }
    
    printf("\nAddress Translation (Segmentation):\n");
    printf("-----------------------------------\n");
    printf("Segment Number: %d\n", segment_num);
    printf("Offset: %d\n", offset);
    
    SegmentTableEntry* segment = &processes[pid].segment_table[segment_num];
    
    if(!segment->valid) {
        printf("✗ SEGMENTATION FAULT: Segment %d not valid\n", segment_num);
        return;
    }
    
    if(offset < 0 || offset >= segment->limit) {
        printf("✗ SEGMENTATION FAULT: Offset %d exceeds limit %d\n", 
               offset, segment->limit);
        return;
    }
    
    int physical_addr = segment->base_address + offset;
    
    printf("✓ Segment is valid\n");
    printf("Base Address: %d\n", segment->base_address);
    printf("Limit: %d\n", segment->limit);
    printf("Physical Address: %d\n", physical_addr);
    printf("Translation successful!\n");
}

// Display segment table
void display_segment_table(int pid) {
    if(pid < 0 || pid >= MAX_PROCESSES || processes[pid].pid == -1) {
        printf("Error: Invalid process ID %d\n", pid);
        return;
    }
    
    printf("\nSegment Table for Process %d (%s):\n", 
           pid, processes[pid].name);
    printf("========================================\n");
    printf("Seg  Base Address  Limit   Valid  Protection\n");
    printf("---  ------------  -----   -----  ----------\n");
    
    for(int i = 0; i < MAX_SEGMENTS; i++) {
        printf("%3d  %12d  %5d  %5s  %10d\n",
               i,
               processes[pid].segment_table[i].base_address,
               processes[pid].segment_table[i].limit,
               processes[pid].segment_table[i].valid ? "Yes" : "No",
               processes[pid].segment_table[i].protection);
    }
}

// Create a segment
void create_segment(int pid, int segment_id, int base, int limit) {
    if(pid < 0 || pid >= MAX_PROCESSES || processes[pid].pid == -1) {
        printf("Error: Invalid process ID %d\n", pid);
        return;
    }
    
    if(segment_id < 0 || segment_id >= MAX_SEGMENTS) {
        printf("Error: Invalid segment ID %d\n", segment_id);
        return;
    }
    
    if(limit <= 0 || limit > SEGMENT_SIZE) {
        printf("Error: Limit must be between 1 and %d\n", SEGMENT_SIZE);
        return;
    }
    
    processes[pid].segment_table[segment_id].segment_id = segment_id;
    processes[pid].segment_table[segment_id].base_address = base;
    processes[pid].segment_table[segment_id].limit = limit;
    processes[pid].segment_table[segment_id].valid = true;
    processes[pid].segment_table[segment_id].protection = 7;  // RWX
    
    printf("✓ Segment %d created for Process %d\n", segment_id, pid);
    printf("  Base: %d, Limit: %d\n", base, limit);
}

// Delete a segment
void delete_segment(int pid, int segment_id) {
    if(pid < 0 || pid >= MAX_PROCESSES || processes[pid].pid == -1) {
        printf("Error: Invalid process ID %d\n", pid);
        return;
    }
    
    processes[pid].segment_table[segment_id].valid = false;
    printf("Segment %d deleted from Process %d\n", segment_id, pid);
}

/**
 * virtual_memory.h
 * Virtual Memory module header
 */

#ifndef VIRTUAL_MEMORY_H
#define VIRTUAL_MEMORY_H

#include "../../include/common_defs.h"

// Function Prototypes
void init_virtual_memory(int virtual_pages, int physical_frames);
void simulate_virtual_memory_access(int pid, int address);
void display_virtual_memory_state();
void swap_in(int pid, int page_number);
void swap_out(int pid, int page_number);
void display_swap_space();

#endif // VIRTUAL_MEMORY_H

/**
 * memory_display.h
 * Memory visualization functions
 */

#ifndef MEMORY_DISPLAY_H
#define MEMORY_DISPLAY_H

#include "../../include/common_defs.h"

// Function Prototypes
void display_memory_map();
void display_memory_grid();
void display_page_table_visual(int pid);
void display_segment_table_visual(int pid);
void animate_page_fault(int pid, int page_number);
void animate_page_replacement(int old_page, int new_page);
void show_access_pattern(int* pattern, int length);

#endif // MEMORY_DISPLAY_H

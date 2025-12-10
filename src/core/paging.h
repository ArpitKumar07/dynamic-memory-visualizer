/**
 * paging.h
 * Paging system module header
 */

#ifndef PAGING_H
#define PAGING_H

#include "../../include/common_defs.h"

// Function Prototypes
void init_paging_system(int total_pages, int page_size);
void translate_paging_address(int pid, int logical_addr);
void display_page_table(int pid);
void simulate_paging_fault(int pid, int page_number);
void handle_page_fault(int pid, int page_number);
int* generate_reference_string(int length);

#endif // PAGING_H

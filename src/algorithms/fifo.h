/**
 * fifo.h
 * First-In-First-Out page replacement algorithm
 */

#ifndef FIFO_H
#define FIFO_H

#include "../../include/common_defs.h"

// Function Prototypes
int fifo_replace_page();
void fifo_access_page(int pid, int page_number);
void init_fifo();
void display_fifo_queue();

#endif // FIFO_H

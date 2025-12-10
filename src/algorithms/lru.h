/**
 * lru.h
 * Least Recently Used page replacement algorithm
 */

#ifndef LRU_H
#define LRU_H

#include "../../include/common_defs.h"

// Function Prototypes
int lru_replace_page();
void lru_access_page(int pid, int page_number);
void update_lru(int frame_id);
void init_lru();
void display_lru_stack();

#endif // LRU_H

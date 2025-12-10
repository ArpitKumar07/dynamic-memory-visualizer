/**
 * optimal.h
 * Optimal page replacement algorithm
 */

#ifndef OPTIMAL_H
#define OPTIMAL_H

#include "../../include/common_defs.h"

// Function Prototypes
int optimal_replace_page(int pid, int* future_refs, int ref_count);
void init_optimal();
int* predict_future_references(int pid, int lookahead);

#endif // OPTIMAL_H

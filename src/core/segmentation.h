/**
 * segmentation.h
 * Segmentation system module header
 */

#ifndef SEGMENTATION_H
#define SEGMENTATION_H

#include "../../include/common_defs.h"

// Function Prototypes
void init_segmentation_system(int total_segments);
void translate_segmented_address(int pid, int segment_num, int offset);
void display_segment_table(int pid);
void create_segment(int pid, int segment_id, int base, int limit);
void delete_segment(int pid, int segment_id);

#endif // SEGMENTATION_H

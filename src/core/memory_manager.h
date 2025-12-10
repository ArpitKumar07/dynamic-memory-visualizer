/**
 * memory_manager.h
 * Memory Manager module header
 */

#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include "../../include/common_defs.h"

// ==================== GLOBAL VARIABLES ====================
extern MemoryFrame physical_memory[MAX_FRAMES];
extern Process processes[MAX_PROCESSES];
extern Statistics stats;
extern int current_time;
extern int algorithm_type;  // 0=FIFO, 1=LRU, 2=Optimal

// ==================== FUNCTION PROTOTYPES ====================

// Initialization Functions
void init_memory_system();
void init_process(int pid, const char* name);
void reset_simulation();

// Memory Operations
int allocate_page(int pid, int page_number);
int deallocate_page(int pid, int page_number);
int access_memory(int pid, int address);
void compact_memory();

// Process Management
int create_process(const char* name);
void terminate_process(int pid);
void display_process_info(int pid);

// Statistics
void update_statistics(int was_fault);
void display_statistics();
void reset_statistics();

// Utility Functions
int find_free_frame();
int is_page_in_memory(int pid, int page_number);
int get_physical_address(int pid, int logical_addr);

#endif // MEMORY_MANAGER_H

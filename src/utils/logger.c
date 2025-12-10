/**
 * logger.c
 * Logging system implementation
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include "logger.h"

static FILE* log_file = NULL;
static LogLevel current_log_level = LOG_INFO;

// Initialize logger
void init_logger(const char* filename) {
    if(log_file != NULL) {
        fclose(log_file);
    }
    
    log_file = fopen(filename, "a");
    if(log_file == NULL) {
        printf("Warning: Could not open log file '%s'\n", filename);
        log_file = stdout;
    }
    
    fprintf(log_file, "\n");
    fprintf(log_file, "========================================\n");
    fprintf(log_file, "Memory Management Visualizer Log\n");
    fprintf(log_file, "Session started\n");
    fprintf(log_file, "========================================\n");
    fflush(log_file);
}

// Set log level
void set_log_level(LogLevel level) {
    current_log_level = level;
}

// Log a message
void log_message(LogLevel level, const char* format, ...) {
    if(level < current_log_level) {
        return;  // Skip if below current log level
    }
    
    time_t now;
    time(&now);
    struct tm* local = localtime(&now);
    
    char timestamp[20];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", local);
    
    const char* level_str;
    const char* color_code = "";
    const char* reset_code = "";
    
    #ifdef LOG_COLORS
        switch(level) {
            case LOG_DEBUG:   level_str = "DEBUG";   color_code = "\033[0;37m"; break;
            case LOG_INFO:    level_str = "INFO";    color_code = "\033[0;36m"; break;
            case LOG_WARNING: level_str = "WARNING"; color_code = "\033[0;33m"; break;
            case LOG_ERROR:   level_str = "ERROR";   color_code = "\033[0;31m"; break;
            case LOG_FATAL:   level_str = "FATAL";   color_code = "\033[1;31m"; break;
            default:          level_str = "UNKNOWN"; color_code = "\033[0m";
        }
        reset_code = "\033[0m";
    #else
        switch(level) {
            case LOG_DEBUG:   level_str = "DEBUG";   break;
            case LOG_INFO:    level_str = "INFO";    break;
            case LOG_WARNING: level_str = "WARNING"; break;
            case LOG_ERROR:   level_str = "ERROR";   break;
            case LOG_FATAL:   level_str = "FATAL";   break;
            default:          level_str = "UNKNOWN";
        }
    #endif
    
    // Format the message
    va_list args;
    char message[1024];
    
    va_start(args, format);
    vsnprintf(message, sizeof(message), format, args);
    va_end(args);
    
    // Log to file
    if(log_file != NULL) {
        fprintf(log_file, "[%s] [%s] %s\n", timestamp, level_str, message);
        fflush(log_file);
    }
    
    // Also print to console for important messages
    if(level >= LOG_WARNING) {
        printf("%s[%s] [%s] %s%s\n", color_code, timestamp, level_str, message, reset_code);
        fflush(stdout);
    }
}

// Close logger
void close_logger() {
    if(log_file != NULL && log_file != stdout) {
        fprintf(log_file, "\n========================================\n");
        fprintf(log_file, "Session ended\n");
        fprintf(log_file, "========================================\n\n");
        fclose(log_file);
        log_file = NULL;
    }
}

// Log memory allocation
void log_memory_allocation(int pid, int page_number, int frame_id) {
    log_message(LOG_INFO, "Process %d: Page %d allocated to Frame %d", 
                pid, page_number, frame_id);
}

// Log page fault
void log_page_fault(int pid, int page_number) {
    log_message(LOG_WARNING, "Process %d: Page fault for Page %d", 
                pid, page_number);
}

// Log page replacement
void log_page_replacement(int old_pid, int old_page, int new_pid, int new_page, int frame_id) {
    log_message(LOG_INFO, "Frame %d: Replaced Page %d (Process %d) with Page %d (Process %d)",
                frame_id, old_page, old_pid, new_page, new_pid);
}

// Log statistics
void log_statistics(int total_refs, int hits, int faults, double hit_ratio) {
    log_message(LOG_INFO, "Statistics: Refs=%d, Hits=%d, Faults=%d, Hit Ratio=%.2f%%",
                total_refs, hits, faults, hit_ratio);
}

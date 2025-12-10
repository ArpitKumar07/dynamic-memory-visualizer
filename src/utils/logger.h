/**
 * logger.h
 * Logging system
 */

#ifndef LOGGER_H
#define LOGGER_H

// Log levels
typedef enum {
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR,
    LOG_FATAL
} LogLevel;

// Function Prototypes
void init_logger(const char* filename);
void log_message(LogLevel level, const char* format, ...);
void close_logger();
void set_log_level(LogLevel level);

// Macros for easy logging
#define LOG_DEBUG(msg, ...) log_message(LOG_DEBUG, msg, ##__VA_ARGS__)
#define LOG_INFO(msg, ...) log_message(LOG_INFO, msg, ##__VA_ARGS__)
#define LOG_WARN(msg, ...) log_message(LOG_WARNING, msg, ##__VA_ARGS__)
#define LOG_ERROR(msg, ...) log_message(LOG_ERROR, msg, ##__VA_ARGS__)

#endif // LOGGER_H

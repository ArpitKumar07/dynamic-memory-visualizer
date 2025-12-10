/**
 * helpers.c
 * Utility helper functions implementation
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include "helpers.h"

#ifdef __linux__
    #include <unistd.h>
#elif _WIN32
    #include <windows.h>
#endif



// Get integer input with validation
int get_integer_input(const char* prompt, int min, int max) {
    int value;
    char buffer[100];
    
    while(1) {
        printf("%s", prompt);
        fflush(stdout);
        
        if(fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("Error reading input\n");
            continue;
        }
        
        // Remove newline
        buffer[strcspn(buffer, "\n")] = 0;
        
        // Check if input is empty
        if(buffer[0] == '\0') {
            printf("Input cannot be empty. Please enter a number between %d and %d: ", min, max);
            continue;
        }
        
        // Validate that input contains only digits
        int valid = 1;
        for(int i = 0; buffer[i] != '\0'; i++) {
            if(!isdigit(buffer[i]) && !(i == 0 && buffer[i] == '-')) {
                valid = 0;
                break;
            }
        }
        
        if(!valid) {
            printf("Invalid input. Please enter a number between %d and %d: ", min, max);
            continue;
        }
        
        // Convert to integer
        if(sscanf(buffer, "%d", &value) != 1) {
            printf("Invalid number format. Please enter a number between %d and %d: ", min, max);
            continue;
        }
        
        // Check range
        if(value < min || value > max) {
            printf("Value must be between %d and %d. Try again: ", min, max);
            continue;
        }
        
        return value;
    }
}

// Get float input with validation
float get_float_input(const char* prompt, float min, float max) {
    float value;
    char buffer[100];
    
    while(1) {
        printf("%s", prompt);
        fflush(stdout);
        
        if(fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("Error reading input\n");
            continue;
        }
        
        // Remove newline
        buffer[strcspn(buffer, "\n")] = 0;
        
        // Check if input is empty
        if(buffer[0] == '\0') {
            printf("Input cannot be empty. Please enter a number between %.2f and %.2f: ", min, max);
            continue;
        }
        
        // Convert to float
        if(sscanf(buffer, "%f", &value) != 1) {
            printf("Invalid number format. Please enter a number between %.2f and %.2f: ", min, max);
            continue;
        }
        
        // Check range
        if(value < min || value > max) {
            printf("Value must be between %.2f and %.2f. Try again: ", min, max);
            continue;
        }
        
        return value;
    }
}

// Get string input
void get_string_input(const char* prompt, char* buffer, int max_len) {
    printf("%s", prompt);
    fflush(stdout);
    
    if(fgets(buffer, max_len, stdin) == NULL) {
        buffer[0] = '\0';
        return;
    }
    
    // Remove newline character
    buffer[strcspn(buffer, "\n")] = 0;
}

// Print error message
void print_error(const char* message) {
    printf("\n\033[1;31m✗ ERROR: %s\033[0m\n", message);
}

// Print success message
void print_success(const char* message) {
    printf("\n\033[1;32m✓ %s\033[0m\n", message);
}

// Print warning message
void print_warning(const char* message) {
    printf("\n\033[1;33m⚠  WARNING: %s\033[0m\n", message);
}

// Print info message
void print_info(const char* message) {
    printf("\n\033[1;34mℹ  INFO: %s\033[0m\n", message);
}

// Delay execution (milliseconds)
void delay(int milliseconds) {
    #ifdef _WIN32
        Sleep(milliseconds);
    #else
        usleep(milliseconds * 1000);
    #endif
}

// Generate random integer between min and max (inclusive)
int random_int(int min, int max) {
    if(min > max) {
        int temp = min;
        min = max;
        max = temp;
    }
    
    return min + rand() % (max - min + 1);
}

// Shuffle an array
void shuffle_array(int* array, int size) {
    if(size <= 1) return;
    
    srand(time(NULL));
    
    for(int i = size - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        
        // Swap
        int temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }
}

// Convert string to lowercase
void to_lowercase(char* str) {
    for(int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}

// Convert string to uppercase
void to_uppercase(char* str) {
    for(int i = 0; str[i]; i++) {
        str[i] = toupper(str[i]);
    }
}

// Check if string represents a number
int is_number(const char* str) {
    if(str == NULL || *str == '\0') {
        return 0;
    }
    
    int has_digit = 0;
    
    for(int i = 0; str[i] != '\0'; i++) {
        if(i == 0 && (str[i] == '-' || str[i] == '+')) {
            continue;  // Allow sign at beginning
        }
        
        if(!isdigit(str[i])) {
            return 0;
        }
        
        has_digit = 1;
    }
    
    return has_digit;
}

// Trim whitespace from string
void trim_whitespace(char* str) {
    if(str == NULL) return;
    
    char* end;
    
    // Trim leading space
    while(isspace((unsigned char)*str)) {
        str++;
    }
    
    // All spaces?
    if(*str == 0) {
        return;
    }
    
    // Trim trailing space
    end = str + strlen(str) - 1;
    while(end > str && isspace((unsigned char)*end)) {
        end--;
    }
    
    // Write new null terminator
    *(end + 1) = '\0';
}

// Center text in a field of given width
void center_text(const char* text, int width) {
    int len = strlen(text);
    if(len >= width) {
        printf("%s", text);
        return;
    }
    
    int padding = (width - len) / 2;
    printf("%*s%s%*s", padding, "", text, width - len - padding, "");
}

// Draw a progress bar
void draw_progress_bar(int current, int total, int width) {
    if(total <= 0) return;
    
    float percentage = (float)current / total;
    int bar_width = (int)(percentage * width);
    
    printf("[");
    for(int i = 0; i < width; i++) {
        if(i < bar_width) {
            printf("=");
        } else if(i == bar_width) {
            printf(">");
        } else {
            printf(" ");
        }
    }
    printf("] %3.0f%% (%d/%d)", percentage * 100, current, total);
}

// Clear input buffer
void clear_input_buffer() {
    int c;
    while((c = getchar()) != '\n' && c != EOF);
}

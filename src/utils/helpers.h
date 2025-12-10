/**
 * helpers.h
 * Utility helper functions
 */

#ifndef HELPERS_H
#define HELPERS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Function Prototypes
int get_integer_input(const char* prompt, int min, int max);
float get_float_input(const char* prompt, float min, float max);
void get_string_input(const char* prompt, char* buffer, int max_len);
void print_error(const char* message);
void print_success(const char* message);
void print_warning(const char* message);
void print_info(const char* message);
void delay(int milliseconds);
int random_int(int min, int max);
void shuffle_array(int* array, int size);

#endif // HELPERS_H

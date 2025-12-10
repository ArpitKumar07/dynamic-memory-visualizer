/**
 * console_gui.h
 * Console-based user interface
 */

#ifndef CONSOLE_GUI_H
#define CONSOLE_GUI_H

// Function Prototypes
void display_main_menu();
void display_paging_menu();
void display_segmentation_menu();
void display_virtual_memory_menu();
void display_algorithms_menu();
void clear_screen();
void print_header(const char* title);
void print_footer();
void wait_for_enter();

#endif // CONSOLE_GUI_H

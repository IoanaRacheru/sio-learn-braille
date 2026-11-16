#ifndef MENU_H
#define MENU_H

#include <stdint.h>

void menu_begin();
// Call once per loop to advance the optional DFPlayer test without waiting
// for Serial input. Does not consume input or change the active mode.
void menu_update();
// Returns true only when the original Free Writing processor should run.
bool menu_handle_line(const char* text, bool too_long);
bool mode_command(const char* text, char command);
bool mode_read_answer(const char* text, uint8_t& mask);

#endif

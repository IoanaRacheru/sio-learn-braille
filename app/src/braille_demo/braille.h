#ifndef BRAILLE_H
#define BRAILLE_H

#include <stdint.h>

enum class input_status { valid, empty, invalid_character, repeated_dot };

input_status read_dots(const char* text, uint8_t& dot_mask);
char identify_letter(uint8_t dot_mask);

#endif

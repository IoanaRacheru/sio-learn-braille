#ifndef BRAILLE_H
#define BRAILLE_H

#include <stdint.h>

enum class input_status { valid, empty, invalid_character, repeated_dot };

// Each bit stores one dot: bit 0 = dot 1, ..., bit 5 = dot 6.
input_status read_dots(const char* text, uint8_t& dot_mask);
// Returns '\0' if the dots do not match a letter A-Z.
char identify_letter(uint8_t dot_mask);

#endif

#ifndef BRAILLE_PATTERNS_H
#define BRAILLE_PATTERNS_H
#include <stdint.h>
// Returns zero for letters outside A-Z.
uint8_t get_letter_pattern(char letter);
#endif

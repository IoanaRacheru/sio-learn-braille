#include "braille_patterns.h"
#include "braille.h"

uint8_t get_letter_pattern(char letter) {
  if (letter < 'A' || letter > 'Z') return 0;
  // Reuse the decoder as the single source of truth; no extra SRAM table.
  for (uint8_t mask = 1; mask < 64; ++mask) {
    if (identify_letter(mask) == letter) return mask;
  }
  return 0;
}

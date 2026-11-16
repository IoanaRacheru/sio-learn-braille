#include "braille.h"

input_status read_dots(const char* text, uint8_t& dot_mask) {
  dot_mask = 0;
  uint8_t new_mask = 0;
  for (int character_index = 0; text[character_index] != '\0'; character_index++) {
    char digit = text[character_index];
    if (digit == ' ' || digit == '\t') continue;
    if (digit < '1' || digit > '6') return input_status::invalid_character;

    uint8_t dot_bit = 1U << (digit - '1');
    if ((new_mask & dot_bit) != 0) return input_status::repeated_dot;
    new_mask |= dot_bit;
  }
  if (new_mask == 0) return input_status::empty;
  dot_mask = new_mask;
  return input_status::valid;
}

char identify_letter(uint8_t dot_mask) {
  struct BrailleLetter {
    char letter;
    char dots[6];
  };

  static const BrailleLetter alphabet[] = {
    {'A', "1"},    {'B', "12"},    {'C', "14"},   {'D', "145"},
    {'E', "15"},   {'F', "124"},   {'G', "1245"}, {'H', "125"},
    {'I', "24"},   {'J', "245"},   {'K', "13"},   {'L', "123"},
    {'M', "134"},  {'N', "1345"},  {'O', "135"},  {'P', "1234"},
    {'Q', "12345"},{'R', "1235"},  {'S', "234"},  {'T', "2345"},
    {'U', "136"},  {'V', "1236"},  {'W', "2456"}, {'X', "1346"},
    {'Y', "13456"},{'Z', "1356"}
  };

  for (const BrailleLetter& entry : alphabet) {
    uint8_t expected_mask;
    read_dots(entry.dots, expected_mask);
    if (expected_mask == dot_mask) {
      return entry.letter;
    }
  }
  return '\0';
}

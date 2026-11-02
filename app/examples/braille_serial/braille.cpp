#include "braille.h"

input_status read_dots(const char* text, uint8_t& dot_mask) {
  dot_mask = 0;
  uint8_t new_mask = 0;
  for (int character_index = 0; text[character_index] != '\0'; character_index++) {
    if (text[character_index] == ' ' || text[character_index] == '\t') continue;
    if (text[character_index] < '1' || text[character_index] > '6') return input_status::invalid_character;

    // Convert the dot digit to its bit: '1' -> 000001, '2' -> 000010.
    uint8_t dot_bit = static_cast<uint8_t>(1U << (text[character_index] - '1'));
    if ((new_mask & dot_bit) != 0) return input_status::repeated_dot;
    new_mask |= dot_bit;
  }
  if (new_mask == 0) return input_status::empty;
  dot_mask = new_mask;
  return input_status::valid;
}

char identify_letter(uint8_t dot_mask) {
  // A dot mask is a number that stores which of the six dots are selected.
  static const uint8_t letter_patterns[26] = {
    0b000001, // A: dots 1
    0b000011, // B: dots 12
    0b001001, // C: dots 14
    0b011001, // D: dots 145
    0b010001, // E: dots 15
    0b001011, // F: dots 124
    0b011011, // G: dots 1245
    0b010011, // H: dots 125
    0b001010, // I: dots 24
    0b011010, // J: dots 245
    0b000101, // K: dots 13
    0b000111, // L: dots 123
    0b001101, // M: dots 134
    0b011101, // N: dots 1345
    0b010101, // O: dots 135
    0b001111, // P: dots 1234
    0b011111, // Q: dots 12345
    0b010111, // R: dots 1235
    0b001110, // S: dots 234
    0b011110, // T: dots 2345
    0b100101, // U: dots 136
    0b100111, // V: dots 1236
    0b111010, // W: dots 2456
    0b101101, // X: dots 1346
    0b111101, // Y: dots 13456
    0b110101  // Z: dots 1356
  };
  for (uint8_t letter_index = 0; letter_index < 26; ++letter_index) {
    if (letter_patterns[letter_index] == dot_mask) return static_cast<char>('A' + letter_index);
  }
  return '\0';
}

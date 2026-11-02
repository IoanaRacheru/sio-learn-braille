#include "braille.h"

char input_text[24];
uint8_t input_length = 0;
bool input_too_long = false;

void process_input() {
  if (input_too_long) {
    Serial.println(F("Input too long. Try again."));
    return;
  }
  // End the text before passing it to read_dots().
  input_text[input_length] = '\0';
  uint8_t dot_mask;
  input_status status = read_dots(input_text, dot_mask);

  if (status == input_status::empty) return;
  if (status == input_status::invalid_character) {
    Serial.println(F("Use only dots 1-6. Example: 145."));
    return;
  }
  if (status == input_status::repeated_dot) {
    Serial.println(F("Each dot can appear only once."));
    return;
  }

  char letter = identify_letter(dot_mask);
  Serial.print(F("Dots: "));
  for (uint8_t dot_index = 0; dot_index < 6; ++dot_index) {
    if ((dot_mask & (1U << dot_index)) != 0) Serial.print(dot_index + 1);
  }
  if (letter == '\0') {
    Serial.println(F(" -> No matching letter A-Z."));
  } else {
    Serial.print(F(" -> Letter: "));
    Serial.println(letter);
  }
}

void setup() {
  Serial.begin(9600);
}

void loop() {
  while (Serial.available() > 0) {
    char received_character = static_cast<char>(Serial.read());
    // Process a complete line when Enter is received.
    if (received_character == '\n' || received_character == '\r') {
      if (input_length != 0 || input_too_long) process_input();
      input_length = 0;
      input_too_long = false;
    } else if (!input_too_long) {
      if (input_length < sizeof(input_text) - 1) input_text[input_length++] = received_character;
      else input_too_long = true;
    }
  }
}

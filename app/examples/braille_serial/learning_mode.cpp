#include <Arduino.h>
#include "learning_mode.h"
#include "braille.h"
#include "braille_patterns.h"
#include "menu.h"

namespace {
LearningProgress progress = {};
char letter = 'A';
bool answered = false;
void show_letter() {
  answered = false;
  Serial.print(F("\nLetter: "));
  Serial.println(letter);
  Serial.print(F("Hint: Dots "));
  uint8_t mask = get_letter_pattern(letter);
  bool first = true;
  for (uint8_t dot = 0; dot < 6; ++dot) {
    if (mask & (1U << dot)) {
      if (!first) Serial.print(F(", "));
      Serial.print(dot + 1);
      first = false;
    }
  }
  Serial.println(F("\nEnter Braille dots (M for menu):"));
}
}

const LearningProgress& learning_mode_progress() { return progress; }
void learning_mode_begin() {
  Serial.println(F("\n===== LEARNING MODE ====="));
  show_letter();
}
void learning_mode_handle_line(const char* text) {
  if (answered) {
    if (mode_command(text, 'N')) {
      letter = letter == 'Z' ? 'A' : letter + 1;
      show_letter();
    } else Serial.println(F("Press N for next letter or M for menu."));
    return;
  }
  uint8_t mask;
  if (!mode_read_answer(text, mask)) return;
  if (identify_letter(mask) != letter) {
    ++progress.incorrect_attempts;
    Serial.println(F("Incorrect. Try the same letter again."));
    return;
  }
  ++progress.correct_answers;
  progress.completed_letters |= 1UL << (letter - 'A');
  answered = true;
  Serial.println(F("Correct!"));
  Serial.print(F("Learning correct answers: "));
  Serial.println(progress.correct_answers);
  Serial.print(F("Incorrect attempts: "));
  Serial.println(progress.incorrect_attempts);
  Serial.println(F("Press N for next letter or M for menu."));
}

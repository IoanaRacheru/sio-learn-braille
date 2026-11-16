#include <Arduino.h>
#include "test_mode.h"
#include "braille.h"
#include "braille_patterns.h"
#include "menu.h"

namespace {
TestStatistics statistics = {};
char letter = 'A';
enum class Stage { choosing_letter, answering, completed };
Stage stage = Stage::choosing_letter;
unsigned long started_at = 0;
void choose_letter() {
  stage = Stage::choosing_letter;
  Serial.println(F("\nChoose a letter (A-Z):"));
}
void print_dots() {
  const uint8_t mask = get_letter_pattern(letter);
  bool first = true;
  for (uint8_t dot = 0; dot < 6; ++dot) {
    if (mask & (1U << dot)) {
      if (!first) Serial.print(F(", "));
      Serial.print(dot + 1);
      first = false;
    }
  }
  Serial.println(F(""));
}
void show_letter() {
  stage = Stage::answering;
  Serial.print(F("\nSelected letter: "));
  Serial.println(letter);
  Serial.print(F("Braille hint: Dots "));
  print_dots();
  Serial.println(F("\nEnter the Braille dots (M for menu):"));
  started_at = millis();
}
}

const TestStatistics& test_mode_statistics() { return statistics; }
bool test_mode_is_choosing_letter() { return stage == Stage::choosing_letter; }
void test_mode_begin() {
  Serial.println(F("\n===== TEST MODE ====="));
  choose_letter();
}
void test_mode_handle_line(const char* text, unsigned long submitted_at) {
  if (stage == Stage::choosing_letter) {
    while (*text == ' ' || *text == '\t') ++text;
    char selected = *text;
    if (selected >= 'a' && selected <= 'z') selected -= 'a' - 'A';
    if (selected >= 'A' && selected <= 'Z') {
      ++text;
      while (*text == ' ' || *text == '\t') ++text;
      if (*text == '\0') {
        letter = selected;
        show_letter();
        return;
      }
    }
    Serial.println(F("Invalid letter. Choose one letter A-Z."));
    return;
  }
  if (stage == Stage::completed) {
    if (mode_command(text, 'N')) {
      choose_letter();
    } else Serial.println(F("Press N to choose another letter or M for menu."));
    return;
  }
  uint8_t mask;
  if (!mode_read_answer(text, mask)) return;
  statistics.last_response_ms = submitted_at - started_at;
  ++statistics.total_answers;
  if (identify_letter(mask) == letter) {
    ++statistics.correct_answers;
    Serial.println(F("Correct!"));
  } else {
    ++statistics.incorrect_answers;
    Serial.println(F("Incorrect!"));
    Serial.print(F("Expected dots: "));
    print_dots();
  }
  stage = Stage::completed;
  Serial.print(F("Response time: "));
  Serial.print(statistics.last_response_ms);
  Serial.println(F(" ms"));
  Serial.print(F("Total answers: "));
  Serial.println(statistics.total_answers);
  Serial.print(F("Correct answers: "));
  Serial.println(statistics.correct_answers);
  Serial.print(F("Incorrect answers: "));
  Serial.println(statistics.incorrect_answers);
  Serial.println(F("Press N to choose another letter or M for menu."));
}

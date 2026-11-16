#include <Arduino.h>
#include "menu.h"
#include "braille.h"
#include "learning_mode.h"
#include "test_mode.h"
#include "dfplayer_test.h"

namespace {
enum class Mode { menu, learning, test, free_writing, dfplayer };
Mode active_mode = Mode::menu;
bool dfplayer_started = false;
}

void menu_update() {
  if (dfplayer_started) dfplayer_test_update();
}

bool mode_command(const char* text, char command) {
  while (*text == ' ' || *text == '\t') ++text;
  if (*text != command && *text != command + ('a' - 'A')) return false;
  ++text;
  while (*text == ' ' || *text == '\t') ++text;
  return *text == '\0';
}

bool mode_read_answer(const char* text, uint8_t& mask) {
  switch (read_dots(text, mask)) {
    case input_status::valid: return true;
    case input_status::empty: Serial.println(F("Enter Braille dots.")); break;
    case input_status::invalid_character:
      Serial.println(F("Use only dots 1-6. Example: 145.")); break;
    case input_status::repeated_dot:
      Serial.println(F("Each dot can appear only once.")); break;
  }
  return false;
}

void menu_begin() {
  active_mode = Mode::menu;
  Serial.println(F("\n===== BRAILLE MENU =====\n1. Learning Mode\n2. Test Mode\n3. Free Writing Mode\n4. Test DFPlayer\n\nSelect mode:"));
}

bool menu_handle_line(const char* text, bool too_long) {
  const unsigned long submitted_at = millis();
  if (too_long) {
    if (active_mode == Mode::free_writing) return true;
    Serial.println(F("Input too long. Try again."));
    return false;
  }
  if (active_mode != Mode::menu && mode_command(text, 'M') &&
      !(active_mode == Mode::test && test_mode_is_choosing_letter())) {
    menu_begin();
    return false;
  }
  switch (active_mode) {
    case Mode::menu:
      if (text[0] == '4' && text[1] == '\0') {
#if ENABLE_DFPLAYER_TEST
        active_mode = Mode::dfplayer;
        Serial.println(F("\n===== DFPLAYER TEST =====\nPress M for menu at any time."));
        if (!dfplayer_started) {
          dfplayer_started = true;
          dfplayer_test_begin();
        } else {
          Serial.println(F("Test already started; initialization will not repeat. Reset the board to retry."));
        }
#else
        Serial.println(F("DFPlayer test disabled. Build with ENABLE_DFPLAYER_TEST=1 for all C++ files.\nSelect mode:"));
#endif
      } else if (text[0] >= '1' && text[0] <= '3' && text[1] == '\0') {
        if (text[0] == '1') { active_mode = Mode::learning; learning_mode_begin(); }
        else if (text[0] == '2') { active_mode = Mode::test; test_mode_begin(); }
        else {
          active_mode = Mode::free_writing;
          Serial.println(F("\n===== FREE WRITING MODE =====\nEnter Braille dots, or M for menu."));
        }
      } else Serial.println(F("Invalid command. Select 1, 2, 3, or 4."));
      break;
    case Mode::learning: learning_mode_handle_line(text); break;
    case Mode::test: test_mode_handle_line(text, submitted_at); break;
    case Mode::free_writing: return true;
    case Mode::dfplayer:
      Serial.println(F("Press M for menu. Reset the board to retry the DFPlayer test."));
      break;
  }
  return false;
}

#include "buttons.h"

// Dots 1 to 6, then Mod and OK.
static const byte button_pins[] = {2, 3, 4, 9, 8, 7, 5, 6};
static const byte button_count = sizeof(button_pins) / sizeof(button_pins[0]);
static const unsigned long debounce_ms = 30;

static bool last_reading[button_count] = {};
static bool stable_pressed[button_count] = {};
static unsigned long last_change[button_count] = {};

void init_buttons() {
  for (byte index = 0; index < button_count; index++) {
    pinMode(button_pins[index], INPUT_PULLUP);
  }
}

bool button_pressed(byte index) {
  bool pressed = digitalRead(button_pins[index]) == LOW;
  unsigned long now = millis();

  if (pressed != last_reading[index]) {
    last_reading[index] = pressed;
    last_change[index] = now;
  }

  if (pressed != stable_pressed[index] && now - last_change[index] >= debounce_ms) {
    stable_pressed[index] = pressed;
    return pressed;
  }
  return false;
}

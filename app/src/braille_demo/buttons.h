#ifndef BRAILLE_BUTTONS_H
#define BRAILLE_BUTTONS_H

#include <Arduino.h>

const byte dot_count = 6;
const byte mode_button = 6;
const byte confirm_button = 7;

void init_buttons();
bool button_pressed(byte index);

#endif

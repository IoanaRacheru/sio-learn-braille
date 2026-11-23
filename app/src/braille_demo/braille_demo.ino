#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <SoftwareSerial.h>
#include "braille.h"
#include "buttons.h"

const byte oled_address = 0x3C;
const unsigned long baud_rate = 9600;

Adafruit_SSD1306 oled(128, 64, &Wire, -1);
SoftwareSerial bluetooth(A0, 10);  // Arduino RX, TX

bool oled_ready = false;

uint8_t selected_dots = 0;

void setup() {
  Serial.begin(baud_rate);
  bluetooth.begin(baud_rate);
  init_buttons();
  Wire.begin();
  Wire.setWireTimeout(25000, true);
  delay(500);

  Wire.beginTransmission(oled_address);
  if (Wire.endTransmission() == 0) {
    oled_ready = oled.begin(SSD1306_SWITCHCAPVCC, oled_address, false, false);
  }

  if (oled_ready) {
    oled.clearDisplay();
    oled.setTextColor(SSD1306_WHITE);
    oled.display();
  } else {
    Serial.println(F("OLED unavailable at 0x3C."));
  }

}

void show_letter(char letter) {
  oled.clearDisplay();
  oled.setTextSize(4);
  oled.setCursor(0, 0);
  oled.print(letter);
  oled.display();
}

void publish_letter(uint8_t dot_mask) {
  char letter = identify_letter(dot_mask);
  if (letter == '\0') {
    Serial.println(F("No matching letter A-Z."));
    return;
  }

  if (oled_ready) {
    show_letter(letter);
  }
  bluetooth.println(letter);
  Serial.print(F("Letter: "));
  Serial.println(letter);
}

void loop() {
  for (byte index = 0; index < dot_count; index++) {
    if (button_pressed(index)) {
      // Press the same button again to deselect its dot.
      byte dot_bit = 1U << index;
      selected_dots ^= dot_bit;
      Serial.print(F("Dot "));
      Serial.print(index + 1);
      Serial.println((selected_dots & dot_bit) ? F(" selected.") : F(" deselected."));
    }
  }

  if (button_pressed(mode_button)) {
    Serial.println(F("Mode button pressed."));
  }

  if (button_pressed(confirm_button)) {
    Serial.println(F("OK button pressed."));
    if (selected_dots != 0) {
      publish_letter(selected_dots);
      selected_dots = 0;
    } else {
      Serial.println(F("Select at least one dot."));
    }
  }

  if (Serial.available() == 0) {
    return;
  }

  String dots = Serial.readStringUntil('\n');
  dots.trim();

  uint8_t dot_mask = 0;
  if (read_dots(dots.c_str(), dot_mask) != input_status::valid) {
    Serial.println(F("Enter distinct dots from 1 to 6, e.g. 145."));
    return;
  }

  publish_letter(dot_mask);
}

#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <SoftwareSerial.h>
#include "braille.h"

const byte oled_address = 0x3C;
const unsigned long baud_rate = 9600;

Adafruit_SSD1306 oled(128, 64, &Wire, -1);
SoftwareSerial bluetooth(A0, 10);  // Arduino RX, TX
bool oled_ready = false;

void setup() {
  Serial.begin(baud_rate);
  bluetooth.begin(baud_rate);
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

void loop() {
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

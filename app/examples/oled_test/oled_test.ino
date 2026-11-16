#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

const uint8_t screen_width = 128;
const uint8_t screen_height = 64;
const uint8_t oled_address = 0x3C;
const int8_t reset_pin = -1;

Adafruit_SSD1306 oled(screen_width, screen_height, &Wire, reset_pin);

void setup() {
  Serial.begin(9600);
  Wire.begin();
  delay(1000);
  // Check that a device responds at the OLED address.
  Wire.beginTransmission(oled_address);
  if (Wire.endTransmission() != 0) {
    Serial.println(F("No I2C response at 0x3C."));
    return;
  }

  if (!oled.begin(SSD1306_SWITCHCAPVCC, oled_address)) {
    Serial.println(F("OLED initialization failed."));
    return;
  }

  oled.clearDisplay();
  oled.setTextColor(SSD1306_WHITE);
  oled.setTextSize(2);
  oled.setCursor(0, 10);
  oled.println(F("Learn"));
  oled.println(F("Braille"));
  oled.display(); // Send the text buffer to the screen.
  Serial.println(F("Text sent to OLED."));
}

void loop() {}

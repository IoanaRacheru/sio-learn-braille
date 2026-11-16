#include <SoftwareSerial.h>

const uint8_t bluetooth_rx_pin = A0;
const uint8_t bluetooth_tx_pin = 10;
const unsigned long baud_rate = 9600;

SoftwareSerial bluetooth(bluetooth_rx_pin, bluetooth_tx_pin);

void setup() {
  Serial.begin(baud_rate);
  bluetooth.begin(baud_rate);
  Serial.println(F("Bluetooth test"));
}

void loop() {
  if (bluetooth.available() > 0) {
    Serial.write(bluetooth.read());
  }
  if (Serial.available() > 0) {
    bluetooth.write(Serial.read());
  }
}

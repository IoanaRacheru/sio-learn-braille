#include "hardware_diagnostics.h"

#include <Arduino.h>
#include <Wire.h>
#include "dfplayer_test.h"

#if ENABLE_DFPLAYER_TEST
#include <DFRobotDFPlayerMini.h>
#include <SoftwareSerial.h>
#endif

namespace {
// The SSD1306 is configured in braille_demo and oled_test.
constexpr uint8_t oled_address = 0x3C;

void scan_i2c() {
  Serial.println(F("I2C scan: SDA=A4, SCL=A5 (Arduino UNO)."));
  Serial.println(F("SSD1306 OLED expected at 0x3C (separate demo sketch)."));
  Wire.begin();
  Wire.setWireTimeout(25000, true);

  bool oled_responded = false;
  bool scan_complete = true;
  for (uint8_t address = 0x08; address <= 0x77; ++address) {
    Wire.beginTransmission(address);
    const uint8_t result = Wire.endTransmission();
    if (result == 0) {
      Serial.print(F("I2C response at 0x"));
      Serial.println(address, HEX);
      if (address == oled_address) oled_responded = true;
    } else if (result != 2) { // 2 means the address was not acknowledged.
      Serial.print(F("I2C scan stopped on bus error "));
      Serial.println(result);
      scan_complete = false;
      break;
    }
  }
  Wire.end();

  Serial.print(F("OLED 0x3C: "));
  if (oled_responded) Serial.println(F("Address acknowledged; display operation not tested."));
  else if (scan_complete) Serial.println(F("No response."));
  else Serial.println(F("Not verified."));
}

void finish_report() {
  Serial.println(F("Audio playback: Not tested (speaker on DFPlayer SPK1/SPK2)."));
  Serial.println(F("--- DIAGNOSTICS COMPLETE ---"));
}

#if ENABLE_DFPLAYER_TEST
enum class Probe { complete, settling, waiting };
enum class Card { unknown, detected, removed };
Probe probe = Probe::complete;
Card card = Card::unknown;
unsigned long probe_started;
bool dfplayer_responded = false;

constexpr unsigned long power_up_ms = 1000;
constexpr unsigned long reply_timeout_ms = 500;
constexpr uint8_t query_volume = 0x43;
// The library's readVolume() waits synchronously. Send the same read-only
// command without ACK and use the existing library to validate and parse replies.
const uint8_t volume_request[] = {0x7E, 0xFF, 0x06, query_volume, 0, 0, 0, 0xFE, 0xB8, 0xEF};

void read_dfplayer_reply() {
  DFRobotDFPlayerMini& player = dfplayer_player();
  // begin() seeds an online type even with no device. Inspect the type only
  // when the parser has a new message, and ignore its local error types.
  if (!player.available()) return;
  const uint8_t type = player.readType();
  const uint8_t command = player.readCommand();
  const uint16_t value = player.read();
  if (type == DFPlayerCardOnline || type == DFPlayerCardInserted ||
      type == DFPlayerCardUSBOnline || (type == DFPlayerUSBOnline && (value & 0x02))) {
    card = Card::detected;
    dfplayer_responded = true;
  } else if (type == DFPlayerCardRemoved || (type == DFPlayerUSBRemoved && (value & 0x02))) {
    card = Card::removed;
    dfplayer_responded = true;
  } else if (type == DFPlayerUSBOnline || type == DFPlayerUSBInserted ||
             type == DFPlayerUSBRemoved || type == DFPlayerError ||
             (probe == Probe::waiting && type == DFPlayerFeedBack && command == query_volume && value <= 30)) {
    dfplayer_responded = true;
  }
}
#endif
}

void hardware_diagnostics_begin() {
  Serial.println(F("\n================================\nBRAILLE INTERACTIVE SYSTEM\n================================"));
  Serial.println(F("\nInitializing hardware...\n\n--- HARDWARE DIAGNOSTICS ---"));
  scan_i2c();
  Serial.println(F("Bluetooth (separate sketch): UART, Arduino RX=A0, TX=D10, 9600 baud."));
  Serial.println(F("Bluetooth communication: Not tested; Bluetooth UART inactive in this sketch."));
  Serial.println(F("Serial Monitor: Hardware UART via USB, RX=D0, TX=D1, 9600 baud."));
  Serial.println(F("\nDFPlayer Mini: UART, 9600 baud. No I2C address."));
  Serial.print(F("DFPlayer TX -> Arduino RX D"));
  Serial.println(dfplayer_rx_pin);
  Serial.print(F("DFPlayer RX <- Arduino TX D"));
  Serial.println(dfplayer_tx_pin);

#if ENABLE_DFPLAYER_TEST
  if (dfplayer_begin()) {
    Serial.println(F("Local UART/library initialization: OK."));
    probe_started = millis();
    probe = Probe::settling;
    return;
  }
  dfplayer_serial().stopListening();
  Serial.println(F("Local UART/library initialization: FAILED."));
#else
  Serial.println(F("Local UART/library initialization: Disabled by build configuration."));
#endif
  Serial.println(F("Communication: NOT VERIFIED\nmicroSD card: Unknown."));
  finish_report();
}

bool hardware_diagnostics_complete() {
#if ENABLE_DFPLAYER_TEST
  if (probe == Probe::complete) return true;
  read_dfplayer_reply();

  if (probe == Probe::settling) {
    if (millis() - probe_started < power_up_ms) return false;
    dfplayer_serial().write(volume_request, sizeof(volume_request));
    probe_started = millis();
    probe = Probe::waiting;
    return false;
  }
  if (millis() - probe_started < reply_timeout_ms) return false;

  dfplayer_serial().stopListening();
  if (dfplayer_responded) Serial.println(F("Communication: OK (valid DFPlayer UART response)."));
  else {
    Serial.println(F("Communication: NOT VERIFIED (no recognized UART response)."));
    Serial.println(F("Silence cannot distinguish a missing microSD card from a UART or module failure."));
  }
  switch (card) {
    case Card::detected: Serial.println(F("microSD card: Detected.")); break;
    case Card::removed: Serial.println(F("microSD card: Removal reported.")); break;
    case Card::unknown: Serial.println(F("microSD card: Unknown (no card status reported).")); break;
  }
  finish_report();
  probe = Probe::complete;
#endif
  return true;
}

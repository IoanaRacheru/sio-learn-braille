#include "dfplayer_test.h"

#if ENABLE_DFPLAYER_TEST
#include <Arduino.h>
#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

namespace {
enum class Stage { idle, booting, waiting_for_card, setting_volume, playing, failed };
Stage stage = Stage::idle;
unsigned long stage_started = 0;

// Construct only when explicitly started: merely linking this module does
// not configure pins or claim the SoftwareSerial receive interrupt.
SoftwareSerial& audio_serial() {
  static SoftwareSerial serial(11, 10); // Constructor order: RX, TX.
  return serial;
}
DFRobotDFPlayerMini& player() {
  static DFRobotDFPlayerMini instance;
  return instance;
}
void fail() {
  Serial.println(F("DFPlayer test stopped. Check 5V/GND, crossed RX/TX, and SD card; reset to retry."));
  audio_serial().end();
  stage = Stage::failed;
}
}

void dfplayer_test_begin() {
  if (stage != Stage::idle) return;
  Serial.begin(9600);
  Serial.println(F("DFPlayer test: RX=D11, TX=D10, debug/audio=9600 baud."));
  audio_serial().begin(9600);
  // No reset/wait here. This return value only confirms library setup,
  // NOT hardware presence. Require an actual card-online event below.
  if (!player().begin(audio_serial(), false, false)) {
    Serial.println(F("DFPlayer library initialization failed."));
    fail();
    return;
  }
  stage = Stage::booting;
  stage_started = millis();
  Serial.println(F("Waiting for power-up, then checking SD-card readiness..."));
}

void dfplayer_test_update() {
  if (stage == Stage::idle || stage == Stage::failed) return;
  if (stage == Stage::booting) {
    if (millis() - stage_started < 1000UL) return;
    player().reset(); // ACK disabled: no wait for a missing device.
    stage = Stage::waiting_for_card;
    stage_started = millis();
    return;
  }

  if (player().available()) {
    const uint8_t type = player().readType();
    const int value = player().read();
    switch (type) {
      case DFPlayerCardOnline:
        if (stage == Stage::waiting_for_card) {
          Serial.println(F("DFPlayer initialization successful: SD card online."));
          player().volume(10);
          Serial.println(F("Volume set to 10/30."));
          stage = Stage::setting_volume;
          stage_started = millis();
        }
        break;
      case DFPlayerCardInserted:
        Serial.println(F("SD card inserted; waiting for card-online confirmation."));
        break;
      case DFPlayerCardRemoved:
        Serial.println(F("DFPlayer error: SD card removed."));
        fail();
        return;
      case DFPlayerPlayFinished:
        Serial.print(F("Playback finished, track: "));
        Serial.println(value);
        break;
      case DFPlayerError:
        Serial.print(F("DFPlayer error code: "));
        Serial.println(value);
        Serial.println(F("1=busy/card missing, 2=sleeping, 3=serial frame, 4=checksum, 5=file index, 6=file mismatch, 7=advertise."));
        fail();
        return;
      case TimeOut:
      case WrongStack:
        Serial.println(F("DFPlayer communication error: timeout or malformed packet."));
        fail();
        return;
      default:
        Serial.print(F("DFPlayer event: "));
        Serial.print(type);
        Serial.print(F(" value: "));
        Serial.println(value);
        break;
    }
  }

  if (stage == Stage::waiting_for_card && millis() - stage_started >= 5000UL) {
    Serial.println(F("DFPlayer initialization failed: no SD-card-online reply within 5 seconds."));
    fail();
  } else if (stage == Stage::setting_volume && millis() - stage_started >= 100UL) {
    player().playMp3Folder(1);
    Serial.println(F("Play command sent for /mp3/0001.mp3. Listen for sound; command alone does not confirm playback."));
    stage = Stage::playing;
  }
}

#else
void dfplayer_test_begin() {}
void dfplayer_test_update() {}
#endif

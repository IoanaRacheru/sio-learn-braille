#ifndef DFPLAYER_TEST_H
#define DFPLAYER_TEST_H

// Enabled for normal Arduino IDE builds. build.ps1 also supplies this flag
// globally to every C++ translation unit. Set to 0 here to disable in the IDE.
#ifndef ENABLE_DFPLAYER_TEST
#define ENABLE_DFPLAYER_TEST 1
#endif

// Optional standalone sketch (copy this header and .cpp into its folder):
// #include "dfplayer_test.h"
// void setup() { dfplayer_test_begin(); }
// void loop() { dfplayer_test_update(); }
// Debug Serial: 9600 baud. SD file: /mp3/0001.mp3, FAT16/FAT32.
// UNO D10 (TX) -> 1 kohm -> DFPlayer RX; UNO D11 (RX) <- DFPlayer TX.
// Common GND, 5 V supply, speaker BETWEEN SPK1 and SPK2.
// Run separately from other SoftwareSerial receivers or SPI peripherals.
// No Serial input is consumed. Disabled builds need no DFPlayer library.
void dfplayer_test_begin();
void dfplayer_test_update();

#endif

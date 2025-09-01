/*!
 * @file vcom_test.ino
 *
 * Example for testing VCOM output on the Adafruit PCM51xx library
 *
 * This example demonstrates how to:
 * 1. Switch between VREF and VCOM modes
 * 2. Control VCOM power state
 * 3. Monitor the VCOM pin output
 *
 * Written by Limor 'ladyada' Fried with assistance from Claude Code
 * for Adafruit Industries.
 *
 * MIT license, all text here must be included in any redistribution.
 */

#include <Adafruit_PCM51xx.h>

Adafruit_PCM51xx pcm;

void setup() {
  Serial.begin(115200);
  while (!Serial)
    delay(10);

  Serial.println(F("Adafruit PCM51xx VCOM Test"));

  if (!pcm.begin()) {
    Serial.println(F("Could not find PCM51xx, check wiring!"));
    while (1)
      delay(10);
  }

  Serial.println(F("PCM51xx initialized successfully!"));
  Serial.println(F("Testing VCOM functionality..."));
  Serial.println();

  // Check initial state
  Serial.println(F("=== Initial State ==="));
  bool vcomMode = pcm.isVCOMEnabled();
  bool vcomPowered = pcm.isVCOMPowered();

  Serial.print(F("VCOM Mode: "));
  Serial.println(vcomMode ? F("VCOM") : F("VREF"));
  Serial.print(F("VCOM Power: "));
  Serial.println(vcomPowered ? F("ON") : F("OFF"));
  Serial.println();
}

void loop() {
  Serial.println(F("=== Testing VCOM Mode Switch ==="));

  // Test VREF mode (default)
  Serial.println(F("Switching to VREF mode..."));
  pcm.enableVCOM(false);
  delay(100);

  bool vcomMode = pcm.isVCOMEnabled();
  Serial.print(F("Current mode: "));
  Serial.println(vcomMode ? F("VCOM") : F("VREF"));
  Serial.println(F("VCOM pin should be tied to GND in VREF mode"));

  delay(3000);

  // Test VCOM mode
  Serial.println(F("Switching to VCOM mode..."));

  // First power on VCOM
  pcm.setVCOMPower(true);
  delay(100);

  // Then enable VCOM mode
  pcm.enableVCOM(true);
  delay(100);

  vcomMode = pcm.isVCOMEnabled();
  bool vcomPowered = pcm.isVCOMPowered();

  Serial.print(F("Current mode: "));
  Serial.println(vcomMode ? F("VCOM") : F("VREF"));
  Serial.print(F("VCOM Power: "));
  Serial.println(vcomPowered ? F("ON") : F("OFF"));
  Serial.println(F("VCOM pin should output common-mode voltage in VCOM mode"));

  delay(3000);

  // Test VCOM power control
  Serial.println(F("=== Testing VCOM Power Control ==="));

  Serial.println(F("Powering down VCOM..."));
  pcm.setVCOMPower(false);
  delay(100);

  vcomPowered = pcm.isVCOMPowered();
  Serial.print(F("VCOM Power: "));
  Serial.println(vcomPowered ? F("ON") : F("OFF"));
  Serial.println(F("VCOM output should be disabled"));

  delay(3000);

  Serial.println(F("Powering up VCOM..."));
  pcm.setVCOMPower(true);
  delay(100);

  vcomPowered = pcm.isVCOMPowered();
  Serial.print(F("VCOM Power: "));
  Serial.println(vcomPowered ? F("ON") : F("OFF"));
  Serial.println(F("VCOM output should be active"));

  delay(3000);
  Serial.println();
}
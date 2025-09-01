/*!
 * @file simpletest.ino
 *
 * Simple test example for the Adafruit PCM51xx library
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
  while (!Serial) delay(10);

  Serial.println(F("Adafruit PCM51xx Simple Test"));

  if (!pcm.begin()) {
    Serial.println(F("Could not find PCM51xx, check wiring!"));
    while (1) delay(10);
  }
  
  Serial.println(F("PCM51xx initialized successfully!"));

  // Set I2S bit width to 24-bit
  Serial.println(F("Setting I2S to 24-bit"));
  pcm.setI2SSize(PCM51XX_I2S_SIZE_24BIT);
  
  // Unmute the DAC
  Serial.println(F("Unmuting DAC"));
  pcm.mute(false);
  
  Serial.println(F("Ready to play audio!"));
}

void loop() {
  delay(1000);
}
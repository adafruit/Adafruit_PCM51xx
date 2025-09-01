/*!
 * @file gpio_test.ino
 *
 * Example for reading GPIO values from the Adafruit PCM51xx library
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

  Serial.println(F("Adafruit PCM51xx GPIO Test"));

  if (!pcm.begin()) {
    Serial.println(F("Could not find PCM51xx, check wiring!"));
    while (1) delay(10);
  }
  
  Serial.println(F("PCM51xx initialized successfully!"));
  Serial.println(F("Reading GPIO values..."));
  Serial.println();
}

void loop() {
  Serial.println(F("GPIO Pin States:"));
  
  for (uint8_t pin = 1; pin <= 6; pin++) {
    bool state = pcm.digitalRead(pin);
    Serial.print(F("GPIO"));
    Serial.print(pin);
    Serial.print(F(": "));
    Serial.println(state ? F("HIGH") : F("LOW"));
  }
  
  Serial.println();
  delay(2000);
}
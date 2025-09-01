/*!
 * @file gpio5_toggle.ino
 *
 * Simple GPIO5 toggle test for PCM51xx
 * 
 * This example sets GPIO5 to register output mode and toggles it high/low
 * every second to test manual GPIO control.
 */

#include <Adafruit_PCM51xx.h>

Adafruit_PCM51xx pcm;

void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10);

  Serial.println(F("PCM51xx GPIO5 Toggle Test"));

  if (!pcm.begin()) {
    Serial.println(F("Could not find PCM51xx, check wiring!"));
    while (1) delay(10);
  }
  
  Serial.println(F("PCM51xx initialized!"));

  // Set GPIO5 to register output mode
  if (!pcm.setGPIO5Output(PCM51XX_GPIO5_REGISTER_OUTPUT)) {
    Serial.println(F("Failed to set GPIO5 to register output mode"));
    while (1) delay(10);
  }
  
  // Set GPIO5 direction to output
  if (!pcm.setGPIODirection(5, true)) {
    Serial.println(F("Failed to set GPIO5 direction to output"));
    while (1) delay(10);
  }
  
  Serial.println(F("GPIO5 configured for manual toggle"));
  Serial.println(F("Connect scope/meter to GPIO5 pin to see toggling"));
}

void loop() {
  // Toggle GPIO5 high
  pcm.setGPIORegisterOutput(5, true);
  Serial.println(F("GPIO5 -> HIGH"));
  delay(1000);
  
  // Toggle GPIO5 low  
  pcm.setGPIORegisterOutput(5, false);
  Serial.println(F("GPIO5 -> LOW"));
  delay(1000);
}
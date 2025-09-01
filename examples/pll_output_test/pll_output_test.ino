/*!
 * @file pll_output_test.ino
 *
 * PLL output test on GPIO5 for PCM51xx
 * 
 * This example configures GPIO5 to output PLL/4 signal to verify
 * PLL functionality and MCLK-related clocking.
 */

#include <Adafruit_PCM51xx.h>

Adafruit_PCM51xx pcm;

void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10);

  Serial.println(F("PCM51xx PLL Output Test"));

  if (!pcm.begin()) {
    Serial.println(F("Could not find PCM51xx, check wiring!"));
    while (1) delay(10);
  }
  
  Serial.println(F("PCM51xx initialized!"));

  // Enable PLL first
  Serial.println(F("Enabling PLL..."));
  if (!pcm.enablePLL(true)) {
    Serial.println(F("Failed to enable PLL"));
    while (1) delay(10);
  }
  
  // Wait for PLL lock
  delay(100);
  bool pllLocked = pcm.isPLLLocked();
  Serial.print(F("PLL locked: "));
  Serial.println(pllLocked ? F("Yes") : F("No"));

  // Set GPIO5 direction to output
  if (!pcm.setGPIODirection(5, true)) {
    Serial.println(F("Failed to set GPIO5 direction to output"));
    while (1) delay(10);
  }
  
  // Set GPIO5 to output PLL/4 
  if (!pcm.setGPIO5Output(PCM51XX_GPIO5_PLL_OUT_DIV4)) {
    Serial.println(F("Failed to set GPIO5 to PLL output mode"));
    while (1) delay(10);
  }
  
  Serial.println(F("GPIO5 configured to output PLL/4"));
  Serial.println(F("Connect scope to GPIO5 pin to verify PLL clock output"));
  
  // Display current configuration
  pcm51xx_gpio5_output_t gpio5Mode = pcm.getGPIO5Output();
  Serial.print(F("GPIO5 output mode: "));
  Serial.println(gpio5Mode);
  
  Serial.print(F("PLL reference: "));
  pcm51xx_pll_ref_t pllRef = pcm.getPLLReference();
  switch (pllRef) {
    case PCM51XX_PLL_REF_SCK:
      Serial.println(F("SCK"));
      break;
    case PCM51XX_PLL_REF_BCK:
      Serial.println(F("BCK"));
      break;
    case PCM51XX_PLL_REF_GPIO:
      Serial.println(F("GPIO"));
      break;
    default:
      Serial.println(F("Unknown"));
      break;
  }
}

void loop() {
  // Check PLL status periodically
  static unsigned long lastCheck = 0;
  if (millis() - lastCheck > 5000) {
    lastCheck = millis();
    
    bool pllLocked = pcm.isPLLLocked();
    Serial.print(F("PLL locked: "));
    Serial.println(pllLocked ? F("Yes") : F("No"));
  }
  
  delay(100);
}
/*!
 * @file mclk_test.ino
 *
 * MCLK input test for PCM51xx
 * 
 * This example generates 8MHz clock on Arduino pin 9 using Timer1 and feeds it to 
 * PCM51xx MCLK/SCK input to test external master clock functionality.
 * 
 * Connect Arduino pin 9 to PCM51xx MCLK/SCK input pin.
 */

#include <Adafruit_PCM51xx.h>

Adafruit_PCM51xx pcm;

void setup8MHzClock() {
  // Configure Timer1 to generate 8MHz on pin 9 (OC1A)
  pinMode(9, OUTPUT);
  
  // Set Timer1 to CTC mode, toggle OC1A on compare match
  TCCR1A = (1 << COM1A0);  // Toggle OC1A on compare match
  TCCR1B = (1 << WGM12) | (1 << CS10);  // CTC mode, no prescaler
  
  // For 8MHz output with 16MHz system clock: OCR1A = (16MHz / (2 * 8MHz)) - 1 = 0
  OCR1A = 0;  // This gives us 8MHz toggle rate
}

void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10);

  Serial.println(F("PCM51xx MCLK Input Test"));

  // Generate 8MHz clock on pin 9
  Serial.println(F("Setting up 8MHz clock on pin 9"));
  Serial.println(F("Connect pin 9 to PCM51xx MCLK/SCK input"));
  setup8MHzClock();
  
  delay(100);  // Let clock stabilize

  if (!pcm.begin()) {
    Serial.println(F("Could not find PCM51xx, check wiring!"));
    while (1) delay(10);
  }
  
  Serial.println(F("PCM51xx initialized with defaults!"));

  // Configure to use SCK (MCLK) as PLL reference instead of BCK
  Serial.println(F("Setting PLL reference to SCK (MCLK input)"));
  if (!pcm.setPLLReference(PCM51XX_PLL_REF_SCK)) {
    Serial.println(F("Failed to set PLL reference to SCK"));
  }

  delay(200);  // Wait for PLL to lock with new reference
  
  // Check PLL lock status
  bool pllLocked = pcm.isPLLLocked();
  Serial.print(F("PLL locked with 8MHz MCLK: "));
  Serial.println(pllLocked ? F("Yes") : F("No"));

  // Set I2S bit width and unmute
  Serial.println(F("Setting I2S to 24-bit and unmuting"));
  pcm.setI2SSize(PCM51XX_I2S_SIZE_24BIT);
  pcm.mute(false);
  
  Serial.println(F("MCLK test complete - ready for audio with 8MHz master clock"));
}

void loop() {
  // Monitor PLL lock status
  static unsigned long lastCheck = 0;
  if (millis() - lastCheck > 3000) {
    lastCheck = millis();
    
    bool pllLocked = pcm.isPLLLocked();
    Serial.print(F("PLL: "));
    Serial.println(pllLocked ? F("LOCKED") : F("UNLOCKED"));
  }
  
  delay(100);
}
/*!
 * @file Adafruit_PCM51xx.cpp
 *
 * @mainpage Adafruit PCM51xx library
 *
 * @section intro_sec Introduction
 *
 * This is the library for the Adafruit PCM51xx breakout
 * ----> http://www.adafruit.com/
 *
 * Adafruit invests time and resources providing this open source code,
 * please support Adafruit and open-source hardware by purchasing
 * products from Adafruit!
 *
 * @section author Author
 *
 * Written by Limor 'ladyada' Fried with assistance from Claude Code
 * for Adafruit Industries.
 *
 * @section license License
 *
 * MIT license, all text here must be included in any redistribution.
 *
 */

#include "Adafruit_PCM51xx.h"

/*!
 * @brief Constructor for PCM51xx
 */
Adafruit_PCM51xx::Adafruit_PCM51xx(void) {}

/*!
 * @brief Destructor for PCM51xx
 */
Adafruit_PCM51xx::~Adafruit_PCM51xx(void) {
  if (i2c_dev) {
    delete i2c_dev;
  }
}

/*!
 * @brief Initialize the PCM512x
 * @param i2c_addr I2C address (default is PCM51XX_DEFAULT_ADDR)
 * @param wire Pointer to TwoWire instance (default is &Wire)
 * @return True if successful, false otherwise
 */
bool Adafruit_PCM51xx::begin(uint8_t i2c_addr, TwoWire *wire) {
  if (i2c_dev) {
    delete i2c_dev;
  }
  
  i2c_dev = new Adafruit_I2CDevice(i2c_addr, wire);
  
  if (!i2c_dev->begin()) {
    return false;
  }
  
  // Put device into standby before reset operations
  if (!standby(true)) {
    return false;
  }
  
  // Reset registers first
  if (!resetRegisters()) {
    return false;
  }
  
  // Reset modules
  if (!resetModules()) {
    return false;
  }
  
  // Make sure we're out of powerdown mode
  if (!powerdown(false)) {
    return false;
  }
  
  // Make sure we're out of standby mode
  if (!standby(false)) {
    return false;
  }
  
  return true;
}

/*!
 * @brief Reset modules (interpolation filter and DAC modules)
 * @return True if successful, false if timeout or error
 */
bool Adafruit_PCM51xx::resetModules(void) {
  if (!selectPage(0)) {
    return false;
  }
  
  Adafruit_BusIO_Register reset_reg = 
    Adafruit_BusIO_Register(i2c_dev, PCM51XX_REG_RESET, 1);
  Adafruit_BusIO_RegisterBits rstm_bit = 
    Adafruit_BusIO_RegisterBits(&reset_reg, 1, 4);

  // Set the RSTM bit to initiate reset
  if (!rstm_bit.write(1)) {
    return false;
  }

  // Wait for auto-clearing with timeout (max 100ms)
  uint32_t start = millis();
  while (millis() - start < 100) {
    if (rstm_bit.read() == 0) {
      return true; // Reset completed
    }
    delay(1);
  }
  
  return false; // Timeout
}

/*!
 * @brief Reset registers back to their initial values
 * @return True if successful, false if timeout or error
 */
bool Adafruit_PCM51xx::resetRegisters(void) {
  if (!selectPage(0)) {
    return false;
  }
  
  Adafruit_BusIO_Register reset_reg = 
    Adafruit_BusIO_Register(i2c_dev, PCM51XX_REG_RESET, 1);
  Adafruit_BusIO_RegisterBits rstr_bit = 
    Adafruit_BusIO_RegisterBits(&reset_reg, 1, 0);

  // Set the RSTR bit to initiate reset
  if (!rstr_bit.write(1)) {
    return false;
  }

  // Wait for auto-clearing with timeout (max 100ms)
  uint32_t start = millis();
  while (millis() - start < 100) {
    if (rstr_bit.read() == 0) {
      return true; // Reset completed
    }
    delay(1);
  }
  
  return false; // Timeout
}

/*!
 * @brief Set or clear standby mode
 * @param enable True to enter standby mode, false for normal operation
 * @return True if successful, false otherwise
 */
bool Adafruit_PCM51xx::standby(bool enable) {
  if (!selectPage(0)) {
    return false;
  }
  
  Adafruit_BusIO_Register standby_reg = 
    Adafruit_BusIO_Register(i2c_dev, PCM51XX_REG_STANDBY, 1);
  Adafruit_BusIO_RegisterBits rqst_bit = 
    Adafruit_BusIO_RegisterBits(&standby_reg, 1, 4);

  return rqst_bit.write(enable ? 1 : 0);
}

/*!
 * @brief Check if device is in standby mode
 * @return True if in standby mode, false otherwise
 */
bool Adafruit_PCM51xx::isStandby(void) {
  if (!selectPage(0)) {
    return false;
  }
  
  Adafruit_BusIO_Register standby_reg = 
    Adafruit_BusIO_Register(i2c_dev, PCM51XX_REG_STANDBY, 1);
  Adafruit_BusIO_RegisterBits rqst_bit = 
    Adafruit_BusIO_RegisterBits(&standby_reg, 1, 4);

  return rqst_bit.read() == 1;
}

/*!
 * @brief Set or clear powerdown mode
 * @param enable True to enter powerdown mode, false for normal operation
 * @return True if successful, false otherwise
 */
bool Adafruit_PCM51xx::powerdown(bool enable) {
  if (!selectPage(0)) {
    return false;
  }
  
  Adafruit_BusIO_Register standby_reg = 
    Adafruit_BusIO_Register(i2c_dev, PCM51XX_REG_STANDBY, 1);
  Adafruit_BusIO_RegisterBits rqpd_bit = 
    Adafruit_BusIO_RegisterBits(&standby_reg, 1, 0);

  return rqpd_bit.write(enable ? 1 : 0);
}

/*!
 * @brief Check if device is in powerdown mode
 * @return True if in powerdown mode, false otherwise
 */
bool Adafruit_PCM51xx::isPowerdown(void) {
  if (!selectPage(0)) {
    return false;
  }
  
  Adafruit_BusIO_Register standby_reg = 
    Adafruit_BusIO_Register(i2c_dev, PCM51XX_REG_STANDBY, 1);
  Adafruit_BusIO_RegisterBits rqpd_bit = 
    Adafruit_BusIO_RegisterBits(&standby_reg, 1, 0);

  return rqpd_bit.read() == 1;
}

/*!
 * @brief Set I2S data format
 * @param format I2S format to set
 * @return True if successful, false otherwise
 */
bool Adafruit_PCM51xx::setI2SFormat(pcm51xx_i2s_format_t format) {
  if (!selectPage(0)) {
    return false;
  }
  
  Adafruit_BusIO_Register i2s_reg = 
    Adafruit_BusIO_Register(i2c_dev, PCM51XX_REG_I2S_CONFIG, 1);
  Adafruit_BusIO_RegisterBits format_bits = 
    Adafruit_BusIO_RegisterBits(&i2s_reg, 2, 4);

  return format_bits.write((uint8_t)format);
}

/*!
 * @brief Get I2S data format
 * @return Current I2S format
 */
pcm51xx_i2s_format_t Adafruit_PCM51xx::getI2SFormat(void) {
  if (!selectPage(0)) {
    return PCM51XX_I2S_FORMAT_I2S;
  }
  
  Adafruit_BusIO_Register i2s_reg = 
    Adafruit_BusIO_Register(i2c_dev, PCM51XX_REG_I2S_CONFIG, 1);
  Adafruit_BusIO_RegisterBits format_bits = 
    Adafruit_BusIO_RegisterBits(&i2s_reg, 2, 4);

  return (pcm51xx_i2s_format_t)format_bits.read();
}

/*!
 * @brief Set I2S word length
 * @param size I2S word length to set
 * @return True if successful, false otherwise
 */
bool Adafruit_PCM51xx::setI2SSize(pcm51xx_i2s_size_t size) {
  if (!selectPage(0)) {
    return false;
  }
  
  Adafruit_BusIO_Register i2s_reg = 
    Adafruit_BusIO_Register(i2c_dev, PCM51XX_REG_I2S_CONFIG, 1);
  Adafruit_BusIO_RegisterBits size_bits = 
    Adafruit_BusIO_RegisterBits(&i2s_reg, 2, 0);

  return size_bits.write((uint8_t)size);
}

/*!
 * @brief Get I2S word length
 * @return Current I2S word length
 */
pcm51xx_i2s_size_t Adafruit_PCM51xx::getI2SSize(void) {
  if (!selectPage(0)) {
    return PCM51XX_I2S_SIZE_24BIT;
  }
  
  Adafruit_BusIO_Register i2s_reg = 
    Adafruit_BusIO_Register(i2c_dev, PCM51XX_REG_I2S_CONFIG, 1);
  Adafruit_BusIO_RegisterBits size_bits = 
    Adafruit_BusIO_RegisterBits(&i2s_reg, 2, 0);

  return (pcm51xx_i2s_size_t)size_bits.read();
}

/*!
 * @brief Set PLL reference clock source
 * @param ref PLL reference clock source to set
 * @return True if successful, false otherwise
 */
bool Adafruit_PCM51xx::setPLLReference(pcm51xx_pll_ref_t ref) {
  if (!selectPage(0)) {
    return false;
  }
  
  Adafruit_BusIO_Register pll_ref_reg = 
    Adafruit_BusIO_Register(i2c_dev, PCM51XX_REG_PLL_REF, 1);
  Adafruit_BusIO_RegisterBits ref_bits = 
    Adafruit_BusIO_RegisterBits(&pll_ref_reg, 3, 4);

  return ref_bits.write((uint8_t)ref);
}

/*!
 * @brief Get PLL reference clock source
 * @return Current PLL reference clock source
 */
pcm51xx_pll_ref_t Adafruit_PCM51xx::getPLLReference(void) {
  if (!selectPage(0)) {
    return PCM51XX_PLL_REF_SCK;
  }
  
  Adafruit_BusIO_Register pll_ref_reg = 
    Adafruit_BusIO_Register(i2c_dev, PCM51XX_REG_PLL_REF, 1);
  Adafruit_BusIO_RegisterBits ref_bits = 
    Adafruit_BusIO_RegisterBits(&pll_ref_reg, 3, 4);

  return (pcm51xx_pll_ref_t)ref_bits.read();
}

/*!
 * @brief Set volume in dB for both channels
 * @param leftDB Left channel volume in dB (-103.5 to 24.0 dB, 0.5dB steps)
 * @param rightDB Right channel volume in dB (-103.5 to 24.0 dB, 0.5dB steps)
 * @return True if successful, false otherwise
 */
bool Adafruit_PCM51xx::setVolumeDB(float leftDB, float rightDB) {
  if (!selectPage(0)) {
    return false;
  }
  
  // Convert dB to register values (0.5dB steps, 0x00 = 24dB, 0xFF = -103.5dB)
  // Formula: regVal = (24.0 - dB) / 0.5
  uint8_t leftVal = (uint8_t)constrain((24.0 - leftDB) / 0.5, 0, 255);
  uint8_t rightVal = (uint8_t)constrain((24.0 - rightDB) / 0.5, 0, 255);
  
  Adafruit_BusIO_Register left_reg = 
    Adafruit_BusIO_Register(i2c_dev, PCM51XX_REG_DIGITAL_VOLUME_L, 1);
  Adafruit_BusIO_Register right_reg = 
    Adafruit_BusIO_Register(i2c_dev, PCM51XX_REG_DIGITAL_VOLUME_R, 1);
  
  if (!left_reg.write(leftVal)) {
    return false;
  }
  
  return right_reg.write(rightVal);
}

/*!
 * @brief Get volume in dB for both channels
 * @param leftDB Pointer to store left channel volume in dB
 * @param rightDB Pointer to store right channel volume in dB
 */
void Adafruit_PCM51xx::getVolumeDB(float *leftDB, float *rightDB) {
  if (!selectPage(0)) {
    *leftDB = 0.0;
    *rightDB = 0.0;
    return;
  }
  
  Adafruit_BusIO_Register left_reg = 
    Adafruit_BusIO_Register(i2c_dev, PCM51XX_REG_DIGITAL_VOLUME_L, 1);
  Adafruit_BusIO_Register right_reg = 
    Adafruit_BusIO_Register(i2c_dev, PCM51XX_REG_DIGITAL_VOLUME_R, 1);
  
  uint8_t leftVal = left_reg.read();
  uint8_t rightVal = right_reg.read();
  
  // Convert register values back to dB
  // Formula: dB = 24.0 - (regVal * 0.5)
  *leftDB = 24.0 - (leftVal * 0.5);
  *rightDB = 24.0 - (rightVal * 0.5);
}

/*!
 * @brief Check if DSP boot is complete
 * @return True if DSP boot is complete, false otherwise
 */
bool Adafruit_PCM51xx::getDSPBootDone(void) {
  if (!selectPage(0)) {
    return false;
  }
  
  Adafruit_BusIO_Register power_state_reg = 
    Adafruit_BusIO_Register(i2c_dev, PCM51XX_REG_POWER_STATE, 1);
  Adafruit_BusIO_RegisterBits boot_done_bit = 
    Adafruit_BusIO_RegisterBits(&power_state_reg, 1, 7);

  return boot_done_bit.read() == 1;
}

/*!
 * @brief Get current power state
 * @return Current power state
 */
pcm51xx_power_state_t Adafruit_PCM51xx::getPowerState(void) {
  if (!selectPage(0)) {
    return PCM51XX_POWER_POWERDOWN;
  }
  
  Adafruit_BusIO_Register power_state_reg = 
    Adafruit_BusIO_Register(i2c_dev, PCM51XX_REG_POWER_STATE, 1);
  Adafruit_BusIO_RegisterBits power_state_bits = 
    Adafruit_BusIO_RegisterBits(&power_state_reg, 4, 0);

  return (pcm51xx_power_state_t)power_state_bits.read();
}

/*!
 * @brief Control FS detection ignore
 * @param ignore True to ignore FS detection, false to enable
 * @return True if successful, false otherwise
 */
bool Adafruit_PCM51xx::ignoreFSDetect(bool ignore) {
  if (!selectPage(0)) {
    return false;
  }
  
  Adafruit_BusIO_Register error_detect_reg = 
    Adafruit_BusIO_Register(i2c_dev, PCM51XX_REG_ERROR_DETECT, 1);
  Adafruit_BusIO_RegisterBits idfs_bit = 
    Adafruit_BusIO_RegisterBits(&error_detect_reg, 1, 6);

  return idfs_bit.write(ignore ? 1 : 0);
}

/*!
 * @brief Control BCK detection ignore
 * @param ignore True to ignore BCK detection, false to enable
 * @return True if successful, false otherwise
 */
bool Adafruit_PCM51xx::ignoreBCKDetect(bool ignore) {
  if (!selectPage(0)) {
    return false;
  }
  
  Adafruit_BusIO_Register error_detect_reg = 
    Adafruit_BusIO_Register(i2c_dev, PCM51XX_REG_ERROR_DETECT, 1);
  Adafruit_BusIO_RegisterBits idbk_bit = 
    Adafruit_BusIO_RegisterBits(&error_detect_reg, 1, 5);

  return idbk_bit.write(ignore ? 1 : 0);
}

/*!
 * @brief Control SCK detection ignore
 * @param ignore True to ignore SCK detection, false to enable
 * @return True if successful, false otherwise
 */
bool Adafruit_PCM51xx::ignoreSCKDetect(bool ignore) {
  if (!selectPage(0)) {
    return false;
  }
  
  Adafruit_BusIO_Register error_detect_reg = 
    Adafruit_BusIO_Register(i2c_dev, PCM51XX_REG_ERROR_DETECT, 1);
  Adafruit_BusIO_RegisterBits idsk_bit = 
    Adafruit_BusIO_RegisterBits(&error_detect_reg, 1, 4);

  return idsk_bit.write(ignore ? 1 : 0);
}

/*!
 * @brief Control clock halt detection ignore
 * @param ignore True to ignore clock halt detection, false to enable
 * @return True if successful, false otherwise
 */
bool Adafruit_PCM51xx::ignoreClockHalt(bool ignore) {
  if (!selectPage(0)) {
    return false;
  }
  
  Adafruit_BusIO_Register error_detect_reg = 
    Adafruit_BusIO_Register(i2c_dev, PCM51XX_REG_ERROR_DETECT, 1);
  Adafruit_BusIO_RegisterBits idch_bit = 
    Adafruit_BusIO_RegisterBits(&error_detect_reg, 1, 3);

  return idch_bit.write(ignore ? 1 : 0);
}

/*!
 * @brief Control LRCK/BCK missing detection ignore
 * @param ignore True to ignore LRCK/BCK missing detection, false to enable
 * @return True if successful, false otherwise
 */
bool Adafruit_PCM51xx::ignoreClockMissing(bool ignore) {
  if (!selectPage(0)) {
    return false;
  }
  
  Adafruit_BusIO_Register error_detect_reg = 
    Adafruit_BusIO_Register(i2c_dev, PCM51XX_REG_ERROR_DETECT, 1);
  Adafruit_BusIO_RegisterBits idcm_bit = 
    Adafruit_BusIO_RegisterBits(&error_detect_reg, 1, 2);

  return idcm_bit.write(ignore ? 1 : 0);
}

/*!
 * @brief Control clock divider autoset mode
 * @param disable True to disable clock auto set, false to enable
 * @return True if successful, false otherwise
 */
bool Adafruit_PCM51xx::disableClockAutoset(bool disable) {
  if (!selectPage(0)) {
    return false;
  }
  
  Adafruit_BusIO_Register error_detect_reg = 
    Adafruit_BusIO_Register(i2c_dev, PCM51XX_REG_ERROR_DETECT, 1);
  Adafruit_BusIO_RegisterBits dcas_bit = 
    Adafruit_BusIO_RegisterBits(&error_detect_reg, 1, 1);

  return dcas_bit.write(disable ? 1 : 0);
}

/*!
 * @brief Control PLL unlock detection ignore
 * @param ignore True to ignore PLL unlock detection, false to enable
 * @return True if successful, false otherwise
 */
bool Adafruit_PCM51xx::ignorePLLUnlock(bool ignore) {
  if (!selectPage(0)) {
    return false;
  }
  
  Adafruit_BusIO_Register error_detect_reg = 
    Adafruit_BusIO_Register(i2c_dev, PCM51XX_REG_ERROR_DETECT, 1);
  Adafruit_BusIO_RegisterBits iplk_bit = 
    Adafruit_BusIO_RegisterBits(&error_detect_reg, 1, 0);

  return iplk_bit.write(ignore ? 1 : 0);
}

/*!
 * @brief Set DAC clock source
 * @param source DAC clock source to set
 * @return True if successful, false otherwise
 */
bool Adafruit_PCM51xx::setDACSource(pcm51xx_dac_clk_src_t source) {
  if (!selectPage(0)) {
    return false;
  }
  
  Adafruit_BusIO_Register dac_clk_reg = 
    Adafruit_BusIO_Register(i2c_dev, PCM51XX_REG_DAC_CLK_SRC, 1);
  Adafruit_BusIO_RegisterBits sdac_bits = 
    Adafruit_BusIO_RegisterBits(&dac_clk_reg, 3, 4);

  return sdac_bits.write((uint8_t)source);
}

/*!
 * @brief Get DAC clock source
 * @return Current DAC clock source
 */
pcm51xx_dac_clk_src_t Adafruit_PCM51xx::getDACSource(void) {
  if (!selectPage(0)) {
    return PCM51XX_DAC_CLK_MASTER;
  }
  
  Adafruit_BusIO_Register dac_clk_reg = 
    Adafruit_BusIO_Register(i2c_dev, PCM51XX_REG_DAC_CLK_SRC, 1);
  Adafruit_BusIO_RegisterBits sdac_bits = 
    Adafruit_BusIO_RegisterBits(&dac_clk_reg, 3, 4);

  return (pcm51xx_dac_clk_src_t)sdac_bits.read();
}

/*!
 * @brief Set auto mute enable
 * @param enable True to enable auto mute, false to disable
 * @return True if successful, false otherwise
 */
bool Adafruit_PCM51xx::setAutoMute(bool enable) {
  if (!selectPage(0)) {
    return false;
  }
  
  Adafruit_BusIO_Register auto_mute_reg = 
    Adafruit_BusIO_Register(i2c_dev, PCM51XX_REG_AUTO_MUTE, 1);
  Adafruit_BusIO_RegisterBits auto_mute_bits = 
    Adafruit_BusIO_RegisterBits(&auto_mute_reg, 3, 0);

  return auto_mute_bits.write(enable ? 0x7 : 0x0);
}

/*!
 * @brief Get auto mute status
 * @return True if auto mute is enabled, false otherwise
 */
bool Adafruit_PCM51xx::getAutoMute(void) {
  if (!selectPage(0)) {
    return false;
  }
  
  Adafruit_BusIO_Register auto_mute_reg = 
    Adafruit_BusIO_Register(i2c_dev, PCM51XX_REG_AUTO_MUTE, 1);
  Adafruit_BusIO_RegisterBits auto_mute_bits = 
    Adafruit_BusIO_RegisterBits(&auto_mute_reg, 3, 0);

  uint8_t value = auto_mute_bits.read();
  return (value == 0x7);
}

/*!
 * @brief Set GPIO5 output selection
 * @param output GPIO5 output signal to select
 * @return True if successful, false otherwise
 */
bool Adafruit_PCM51xx::setGPIO5Output(pcm51xx_gpio5_output_t output) {
  if (!selectPage(0)) {
    return false;
  }
  
  Adafruit_BusIO_Register gpio5_reg = 
    Adafruit_BusIO_Register(i2c_dev, PCM51XX_REG_GPIO5_OUTPUT, 1);
  Adafruit_BusIO_RegisterBits gpio5_bits = 
    Adafruit_BusIO_RegisterBits(&gpio5_reg, 5, 0);

  return gpio5_bits.write((uint8_t)output);
}

/*!
 * @brief Get GPIO5 output selection
 * @return Current GPIO5 output selection
 */
pcm51xx_gpio5_output_t Adafruit_PCM51xx::getGPIO5Output(void) {
  if (!selectPage(0)) {
    return PCM51XX_GPIO5_OFF;
  }
  
  Adafruit_BusIO_Register gpio5_reg = 
    Adafruit_BusIO_Register(i2c_dev, PCM51XX_REG_GPIO5_OUTPUT, 1);
  Adafruit_BusIO_RegisterBits gpio5_bits = 
    Adafruit_BusIO_RegisterBits(&gpio5_reg, 5, 0);

  return (pcm51xx_gpio5_output_t)gpio5_bits.read();
}

/*!
 * @brief Set GPIO direction (input/output)
 * @param gpio GPIO number (1-6)
 * @param output True for output, false for input
 * @return True if successful, false otherwise
 */
bool Adafruit_PCM51xx::setGPIODirection(uint8_t gpio, bool output) {
  if (gpio < 1 || gpio > 6) {
    return false;
  }
  
  if (!selectPage(0)) {
    return false;
  }
  
  Adafruit_BusIO_Register gpio_enable_reg = 
    Adafruit_BusIO_Register(i2c_dev, PCM51XX_REG_GPIO_ENABLE, 1);
  Adafruit_BusIO_RegisterBits gpio_bit = 
    Adafruit_BusIO_RegisterBits(&gpio_enable_reg, 1, gpio - 1);

  return gpio_bit.write(output ? 1 : 0);
}

/*!
 * @brief Select register page
 * @param page Page number to select (0-255)
 * @return True if successful, false otherwise
 */
bool Adafruit_PCM51xx::selectPage(uint8_t page) {
  Adafruit_BusIO_Register page_reg = 
    Adafruit_BusIO_Register(i2c_dev, PCM51XX_REG_PAGE_SELECT, 1);
  
  return page_reg.write(page);
}
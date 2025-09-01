/*!
 * @file Adafruit_PCM51xx.h
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

#ifndef _ADAFRUIT_PCM51XX_H
#define _ADAFRUIT_PCM51XX_H

#include <Adafruit_BusIO_Register.h>
#include <Adafruit_I2CDevice.h>
#include <Adafruit_SPIDevice.h>
#include <Wire.h>
#include <SPI.h>

#include "Arduino.h"

/*! @brief Default I2C address for the PCM51xx chip */
#define PCM51XX_DEFAULT_ADDR 0x4C

/*! @brief I2S Data Format */
typedef enum {
  PCM51XX_I2S_FORMAT_I2S = 0, ///< I2S format
  PCM51XX_I2S_FORMAT_TDM = 1, ///< TDM/DSP format
  PCM51XX_I2S_FORMAT_RTJ = 2, ///< Right justified format
  PCM51XX_I2S_FORMAT_LTJ = 3  ///< Left justified format
} pcm51xx_i2s_format_t;

/*! @brief I2S Word Length */
typedef enum {
  PCM51XX_I2S_SIZE_16BIT = 0, ///< 16 bits
  PCM51XX_I2S_SIZE_20BIT = 1, ///< 20 bits
  PCM51XX_I2S_SIZE_24BIT = 2, ///< 24 bits
  PCM51XX_I2S_SIZE_32BIT = 3  ///< 32 bits
} pcm51xx_i2s_size_t;

/*! @brief PLL Reference Clock Source */
typedef enum {
  PCM51XX_PLL_REF_SCK = 0, ///< SCK clock
  PCM51XX_PLL_REF_BCK = 1, ///< BCK clock
  PCM51XX_PLL_REF_GPIO = 3 ///< GPIO clock
} pcm51xx_pll_ref_t;

/*! @brief Power State (Read Only) */
typedef enum {
  PCM51XX_POWER_POWERDOWN = 0,        ///< Powerdown
  PCM51XX_POWER_WAIT_CP_VALID = 1,    ///< Wait for CP voltage valid
  PCM51XX_POWER_CALIBRATION_1 = 2,    ///< Calibration
  PCM51XX_POWER_CALIBRATION_2 = 3,    ///< Calibration
  PCM51XX_POWER_VOLUME_RAMP_UP = 4,   ///< Volume ramp up
  PCM51XX_POWER_RUN_PLAYING = 5,      ///< Run (Playing)
  PCM51XX_POWER_LINE_SHORT = 6,       ///< Line output short / Low impedance
  PCM51XX_POWER_VOLUME_RAMP_DOWN = 7, ///< Volume ramp down
  PCM51XX_POWER_STANDBY = 8           ///< Standby
} pcm51xx_power_state_t;

/*! @brief DAC Clock Source */
typedef enum {
  PCM51XX_DAC_CLK_MASTER = 0, ///< Master clock (PLL/SCK and OSC auto-select)
  PCM51XX_DAC_CLK_PLL = 1,    ///< PLL clock
  PCM51XX_DAC_CLK_SCK = 3,    ///< SCK clock
  PCM51XX_DAC_CLK_BCK = 4     ///< BCK clock
} pcm51xx_dac_clk_src_t;

/*! @brief GPIO5 Output Selection */
typedef enum {
  PCM51XX_GPIO5_OFF = 0x00,             ///< Off (low)
  PCM51XX_GPIO5_DSP_OUTPUT = 0x01,      ///< DSP GPIO5 output
  PCM51XX_GPIO5_REGISTER_OUTPUT = 0x02, ///< Register GPIO5 output
  PCM51XX_GPIO5_AUTO_MUTE_FLAG = 0x03,  ///< Auto mute flag (both channels)
  PCM51XX_GPIO5_AUTO_MUTE_L = 0x04,     ///< Auto mute flag left channel
  PCM51XX_GPIO5_AUTO_MUTE_R = 0x05,     ///< Auto mute flag right channel
  PCM51XX_GPIO5_CLOCK_INVALID = 0x06,   ///< Clock invalid flag
  PCM51XX_GPIO5_SDOUT = 0x07,           ///< Serial audio interface data output
  PCM51XX_GPIO5_ANALOG_MUTE_L = 0x08,   ///< Analog mute flag left (low active)
  PCM51XX_GPIO5_ANALOG_MUTE_R = 0x09,   ///< Analog mute flag right (low active)
  PCM51XX_GPIO5_PLL_LOCK = 0x0A,        ///< PLL lock flag
  PCM51XX_GPIO5_CHARGE_PUMP_CLK = 0x0B, ///< Charge pump clock
  PCM51XX_GPIO5_UNDER_VOLT_07 = 0x0E,   ///< Under voltage flag (0.7 DVDD)
  PCM51XX_GPIO5_UNDER_VOLT_03 = 0x0F,   ///< Under voltage flag (0.3 DVDD)
  PCM51XX_GPIO5_PLL_OUT_DIV4 = 0x10     ///< PLL Output/4 (requires Clock Flex)
} pcm51xx_gpio5_output_t;

/*! @brief Page 0 Register Addresses */
#define PCM51XX_REG_PAGE_SELECT 0x00        ///< Page select register
#define PCM51XX_REG_RESET 0x01              ///< Reset register
#define PCM51XX_REG_STANDBY 0x02            ///< Standby and powerdown requests
#define PCM51XX_REG_MUTE 0x03               ///< Mute control
#define PCM51XX_REG_PLL 0x04                ///< PLL enable and lock status
#define PCM51XX_REG_SPI_MISO 0x06           ///< SPI MISO function select
#define PCM51XX_REG_DEEMPHASIS 0x07         ///< De-emphasis and SDOUT select
#define PCM51XX_REG_GPIO_ENABLE 0x08        ///< GPIO output enables
#define PCM51XX_REG_BCK_LRCLK 0x09          ///< BCK and LRCLK configuration
#define PCM51XX_REG_DSP_GPIO 0x0A           ///< DSP GPIO input
#define PCM51XX_REG_MASTER_MODE_RST 0x0C    ///< Master mode BCK/LRCLK reset
#define PCM51XX_REG_PLL_REF 0x0D            ///< PLL clock source select
#define PCM51XX_REG_DAC_CLK_SRC 0x0E        ///< DAC clock source
#define PCM51XX_REG_GPIO_PLL_REF 0x12       ///< GPIO source for PLL reference
#define PCM51XX_REG_SYNC_REQ 0x13           ///< Sync request
#define PCM51XX_REG_PLL_P 0x14              ///< PLL P divider
#define PCM51XX_REG_PLL_J 0x15              ///< PLL J divider
#define PCM51XX_REG_PLL_D_MSB 0x16          ///< PLL D divider MSB
#define PCM51XX_REG_PLL_D_LSB 0x17          ///< PLL D divider LSB
#define PCM51XX_REG_PLL_R 0x18              ///< PLL R divider
#define PCM51XX_REG_DSP_CLK_DIV 0x1B        ///< DSP clock divider
#define PCM51XX_REG_DAC_CLK_DIV 0x1C        ///< DAC clock divider
#define PCM51XX_REG_NCP_CLK_DIV 0x1D        ///< NCP clock divider
#define PCM51XX_REG_OSR_CLK_DIV 0x1E        ///< OSR clock divider
#define PCM51XX_REG_MASTER_BCK_DIV 0x20     ///< Master mode BCK divider
#define PCM51XX_REG_MASTER_LRCK_DIV 0x21    ///< Master mode LRCLK divider
#define PCM51XX_REG_FS_SPEED 0x22           ///< FS speed mode
#define PCM51XX_REG_IDAC_MSB 0x23           ///< IDAC MSB
#define PCM51XX_REG_IDAC_LSB 0x24           ///< IDAC LSB
#define PCM51XX_REG_ERROR_DETECT 0x25       ///< Error detection control
#define PCM51XX_REG_I2S_CONFIG 0x28         ///< I2S configuration
#define PCM51XX_REG_I2S_OFFSET 0x29         ///< I2S offset
#define PCM51XX_REG_I2S_UPSAMPLE 0x2A       ///< I2S upsampling
#define PCM51XX_REG_DSP_PROGRAM 0x2B        ///< DSP program selection
#define PCM51XX_REG_CLK_MISSING 0x2C        ///< Clock missing detection
#define PCM51XX_REG_AUTO_MUTE_TIME 0x3B     ///< Auto mute time
#define PCM51XX_REG_DIGITAL_VOLUME_CTL 0x3C ///< Digital volume control
#define PCM51XX_REG_DIGITAL_VOLUME_L 0x3D   ///< Digital volume left channel
#define PCM51XX_REG_DIGITAL_VOLUME_R 0x3E   ///< Digital volume right channel
#define PCM51XX_REG_VOLUME_FADE 0x3F        ///< Volume fade control
#define PCM51XX_REG_VOLUME_FADE_EMRG 0x40   ///< Emergency volume fade
#define PCM51XX_REG_AUTO_MUTE 0x41          ///< Auto mute control
#define PCM51XX_REG_GPIO1_OUTPUT 0x50       ///< GPIO1 output selection
#define PCM51XX_REG_GPIO2_OUTPUT 0x51       ///< GPIO2 output selection
#define PCM51XX_REG_GPIO3_OUTPUT 0x52       ///< GPIO3 output selection
#define PCM51XX_REG_GPIO4_OUTPUT 0x53       ///< GPIO4 output selection
#define PCM51XX_REG_GPIO5_OUTPUT 0x54       ///< GPIO5 output selection
#define PCM51XX_REG_GPIO6_OUTPUT 0x55       ///< GPIO6 output selection
#define PCM51XX_REG_GPIO_CONTROL 0x56       ///< GPIO control
#define PCM51XX_REG_GPIO_INVERT 0x57        ///< GPIO invert
#define PCM51XX_REG_DSP_OVERFLOW 0x5A       ///< DSP overflow flags
#define PCM51XX_REG_RATE_DETECT_1 0x5B      ///< Sample rate detection 1
#define PCM51XX_REG_RATE_DETECT_2 0x5C      ///< Sample rate detection 2
#define PCM51XX_REG_RATE_DETECT_3 0x5D      ///< Sample rate detection 3
#define PCM51XX_REG_RATE_DETECT_4 0x5E      ///< Sample rate detection 4
#define PCM51XX_REG_CLOCK_STATUS 0x5F       ///< Clock status
#define PCM51XX_REG_ANALOG_MUTE 0x6C        ///< Analog mute monitor
#define PCM51XX_REG_POWER_STATE 0x76        ///< Power state and DSP boot status
#define PCM51XX_REG_GPIO_INPUT 0x77         ///< GPIO input
#define PCM51XX_REG_AUTO_MUTE_FLAG 0x78     ///< Auto mute flags

/*! @brief Page 1 Register Addresses */
#define PCM51XX_REG_PAGE1_OUTPUT_AMP_TYPE 0x01 ///< Output amplitude type (OSEL)
#define PCM51XX_REG_PAGE1_VCOM_POWER 0x09      ///< VCOM power control (VCPD)

/*!
 * @brief  PCM51xx class
 */
class Adafruit_PCM51xx {
 public:
  Adafruit_PCM51xx(void);
  ~Adafruit_PCM51xx(void);

  bool begin(uint8_t i2c_addr = PCM51XX_DEFAULT_ADDR, TwoWire* wire = &Wire);
  bool begin(int8_t cs_pin, SPIClass* theSPI);
  bool begin(int8_t cs_pin, int8_t mosi_pin, int8_t miso_pin, int8_t sclk_pin);

  bool resetModules(void);
  bool resetRegisters(void);

  bool standby(bool enable);
  bool isStandby(void);
  bool powerdown(bool enable);
  bool isPowerdown(void);

  bool setI2SFormat(pcm51xx_i2s_format_t format);
  pcm51xx_i2s_format_t getI2SFormat(void);
  bool setI2SSize(pcm51xx_i2s_size_t size);
  pcm51xx_i2s_size_t getI2SSize(void);

  bool setPLLReference(pcm51xx_pll_ref_t ref);
  pcm51xx_pll_ref_t getPLLReference(void);

  bool setVolumeDB(float leftDB, float rightDB);
  void getVolumeDB(float* leftDB, float* rightDB);

  bool getDSPBootDone(void);
  pcm51xx_power_state_t getPowerState(void);

  bool ignoreFSDetect(bool ignore);
  bool ignoreBCKDetect(bool ignore);
  bool ignoreSCKDetect(bool ignore);
  bool ignoreClockHalt(bool ignore);
  bool ignoreClockMissing(bool ignore);
  bool disableClockAutoset(bool disable);
  bool ignorePLLUnlock(bool ignore);

  bool setDACSource(pcm51xx_dac_clk_src_t source);
  pcm51xx_dac_clk_src_t getDACSource(void);

  bool setAutoMute(bool enable);
  bool getAutoMute(void);

  bool mute(bool enable);
  bool isMuted(void);

  bool enablePLL(bool enable);
  bool isPLLEnabled(void);
  bool isPLLLocked(void);

  bool enableDeemphasis(bool enable);
  bool isDeemphasized(void);

  bool digitalRead(uint8_t pin);

  bool enableVCOM(bool enable);
  bool isVCOMEnabled(void);
  bool setVCOMPower(bool enable);
  bool isVCOMPowered(void);

  bool setGPIO5Output(pcm51xx_gpio5_output_t output);
  pcm51xx_gpio5_output_t getGPIO5Output(void);
  bool setGPIODirection(uint8_t gpio, bool output);
  bool setGPIORegisterOutput(uint8_t gpio, bool high);

 private:
  bool selectPage(uint8_t page);
  bool _init(void);
  Adafruit_I2CDevice* i2c_dev; ///< Pointer to I2C bus interface
  Adafruit_SPIDevice* spi_dev; ///< Pointer to SPI bus interface
  uint8_t _page;               ///< Current selected page (cached)
};

#endif
# Adafruit PCM51xx [![Build Status](https://github.com/adafruit/Adafruit_PCM51xx/actions/workflows/githubci.yml/badge.svg)](https://github.com/adafruit/Adafruit_PCM51xx/actions/workflows/githubci.yml) [![Documentation](https://github.com/adafruit/ci-arduino/blob/master/assets/doxygen_badge.svg)](http://adafruit.github.io/Adafruit_PCM51xx/html/index.html)

Arduino library for the Texas Instruments PCM51xx series of stereo audio DACs with I2C control interface.

This PCM51xx uses I2C to communicate, 2 pins are required to interface.

Adafruit invests time and resources providing this open source code, please support Adafruit and open-source hardware by purchasing products from Adafruit!

## About the PCM51xx

The PCM51xx is a high-performance stereo digital-to-analog converter (DAC) from Texas Instruments. Key features include:

* **High-Quality Audio**: Up to 32-bit/384kHz audio processing
* **Flexible I/O**: I2S, TDM, and other digital audio formats  
* **Built-in DSP**: Digital signal processing capabilities
* **Multiple Clock Sources**: PLL, SCK, BCK options
* **Volume Control**: Digital volume control with 0.5dB steps
* **GPIO Outputs**: Configurable GPIO pins for status and clock outputs
* **Low Power**: Multiple power-saving modes

## Installation

To install, use the Arduino Library Manager and search for "Adafruit PCM51xx" and install the library.

## Dependencies
 * [Adafruit BusIO](https://github.com/adafruit/Adafruit_BusIO)

## Contributing

Contributions are welcome! Please read our [Code of Conduct](https://github.com/adafruit/Adafruit_PCM51xx/blob/main/CODE_OF_CONDUCT.md)
before contributing to help this project stay welcoming.

## Documentation and doxygen
Documentation is produced by doxygen. Contributions should include documentation for any new features that they add.

Some examples of how to use the library are included in the examples/ folder.

## About this Driver
Written by Limor 'ladyada' Fried with assistance from Claude Code for Adafruit Industries.
MIT license, check LICENSE for more information.
All text above must be included in any redistribution.
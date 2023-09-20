## PlatformIO Configuration


Sub-directories contain two sample PlatformIO configuration files using the TFT_eSPI library to drive a TFT display and the TFT_eTouch library to handle a touch controller where the latter is connected to a SPI peripheral different from the one connected to the display driver chip.

### ESP32_2432S028

The [platformio.ini](ESP32_2432S028/platformio.ini) file in that directory is suitable for the ESP32_2432S028R (aka ESP32_2432S028, aka [cheap yellow display](https://github.com/witnessmenow/ESP32-Cheap-Yellow-Display)) development board. It has an XPT2046 touch controller which is connected to a second SPI peripheral. The board has an XPT2046 touch controller connected to the second ESP32 SPI peripheral (hSPI) but it uses a non standard I/O pins for the SPI bus.

There should be no need to change the default assignment of GPIO pins shown in the table below.

| Macro | ESP32 GPIO Pin Number |
| --- | :---: |
| TFT_ETOUCH_CS | 33 |
| TFT_ETOUCH_PIRQ | 36  |
| TFT_ETOUCH_SCK | 25 |
| TFT_ETOUCH_MISO | 39 |
| TFT_ETOUCH_MOSI | 32 |

A value assigned to any one of these macros in `platformio.ini` will have precedence over the default value.


### SECOND_SPI_PORT

The [platformio.ini](SECOND_SPI_PORT/platformio.ini) file in that directory is suitable for an ESP32 based board with a TFT display compatible with TFT_eSPI and with a touch controller connected to the second ESP32 SPI peripheral (hSPI) that is handled by TFT_eTouch. 

The default assignment of GPIO pins shown in the table below assumes that hSPI is multiplexed onto the standard ESP32 pins and that no GPIO is connected to the touch controller interrupt request line.

| Macro | ESP32 GPIO Pin Number |
| --- | :---: |
| TFT_ETOUCH_CS | 3 |
| TFT_ETOUCH_PIRQ | 255 |
| TFT_ETOUCH_SCK | 14 |
| TFT_ETOUCH_MISO | 12 |
| TFT_ETOUCH_MOSI | 13 |

A value assigned to any one of these macros in `platformio.ini` will have precedence over the default value.

**Note**: in the sample [platformio.ini](SECOND_SPI_PORT/platformio.ini) file, the default values were changed to correspond to the pins used on the ESP32_2432S028 for testing purposes. 

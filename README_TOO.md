File: README_TOO.md

# Added Configuration Options

Table of contents

<!-- TOC depthFrom:2 orderedList:false -->

- [1. Purpose](#1-purpose)
- [2. Some Background](#2-some-background)
- [3. On Using TFT_eTouch with TFT_eSPI](#3-on-using-tft_etouch-with-tft_espi)
- [4. Support for Touch Controllers Connected to a Second SPI Peripheral](#4-support-for-touch-controllers-connected-to-a-second-spi-peripheral)
- [5. Support for the ESP32_2432S028 Dev Board](#5-support-for-the-esp32_2432s028-dev-board)
- [6. Configuring TFT_eTouch in the Arduino IDE](#6-configuring-tft_etouch-in-the-arduino-ide)
- [7. Configuring TFT_eTouch in PlaformIO](#7-configuring-tft_etouch-in-plaformio)
- [8. Setup and Using TFT_eTouch](#8-setup-and-using-tft_etouch)
- [9. Examples](#9-examples)

<!-- /TOC -->

## 1. Purpose

This [fork](https://github.com/sigmdel/TFT_eTouch) of [TFT_eTouch](https://github.com/achillhasler/TFT_eTouch) adds options to simplify the use of displays that have a touch controller and a display driver that are connected to different SPI peripherals. It is also possible to use the PlatformIO configuration file `platformio.ini` to modify the user definable macros on a local basis without editing the `TFT_eTouchUser.h` in the TFT_eTouch library.

## 2. Some Background

Following a [suggestion](https://github.com/Bodmer/TFT_eSPI/issues/287#issuecomment-455572468) by bodmer (author of [TFT_eSPI](https://github.com/Bodmer/TFT_eSPI)), TFT_eTouch was created by Achill Hasler about four years ago (2019) as a replacement for the touch handler code in TFT_eSPI.  However, as of now, the built-in touch handler remains in place.

## 3. On Using TFT_eTouch with TFT_eSPI

To use TFT_eTouch instead of the built-in touch driver found in TFT_eSPI,

  1. **ensure that `TOUCH_CS` is NOT defined in the TFT_eSPI user configuration file** `User_Setup.h`, and

  1. **do not set** `Compiler warnings` **at levels above** `Default`.

>If using TFT_eTouch and if `Compiler warnings` are set to `More` or `All` in the Arduino IDE `Preference`, a sketch will not compile. This is because TFT_eTouchGesture will generate warnings of the type `variable 'xneg' set but not used` which are treated as errors. 

## 4. Support for Touch Controllers Connected to a Second SPI Peripheral

The default touch code in TFT_eSPI assumes that the touch controller and the display driver are both connected to the same SPI peripheral. That is not always the case. TFT_eTouch can handle a touch controller that is connected to the same SPI peripheral as the display driver or to a different peripheral. In the latter case, the macro `SECOND_SPI_PORT` should be defined and default the following pin assignments will be made.

| Identifier | ESP32 GPIO Pin Number |
| --- | :---: |
| TFT_ETOUCH_CS | 3 |
| TFT_ETOUCH_PIRQ | 255  |
| TFT_ETOUCH_SCK | 14 |
| TFT_ETOUCH_MISO | 12 |
| TFT_ETOUCH_MOSI | 13 |

These macro definitions can be overridden.

Note that setting `TFT_ETOUCH_MOSI` to 255 means that the interrupt signal from the touch controller is not connected to a GPIO pin or if it is that interrupts will not be enabled on the pin.


## 5. Support for the ESP32_2432S028 Dev Board 

This board ( aka [cheap yellow display](https://github.com/witnessmenow/ESP32-Cheap-Yellow-Display)), uses an XPT2046 touch controller connected to SPI peripheral different from the one used for the ILI9341 TFT driver chip. When using this board the macro
`ESP32_2432S028R` should be defined.

This will in turn define `SECOND_SPI_PORT` but the pin assignment will be different:

| Identifier | ESP32 GPIO Pin Number |
| --- | :---: |
| TFT_ETOUCH_CS | 33 |
| TFT_ETOUCH_PIRQ | 36  |
| TFT_ETOUCH_SCK | 25 |
| TFT_ETOUCH_MISO | 39 |
| TFT_ETOUCH_MOSI | 32 |

These default macro definitions can be overridden but it should not be necessary to adjust these values.

## 6. Configuring TFT_eTouch in the Arduino IDE

The new macros discussed above are added at the top of [TFT_eTouchUser.h](TFT_eTouchUser.h). 

```cpp
/** @def SECOND_SPI_PORT
 * Define if the touch controller uses separate SPI peripheral than TFT controller.
 *
 * Default value of
 */
//#define SECOND_SPI_PORT

/** @def ESP32_2432S028R
 * Define if an ESP32-2432S028R dev board (a.k.a "cheap yellow display") is being used.
 * Non default pins are used with its second SPI peripheral (hSPI) connected to the touch controller
 */
//#define ESP32_2432S028R

#if defined(SECOND_SPI_PORT) || defined(ESP32_2432S028R)

/** @def TFT_ETOUCH_SCK
 * Override default GPIO pin connected to second spi clk signal if not equal to default
 */
//#define TFT_ETOUCH_SCK

/** @def TFT_ETOUCH_MISO
 * Override default GPIO pin connected to second spi clk signal if not equal to default
 */
//#define TFT_ETOUCH_MISO

/** @def TFT_ETOUCH_MOSI
 * Override default GPIO pin connected to second spi clk signal if not equal to default
 */
//#define TFT_ETOUCH_MOSI

#endif
```

As can be seen, none are defined by default so that TFT_eTouch remains as before until either `ECOND_SPI_PORT` or `ESP32_2432S028R` is defined.

The [arduino](arduino/README.md) directory contains modified user configuration files for the TFT_eSPI and TFT_eTouch libraries suitable for the ESP32_2432S028R development board.

## 7. Configuring TFT_eTouch in PlaformIO

The TFT_eTouch configuration file [TFT_eTouchUser.h](TFT_eTouchUser.h) can be edited as done in the Arduino IDE. However, it is also  possible, and arguably better, to use the usual (local) configuration file `platformio.ini` to define the new macros listed above and the other macros in [TFT_eTouchUser.h](TFT_eTouchUser.h). Do not forget to add the following build flag


```ini
build_flags =
...
  -DTFT_ETOUCHUSER_LOADED ; TFT_eTouch configuration done here and not in TFT_eTouchUser.h
...  
```
to ensure that [TFT_eTouchUser.h](TFT_eTouchUser.h) will not be read.

Implementing this functionality required changes very much in the spirit of the TFT_eSPI library. Specifically, a part [TFT_eTouchUser.h](TFT_eTouchUser.h) has been moved to [TFT_ETouch_Setup_Select.h](TFT_ETouch_Setup_Select.h) so that the now smaller [TFT_eTouchUser.h](TFT_eTouchUser.h) contains only user settable macros. [TFT_eTouchBase.h](TFT_eTouchBase.h) no longer includes [TFT_eTouchUser.h](TFT_eTouchUser.h) directly but instead includes [TFT_eTouch_Setup_Select.h](TFT_eTouch_Setup_Select.h) The latter will include [TFT_eTouchUser.h](TFT_eTouchUser.h) if the `TFT_ETOUCHUSER_LOADED` macro is not defined. 

Example configuration files can be found in the [platformio](platformio/README.md) directory.

## 8. Setup and Using TFT_eTouch

Here is a typical setup and usage of the TFT_eTouch library in a sketch.

```cpp
#include <FS.h>
#include <SPI.h>
#include <TFT_eSPI.h> // Hardware-specific library
#include <TFT_eTouch.h>

TFT_eSPI tft;         // Invoke custom library

#ifdef SECOND_SPI_PORT
  SPIClass hSPI(HSPI);
  TFT_eTouch<TFT_eSPI> touch(tft, TFT_ETOUCH_CS, TFT_ETOUCH_PIRQ, hSPI); 
#else
  TFT_eTouch<TFT_eSPI> touch(tft, TFT_ETOUCH_CS, TFT_ETOUCH_PIRQ, TFT_eSPI::getSPIinstance());
#endif

void setup(void) {
  Serial.begin(115200);
  //Set up the display
  tft.init();
  tft.setRotation(1); // landscape 

  #ifdef SECOND_SPI_PORT
    hSPI.begin(TFT_ETOUCH_SCK, TFT_ETOUCH_MISO, TFT_ETOUCH_MOSI, TFT_ETOUCH_CS);
  #endif
  touch.init();
...
}

void loop(void) {
  // Pressed will be set true is there is a valid touch on the screen
  int16_t t_x = 0, t_y = 0; // To store the touch coordinates
  bool pressed = touch.getXY(t_x, t_y);
... 
}
```

However, with conditional defines, it is possible to write code that could be used with the touch code handler built-in to TFT_eSPI or with the TFT_eTouch library.

```cpp
#include <FS.h>
#include <SPI.h>
#include <TFT_eSPI.h> // Hardware-specific library
#ifndef TOUCH_CS
#include <TFT_eTouch.h>
#endif

TFT_eSPI tft;         // Invoke custom library

#ifndef TOUCH_CS      // Invoke TFT_eTouch 
  #ifdef SECOND_SPI_PORT
    SPIClass hSPI(HSPI);
    TFT_eTouch<TFT_eSPI> touch(tft, TFT_ETOUCH_CS, TFT_ETOUCH_PIRQ, hSPI); 
  #else
    TFT_eTouch<TFT_eSPI> touch(tft, TFT_ETOUCH_CS, TFT_ETOUCH_PIRQ, TFT_eSPI::getSPIinstance());
  #endif
#endif

void setup(void) {
  Serial.begin(115200);
  //Set up the display
  tft.init();
  tft.setRotation(1); // landscape 

  #ifndef TOUCH_CS  // Using TFT_eTouch
    #ifdef SECOND_SPI_PORT
      hSPI.begin(TFT_ETOUCH_SCK, TFT_ETOUCH_MISO, TFT_ETOUCH_MOSI, TFT_ETOUCH_CS);
    #endif
    touch.init();
  #endif
...
}

void loop(void) {
  // Pressed will be set true is there is a valid touch on the screen
  #ifdef TOUCH_CS
  uint16_t t_x = 0, t_y = 0; // To store the touch coordinates
  bool pressed = tft.getTouch(&t_x, &t_y);
  #else
  int16_t t_x = 0, t_y = 0; // To store the touch coordinates
  bool pressed = touch.getXY(t_x, t_y);
  #endif
... 
}
```

## 9. Examples

All the example Arduino sketches from the original TFT_eTouch library have been modified to work with this fork. Other examples have been added.

It is fairly easy to setup all the Arduino sketches at once.

  1. Create a directory named `TFT_eTouch_test`
  2. Create a subdirectory named `libraries`
  1. Clone [TFT_eSPI](https://github.com/Bodmer/TFT_eSPI) and this [fork of TFT_eTouch](https://github.com/sigmdel/TFT_eTouch) or download their ZIP archives and extract their content to two subdirectories of `TFT_eTouch_test/libraries`.
  1. Copy each example Arduino sketch from the TFT_eTouch `examples/generic` and `examples/TFT_eSPI` directories into `TFT_eTouch_test`.
  ```txt
        TFT_eTouch_test
        ├── calibrate
        │   └── calibrate.ino
        ├── edit_calibation
        │   ├── edit_calibation.ino
        │   ├── MenuCounter.cpp
        │   ├── MenuCounter.h
        │   ├── TMenu.cpp
        │   └── TMenu.h
        ├── raw
        │   └── raw.ino
        ├── Conways_Life
        │   └── Conways_Life.ino
        ├── Keypad_240x320
        │   └── Keypad_240x320.ino
        └── libraries
           ├── TFT_eSPI
           └── TFT_eTouch
```
  5. Edit the configurations files `User_Setup.h` and `TFT_eTouchUser.h` in TFT_eSPI and TFT_eTouch according to the hardware used. If an ESP32_2432S028R (aka cheap yellow display) development board
is used, just copy the files in the [arduino](arduino/README.md) directory over the corresponding files in the libraries.
  1. Enter the full path of the `TFT_eTouch_test` directory in the **Sketchbook location:** field in the **Settings** tab of the **Preferences** window in the Arduino IDE. It would be wise to first save the location somewhere to restore its content after running the examples.
  2. Open any one of the example sketches in the IDE, compile and upload to the microcontroller.

There is a PlatformIO project among the examples. It is in the `cydMacroPad` directory. There is no need to manually download the needed libraries. The dependancy is specified in `platformio.ini` and the PlatformIO will automatically download them. If an ESP32_2432S028R dev board is used, the project will compile as is. If different hardware is use, it will be necessary to edit `platformio.ini`, but there should be no need to edit the libraries.

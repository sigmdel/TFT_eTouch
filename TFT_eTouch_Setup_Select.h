#pragma once

//
//  TFT_eTouch_Setup_Select.h
//
// Pulls user touch controller configuration from platformio.ini
// or TFT_eTouchUser.h and then completes the configuration
//
// To use the configuration in platformio.ini, add
//   -DTFT_ETOUCHUSER_LOADED
// in platformio.ini.
//
// If the Arduino IDE is used, platformio.ini is ignored even if present
// meaning TFT_ETOUCHUSER_LOADED will not be defined and, automatically,
// TFT_eTouchUser.h will be included.
//
// Added 2023-09-18 Sigmdel, see README_TOO.md
//

#ifndef TFT_ETOUCHUSER_LOADED // Settings alreay defined in tft_touch_setup.h or platformio.ini platformio.ini

#include "TFT_eTouchUser.h"

#endif


// include used TFT driver
#if 0 // using Adafuit lib ILI9341
#include <Adafruit_ILI9341.h>
typedef Adafruit_ILI9341 TFT_Driver;

# define TFT_DC 4
# define TFT_CS 3
# define TFT_RST 17  // on SoftwareReset use -1, and connect pin to 3.3V

#elif defined (TEENSYDUINO) // with ILI9341
# include <ILI9341_t3.h>    // Hardware-specific TFT library
typedef ILI9341_t3 TFT_Driver;

# define TFT_DC      20  // Data Command control pin
# define TFT_CS      21  // Chip select pin
# define TFT_RST      2  // on SoftwareReset use 255, and connect pin to 3.3V
# define TFT_MOSI     7  // MasterOut SlaveIn pin (DOUT)
# define TFT_SCLK    14  // SPI clock (SCK)
# define TFT_MISO    12  // MasterIn SlaveOut pin (DIN)
# define TFT_BL       3  // Backlight pin must have PWM for analogWrite if set

#else // ESP_PLATFORM with eSPI
#include <TFT_eSPI.h>      // Hardware-specific TFT library
typedef TFT_eSPI TFT_Driver;
// manage your TFT setup in TFT_eSPI UserSetup.h

# ifdef TOUCH_CS
#error undef TOUCH_CS in TFT_eSPI UserSetup.h for using TFT_eTouch
# endif

#endif // ESP_PLATFORM with eSPI


// Set default pin assignements for touch controller if needed

#ifndef TFT_ETOUCH_CS
  #if defined(ESP32_2432S028R)
    #define TFT_ETOUCH_CS 33
  #elif defined(SECOND_SPI_PORT)
    #define TFT_ETOUCH_CS 3
  #else
    #error TFT_ETOUCH_CS is not defined
  #endif
#endif

#ifndef TFT_ETOUCH_PIRQ
  #if defined(ESP32_2432S028R)
    #define TFT_ETOUCH_PIRQ 36
  #else
    #define TFT_ETOUCH_PIRQ 255
  #endif
#endif

#if (TFT_ETOUCH_PIRQ != 255)
  // if pin is set and valid we have to use penirq code
  #define TOUCH_USE_PENIRQ_CODE
#else
  #undef TOUCH_USE_PENIRQ_CODE
#endif


#if defined(ESP32_2432S028R) && !defined(SECOND_SPI_PORT)
  #define SECOND_SPI_PORT
#endif

#if defined(SECOND_SPI_PORT)
  #if !defined(TFT_ETOUCH_SCK)
    #if defined(ESP32_2432S028R)
      #define TFT_ETOUCH_SCK 25
    #else
      #define TFT_ETOUCH_SCK 14
    #endif
  #endif
  #if !defined(TFT_ETOUCH_MISO)
    #if defined(ESP32_2432S028R)
      #define TFT_ETOUCH_MISO 39
    #else
      #define TFT_ETOUCH_MISO 12
    #endif
  #endif
  #if !defined(TFT_ETOUCH_MOSI)
    #if defined(ESP32_2432S028R)
      #define TFT_ETOUCH_MOSI 32
    #else
      #define TFT_ETOUCH_MOSI 13
    #endif
  #endif
#endif

#ifndef TOUCH_FILTER_TYPE
  #undef TOUCH_X_FILTER
  #undef TOUCH_Y_FILTER
  #undef TOUCH_Z_FILTER
#else
  #include <TFT_eFirFilter.h>
  #ifndef TOUCH_X_FILTER
    #define TOUCH_X_FILTER FirFilter<20>
  #endif
  #ifndef TOUCH_Y_FILTER
    #define TOUCH_Y_FILTER TOUCH_X_FILETER
  #endif
#endif

#ifndef TOUCH_DEFAULT_CALIBRATION
#define TOUCH_DEFAULT_CALIBRATION { 300, 3700, 300, 3700, 2 }
#endif

#if defined (_ILI9341_t3H_) || defined (_ADAFRUIT_ILI9341H_)
// color used by TFT_eTouch
#define TFT_BLACK ILI9341_BLACK
#define TFT_BLUE  ILI9341_BLUE
#define TFT_GREEN ILI9341_GREEN
#define TFT_RED   ILI9341_RED
#define TFT_WHITE ILI9341_WHITE

// missing setTextFont, drawString
#ifndef BASIC_FONT_SUPPORT
#define BASIC_FONT_SUPPORT
#endif
#endif

#ifdef DOXYGEN
// we set all for getting documentation
#define TOUCH_USE_PENIRQ_CODE
#define TOUCH_USE_AVERAGING_CODE
#define TOUCH_USE_USER_CALIBRATION
#define TOUCH_USE_SIMPE_TARGET
#define TOUCH_USE_GESTURE
#define TOUCH_USE_DIFFERENTIAL_MEASURE
#define BASIC_FONT_SUPPORT
#define TOUCH_SERIAL_DEBUG
#define TOUCH_SERIAL_CONVERSATION_TIME
#define TOUCH_SERIAL_DEBUG_FETCH
#define TOUCH_FILTER_TYPE
#define ESP32_2432S028R
#define SECOND_SPI_PORT
#endif

#ifndef TFT_E_TOUCH_USER_H
#define TFT_E_TOUCH_USER_H

//
//  TFT_eTouchUser.h
//
//  (C) Copyright Achill Hasler 2019.
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file at https://www.boost.org/LICENSE_1_0.txt
//
//
//  See TFT_eTouch/docs/html/index.html for documentation.
//

// Modified 2023-09-18 Sigmdel, see README_TOO.md

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


/** @def TFT_ETOUCH_CS
 * Define the chip select pin for the touch chip.
 */
#if defined (TEENSYDUINO) // with ILI9341
# define TFT_ETOUCH_CS 15
#elif defined (ESP8266) // with ILI9341
# define TFT_ETOUCH_CS D1 // Chip select pin (T_CS)
#elif defined (ESP32_2432S028R)
# define TFT_ETOUCH_CS 33
#elif 1
# define TFT_ETOUCH_CS 3
#else
# define TFT_ETOUCH_CS SS
#endif

/** @def TFT_ETOUCH_PIRQ
 * Define the irq pin for the touch chip.
 */
#if 0
# define TFT_ETOUCH_PIRQ 6
#elif 0 //defined (ESP8266) // with ILI9341
# define TFT_ETOUCH_PIRQ D0 // is touched signal pin (T_IRQ) is high when touched
#elif defined (ESP32_2432S028R)
# define TFT_ETOUCH_PIRQ 36
#elif 0
# define TFT_ETOUCH_PIRQ 1
#else
# define TFT_ETOUCH_PIRQ 255
#endif

/** @def TOUCH_USE_AVERAGING_CODE
 * If this defined is set the averaging option is available.
 */
// undefine this to save progmem if you not using averaging
#define TOUCH_USE_AVERAGING_CODE

/** @def TOUCH_USE_USER_CALIBRATION
 * If this defined is set the member function getUserCalibration() is available.
 */
// undefine this to save progmem if you don't use getUserCalibration() anymore
#define TOUCH_USE_USER_CALIBRATION

/** @def TOUCH_USE_GESTURE
 * If this defined is set the gesture interface is available. (work in progress)
 */
// undefine this to save progmem if you don't use gesture interface anymore
//#define TOUCH_USE_GESTURE

/** @def TOUCH_USE_SIMPE_TARGET
 * If this defined is set the function getUserCalibration() show simple target.
 */
// define this to save progmem, simple target wont write outside dispay coordinates (undefine it looks nicer, bud can core on some dispay drivers)
#define TOUCH_USE_SIMPE_TARGET

/** @def TOUCH_USE_DIFFERENTIAL_MEASURE
 * If this defined is set the 'Differential Measure' mode is used. (SER/DFR low)
 * When undefined 'Single Ended Measure' mode is active. (SER/DFR high)
 */
#define TOUCH_USE_DIFFERENTIAL_MEASURE

/** @def TOUCH_DEFAULT_CALIBRATION
 * This is the used touch configuration. If it's match to your configuration, you can disable TOUCH_USE_USER_CALIBRATION.
 */
#define TOUCH_DEFAULT_CALIBRATION { 272, 3749, 3894, 341, 0 }

/** @def TOUCH_FILTER_TYPE
 * If this defined is set the touch driver filter raw data with a fir filter,
 * define additional TOUCH_X_FILTER, TOUCH_Y_FILTER, TOUCH_Z_FILTER for the value to filter
 * Window type:
 * - 1: Hamming
 * - 2: Hanning
 * - 3: Blackmann
 * If TOUCH_FILTER_TYPE is defined undefine a filter or change N, 
 *   N must be even (default 12), T must be uint16_t which is default
 */
//#define TOUCH_FILTER_TYPE 1
//#define TOUCH_X_FILTER FirFilter<20>
//#define TOUCH_Y_FILTER TOUCH_X_FILTER
//#define TOUCH_Z_FILTER FirFilter<>

/** @def BASIC_FONT_SUPPORT
 * If this defined is set the tft driver lacks setTextFont() and drawString().
 */
//#define BASIC_FONT_SUPPORT

/** @def TOUCH_SERIAL_DEBUG
 * If this defined is set the library show info to Serial.
 */
// define this to see additional output on Serial
#define TOUCH_SERIAL_DEBUG

/** @def TOUCH_SERIAL_CONVERSATION_TIME
 * If this defined is set the library show time of one conversation in microsecond to Serial.
 * When TOUCH_SERIAL_DEBUG_FETCH is defined you got additional time from the print call!
 * When defined, getUserCalibration() won't work.
 */
// define this to see converation time on Serial for fetching raw values
//#define TOUCH_SERIAL_CONVERSATION_TIME

/** @def TOUCH_SERIAL_DEBUG_FETCH
 * If this defined is set the library show ctrl command of invalid read measure to Serial.
 */
// define this to see if X, Y, Z1 or Z2 measure is out of range when not touched
//#define TOUCH_SERIAL_DEBUG_FETCH

#endif // TFT_E_TOUCH_USER_H

#ifndef TFT_E_TOUCH_BASE_H
#define TFT_E_TOUCH_BASE_H

//
//  TFT_eTouchBase.h
//
//  (C) Copyright Achill Hasler 2019.
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file at https://www.boost.org/LICENSE_1_0.txt
//
//
//  See TFT_eTouch/docs/html/index.html for documentation.
//

#include <Arduino.h>
#include <SPI.h>

#if ARDUINO < 10600
#error "Arduino 1.6.0 or later (SPI library) is required"
#endif

#define TFT_ETOUCH_VERSION "0.6.0"

#if 0

While __has_include(<tft_touch_setup.h) does work with PlatformIO, the 
equivalent code in TFT_eSPI (with tft_setup_h) does not work and is 
deprecated.

 See [Deprecated due to a number of issues with this approach] User setup in the sketch #2007 
      https://github.com/Bodmer/TFT_eSPI/discussions/2007

 and Remove all references to tft_setup.h and Sketch_with_tft_setup from code #2746 
     https://github.com/Bodmer/TFT_eSPI/issues/2746

Consequently this is disabled
 
// The following lines 'borrowed' from TFT_eSPI should allow the user
// setup to be included in the sketch folder. Unfortunately it does
// not work in the Arduino IDE
//
#if defined __has_include && !defined TFT_ETOUCHUSER_LOADED
#  if __has_include(<tft_touch_setup.h>)
#    include <tft_touch_setup.h>
#    define TFT_ETOUCHUSER_LOADED  // Prevent loading further setups
// else
//   using TFT_eTouchUser.h settings
#  endif
#elif !defined __has_include
#  warning Compiler does not support __has_include, so sketches cannot define the setup
#endif
#endif

// finish loading settings
#include <TFT_eTouch_Setup_Select.h>

#ifdef TOUCH_USE_GESTURE
class TFT_eTouchGesture;
#endif

/**
  * @brief touch support base
  */
class TFT_eTouchBase
{
public:
/**
  * These are the calibration values for the touchscreen.
  *
  * When x0 < x1 the touch x axis is in same direction as the tft axis.
  *
  * when y1 < y0 the touch y axis is in oposite direction as the tft axis.
  *
  * The calibration values { 250, 3800, 260, 3850, 2 } and { 3800, 250, 3850, 260, 0 } are identical.
  * @brief touch calibation
  */
  struct Calibation
  {
    uint16_t x0; ///< x touch value of first display pixel
    uint16_t x1; ///< x touch value of last display pixel
    uint16_t y0; ///< y touch value of first display pixel
    uint16_t y1; ///< y touch value of last display pixel

    uint8_t rel_rotation;  ///< relative rotation clockwise from display to touch
  };

/**
  * This struct hold one raw touch measure.
  *
  * @brief touch raw value
  */
  struct Measure
  {
    uint16_t    x; ///< raw x measure range 0..4095 (12 bit) valid (ca 213..3881)
    uint16_t    y; ///< raw y measure range 0..4095 (12 bit) valid (ca 222..3929)
    uint16_t    z1; ///< raw z1 measure range 0..4095 (12 bit) valid (ca 29..1748)
    uint16_t    z2; ///< raw z2 measure range 0..4095 (12 bit) valid (ca 963..3989)
    uint16_t    rz; ///< calculated resitor value from raw x, z1 and z2 measure, value range (0..rx_plate_*10) when tuched, 0xffff when not tuched.
    Measure() : rz(0xffff) {}
  };

/**
  * This struct hold one touch measure with tft coordinates.
  *
  * @brief tft touch value
  */
  struct TouchPoint
  {
    int16_t x; ///< TFT x position
    int16_t y; ///< TFT y position
    uint16_t rz; ///< resitance of Touch in ohm

    TouchPoint() : x(0), y(0), rz(0xffff) {}
    /// set display position and pressure
    inline void set(int16_t _x, int16_t _y, uint16_t _rz)
    { x = _x; y = _y; rz = _rz; }
  };

/**
  * Create instance with defaults.
  *
  * Display and touch must use the same spi bus.
  * @brief constructor
  * @param cs_pin touch chip select line processor pin
  * @param penirq_pin touch penirq line processor pin. 0xff disable touch interrupt
  * @param spi used bus
  */
              TFT_eTouchBase(uint8_t cs_pin, uint8_t penirq_pin = 0xff, SPIClass& spi = SPI);

/**
  * Initialize the processor cs pin.
  * @brief  init cs pin
  * @param spi_init init spi bus to with default MOSI MISO and SCK
  */
	void        init(bool spi_init);

/**
  * Get raw position of touch. These values are in orientation of the touchscreen.
  *
  * The values x, y, z1, z2 and rz are only set if the function returns true.
  * @brief  touch position
  * @param x touch coordinate
  * @param y touch coordinate
  * @param z1 touch value for calculating rz
  * @param z2 touch value for calculating rz
  * @param rz calculated pressure in ohm
  * @return true when display is tuched
  */
  bool        getRaw(uint16_t& x, uint16_t& y, uint16_t& z1, uint16_t& z2, uint16_t& rz);

/**
  * Get raw position of touch. These values are in orientation of the touchscreen.
  * With TFT_eTouch<T>::transform() you get the display coordinates from the raw measure.
  * The value is only set if the function returns true.
  * @brief  raw touch position
  * @param raw value to set
  * @return true when display is tuched
  */
  bool        getRaw(Measure& raw);

/**
  * Wait in this function as long touchscreen is touched. If you must know the touch coordinates while waiting pen up,
  * use following code:
@code
while (touch.getXY(x, y)) {
  // do something with display coordinates
  delay(touch.getMeasureWait() - time used);
}
// or
while (touch.getRaw(x, y, z1, z2, rz)) {
  // do something with raw touch coordinates
  delay(touch.getMeasureWait() - time used);
}
@endcode
  * @brief  wait for pen up
  */
  void        waitPenUp();

/**
  * Get last calculated RZ value. When display not touched the value is 0xffff.
  * As more finger touch the display as lower is this value
  * @brief  last pressure
  * @return last RZ measure in ohm
  */
  inline uint16_t getRZ() const;

/**
  * Set the calibration for this touchscreen.
  * @brief  set calibration
  * @param data touch calibration for display
  */
  inline void setCalibration(const Calibation& data);

/**
  * Get the calibration for this touchscreen.
  * @brief  get calibration
  * @return data touch calibration for display
  */
  inline Calibation& calibration();

/**
  * Read the calibration for this touchscreen from SPIFLASH or EPROM.
  * @brief  read calibration
  * @param descr filename in flash or adr in eprom
  * @return true when data was readed, otherwise data is invalid
  */
  bool        readCalibration(const char* descr);

/**
  * Write the actual calibration for this touchscreen to SPIFLASH or EPROM.
  * @brief  write calibration
  * @param descr filename in flash or adr in eprom
  * @return true when data was written, otherwise calibration is not stored
  */
  bool        writeCalibration(const char* descr);

/**
  * Set the measure strategie.
  *
  * Base decicion
  * - do i wait until measure is same as measure bevore?<br>
  * This is the Figure 10 implmentation from sbaa036.pdf. Set count = 0
  * - when not take the n'th measurement?<br>
  * This is the Figure 11 implmentation from sbaa036.pdf. Set setAveraging(false) & count = n (0<n<256) to take the n'th measurement.
  * - averaging count measurement?<br>
  * Set averaging=true & count = n (0<n<15) to number of measurement to average.
  * If ignore_min_max=true, do two measurement more for ignore min and max measure.
  * - start measure with X or Z1?<br>
  * When measure of X gives a value in_range() bud Z1 not (touchscreen not touched), its better to start with Z1 (z_first = true)
  *
  * There are some combinations to now
  * - drop_first=0 when count=0 | count>0 & averaging=false <br>
  * drop_first=n will be ignored by X and Y measure, Z1 and Z2 measure ignore this when z_once=false.
  * - z_once=true<br>
  * This set z_first to true, the first Z1 and Z2 measurement will be used when drop_first=0. (otherwise, the drop_first+1 measurement)
  * - z_local_min=true<br>
  * In Formula of RZ = RX-plate * x/4096 * (Z2/Z1 -1); Z1 is the divisor. When Z1 raise RX shrink. This mean we measure Z1 as long it grow for getting the local minimum of RZ.
  *
  *
  * @brief  set measure
  * @param drop_first ignore first n measures
  * @param z_once read z1 and z2 only once. When this flag is true z_first must be also true.
  * @param z_first when true start measure with Z1 otherwise with X
  * @param z_local_min when true, get local minimum of RZ
  * @param count how many values used by averaging or witch measure is taken. 0 means read until measure is equal last measure
  */
  inline void setMeasure(uint8_t drop_first = 0, bool z_once = false, bool z_first = false, bool z_local_min = false, uint8_t count = 0);


#ifdef TOUCH_USE_AVERAGING_CODE
/**
  * Set averaging mode.
  *
  * @param averaging when true averaging count measure, otherwise take the count measure as valid measure (if count > 0)
  * @param ignore_min_max when true, ignore min and max value on averaging
  */

  inline void setAveraging(bool averaging, bool ignore_min_max = false);
#endif

 /**
  * Set valid measure range. We are using the 12bit interface from the chip. Value range is 0 .. 4095, but when measure values near 0 or 4095 indicate that there is no touch.
  * @brief  valid measure range
  * @param min minimal valid measure
  * @param max maximal valid measure
  */
 inline void setValidRawRange(uint16_t min, uint16_t max);

 /**
  * Set the waiting time between two measure. If set to 0 every measure (getRaw(), TFT_eTouch<T>::getXY()) will invoke the chip.
  * Otherwise you get the last measure values as long waiting time is not reached.
  * @brief set waiting time
  * @param ms waiting time in miliseconds between measures
  */
  inline void setMeasureWait(uint16_t ms);
 /**
  * @brief get waiting time
  * @return waiting time between measures in miliseconds
  */
  inline uint16_t getMeasureWait() const;

 /**
  * Set the resistace of the X-plate. The value can be between 300 and 1200. It is used for calculating RZ. If you change this value you have to change also the RZ threshold.
  * @brief set RX-plate
  * @param ohm RX-plate value
  */
  inline void setRXPlate(uint16_t ohm);

 /**
  * @brief get RX-plate
  * @return RX-plate in ohm
  */
  inline uint16_t getRXPlate() const;

 /**
  * Set the pressure threshold. If the calculation of RZ is less then this threshold then we have a touch. The value depend on RX-plate and must be also changed when RX-plate is chaged.
  *
  * Good value is tree time of RX-plate.
  *
  * Do not set this threshold to 0xffff, this value indicate that there is no touch.
  * @brief set threshold
  * @param ohm RZ threshold value
  */
  inline void setRZThreshold(uint16_t ohm);
 /**
  * @brief get threshold
  * @return RZ threshold value in ohm
  */
  inline uint16_t getRZThreshold() const;

 /**
  * If you use TOUCH_FILTER_TYPE, you have to call this function on pen-up. Otherwise when TOUCH_FILTER_TYPE is not defined, this function does nothing.
  * @brief reset touch buffer
  */
  inline void reset();

#ifdef TOUCH_USE_USER_CALIBRATION
 /**
  * If you stay with the pen on the touchscreen the X and Y measure will change a little bit. This value describe the
  * tolarable difference of (max-min) from 16 measures. @sa TFT_eTouch<T>::handleTouchCalibrationTarget()
  * @brief set acurate difference
  * @param raw_difference max difference of target measure
  */
  inline void setAcurateDistance(uint16_t raw_difference);
 /**
  * @brief get acurate difference
  * @return acurate difference
  */
  inline uint16_t getAcurateDistance() const;

protected:
/**
  * The user has to touch at scr_x and scr_y, the touch coordinate stored then in touch_x and touch_y.
  * @brief measure point
  */
  struct CalibrationPoint {
    int16_t scr_x;    ///< x target display position
    int16_t scr_y;    ///< y target display position
    int16_t touch_x;  ///< x target touch position measured
    int16_t touch_y;  ///< y target touch position measured

    /// set target display position
    inline void set(int16_t x, int16_t y)
    { scr_x = x; scr_y = y; }

    /// print to Serial
    void      print();
  };

 /**
  * @brief measure CalibrationPoint
  * @param  point target, to measure.
  * @return true when accurate
  */
  bool acurateCalibrationTarget(CalibrationPoint& point);

#else
protected:
#endif // TOUCH_USE_USER_CALIBRATION

/**
  * Fetch all raw touch values in time when only_z1 is false.
  * After the call check (rz != 0xffff) if true, then we have a touch.
  *
  * or
  *
  * Fetch only z1 in time when only_z1 is true.
  * After the call check (raw z1 > 0) if true, then we have a touch.
  * @brief  update raw in time
  * @param only_z1 when true fetch only z1, otherwise x, y, z1, z2 and calculate RZ
  */
  void        update(bool only_z1);

  inline bool valid();       ///< goes true when tuched (RZ < RZ threshold)

  SPIClass&   spi_;  ///< used spi bus. display and touch must use the same bus
  uint8_t     cs_;   ///< chip select pin
  uint8_t     penirq_; ///< penirq pin

  Calibation  calibation_; ///< used callibration for transforming touch measure into display pixels
  Measure     raw_; ///< last touch measure

#ifdef TOUCH_USE_GESTURE
  TFT_eTouchGesture* recognize_;
#endif

#ifdef TOUCH_USE_PENIRQ_CODE
  volatile bool update_allowed_; ///< goes true when penirq happend
#endif // end TOUCH_USE_PENIRQ_CODE

private:
  inline bool is_touched();  ///< goes true when tuched (RZ != 0xffff)
  inline bool in_range(uint16_t measure); ///< mesure between raw_valid_min_ and raw_valid_max_?
  inline void spi_start(); ///< reserve spi bus and select chip
  inline void spi_end(); ///< deselect chip and leave spi bus
  /// @sa update()
  void        fetch_raw(bool only_z1);  ///< fetch raw values

  uint8_t     drop_first_measures_; ///< ignore first n measures
  bool        z_once_measure_; ///< measure Z1 & Z2 only once (do not averaging)
  bool        z_first_measure_; ///< begin with Z1 & Z2 then X Y. when false X Y Z1 Z2
  bool        z_local_min_measure_; ///< read z1 until value grow
  uint8_t     count_measure_; ///< averaging/use n measure. when 0 read until measure is equal previous measure
#ifdef TOUCH_USE_AVERAGING_CODE
  bool        averaging_measure_; ///< when true, averaging count_measure_, when false take the count_measure_ as valid measure (if count_measure_ > 0)
  bool        ignore_min_max_measure_; ///< when averaging do 2 measure more for dropping min max value in average result
#endif // end TOUCH_USE_AVERAGING_CODE

  uint16_t    raw_valid_min_; ///< raw measure minimum value for x, y, z1 and z2 (otherwise it is not touched)
  uint16_t    raw_valid_max_; ///< raw measure maximum value

  uint32_t    last_measure_time_us_; ///< last measure time in microseconds
  uint16_t    measure_wait_ms_; ///< waiting time in miliseconds between measures

  uint16_t    rx_plate_; ///< Resitor value in ohm of x plate (300)
  uint16_t    rz_threshold_; ///< when RZ < RZ threshold we have a valid touch (default 3*RX-plate)

#ifdef TOUCH_USE_USER_CALIBRATION
  uint16_t    acurate_difference_; ///< tolerable noise on X and Y measure for same point
#endif // TOUCH_USE_USER_CALIBRATION

#ifdef TOUCH_FILTER_TYPE
# ifdef TOUCH_X_FILTER
  TOUCH_X_FILTER *x_filter_;
# endif
# ifdef TOUCH_Y_FILTER
  TOUCH_Y_FILTER *y_filter_;
# endif
# ifdef TOUCH_Z_FILTER
  TOUCH_Z_FILTER *z1_filter_;
  TOUCH_Z_FILTER *z2_filter_;
# endif
#endif
};

#include <TFT_eTouchBase.inl>

#ifdef TOUCH_USE_GESTURE
#include <TFT_eTouchGesture.h>
#endif

#endif // TFT_E_TOUCH_BASE_H

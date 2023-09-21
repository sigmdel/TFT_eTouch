## Arduino Configuration

Sub-directories contain two user configuration files for the TFT_eSPI and TFT_eTouch (sigmdel fork) libraries when using the ESP32_2432S028R (aka ESP32_2432S028, aka [cheap yellow display](https://github.com/witnessmenow/ESP32-Cheap-Yellow-Display)) development board.


All examples in the TFT_eTouch (sigmdel fork) library should run without problems on the ESP32_2432S028R if both of these configuration files are used instead of the default files found in the libraries.


### TFT_eSPI

Replace `User_Setup.h` in the TFT_eSPI directory with [User_setup.h](TFT_eSPI/User_Setup.h) in the [TFT_eSPI](TFT_eSPI) directory. 

This file can be used to configure the display if the touch screen is not used or if a different touch library is used.

### TFT_eTouch-sigmdel

Replace `TFT_eTouchUser.h` in the TFT_eTouch directory with [TFT_eTouchUser.h](TFT_eTouch-sigmdel/TFT_eTouchUser.h) in the [TFT_eTouch-sigmdel](TFT_eTouch-sigmdel) directory. 

Do not use this file with the original [TFT_eTouch](https://github.com/achillhasler/TFT_eTouch) library by (achillhasler).

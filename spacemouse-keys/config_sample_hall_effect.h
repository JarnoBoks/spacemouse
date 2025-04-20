#ifndef CONFIG_h
#define CONFIG_h

/* The user specific settings, like pin mappings or special configuration variables and sensitivities are stored in config.h.
   Please adjust your settings and save it as --> config.h <-- !
*/

/* Calibration instructions
===========================

| Follow this file from top to bottom to calibrate your space mouse.          |
| You can find some pictures for the calibration process here:                |
| https://github.com/AndunHH/spacemouse/wiki/Ergonomouse-Build#calibration    |

General
========
The spacemouse is calibrated through the serial monitor (preferred) or through the configuration parameters
in this file. To view the current calibrated values of the spacemouse, you can use the command "SHOW" in the
serial monitor.

In normal operation, the values from config.h are used only for initialization/default values. If the spacemouse
is initialized updating values in config.h WILL NOT BE APPLIED without changing the version number too.

Important: If the version number is changed, all stored calibration parameters will be replaced with the
           values from config.h
*/

/// Change the version number to force the spacemouse to use the values from config.h
/// @warning Changing the version number will reset all stored calibration parameters in the EEPROM.
#define SM_VERSION 3

/* Debugging Instructions
=========================
To activate one of the following debugging modes, you can either:
- Change the STARTDEBUG value below and compile & upload again, or
- Compile and upload your program. Change to the serial monitor and type
   DEBUG <value> , using the number of the debug modes as described below and hit <ENTER>.

Debug Modes:
------------
-1:  Debugging off. Set to this once everything is working.

 0:  Nothing...

 1:  Report raw values for the sensors and the keys.
     The sensor values are inverted if configured in the INVERTLIST. The key values are the raw readings at the input pins (0 or 1).
     The key values are not debounced and are not filtered, and due to the active low nature of the keys 1 means the key is not pressed and 0
     that the key is pressed. The raw output is helpful in calibrating the space mouse. You can check if the sensors are wired correctly and if they are working.

     Joystick:    The values should be approximately 0-1023.
     Hall Effect: The values should be approximately 0-1023.   //TODO - check if this is correct for the HES sensors

     // TODO - Show idlePositions for each sensor. The values should be approximately 0-1023.

 2:  Report centered values.
     When the space mouse is started the knob should be in the idle position. The sensors are read multiple
     times and the mean idle value is calculated and stored. The centered values reported in this debug mode are the
     difference between the raw values and the mean idle value.

     Joystick:    The values should be approximately -500 to +500, jitter around 0 at idle.
     Hall Effect: The values should be approximately -500 to +500.   //TODO - check if this is correct for the HES sensors

 3:  Report hardware sensor output.
     The hardware component of the spacemouse ignores unintended movements on the sensors and will normalize the values
     to a predefined range. Default the output range is -350 to +350. The amount of movement that is ignored can
     be configured with the deadzone.

     Joystick:    The values should be approximately -350 to +350, small movements are ignored.
     Hall Effect: The values should be approximately -350 to +350, small movements are ignored.   //TODO - check if this is correct for the HES sensors

 4:  Report translation & rotation values and status of the keys.
     The translation (TX, TY, TZ) and rotation (RX, RY, RZ) values are calculated from the hardware output.
     See kinematics.h for the details of the calculation. The parameters for the calculation can be configured (see further below).

     Output:      Approximately -350 to +350 depending on the parameter.

 5:  Report centered values (2nd debug) and translation & rotation values and keystatus (4th debug) side by side for direct reference.
     This is very useful if you need to alter which inputs are used in the arithmetic above.
     The values are reported in the same format as in debug mode 4.

 6:  Report centered values (2nd debug) and translation & rotation values and keystatus (4th debug) after applying
     the kill-key functionality. (If configured).

 61: Report centered values (2nd debug) and translation & rotation values and keystatus (4th debug) after applying
     the kill-switch and the exclusive mode. (If configured).

 7:  Report the frequency of the loop().
     This is useful to check if the loop() is running fast enough. The frequency is calculated by counting the number
     of iterations in one second. The frequency is reported in Hz.

 8:  Report the bits and bytes send as button codes

 9:  Report details about the encoder wheel, if ROTARY_AXIS > 0 or ROTARY_KEYS>0
*/

/// The debug level that is used when the program is started. This can be changed in the serial monitor.
/// @note Use DEBUG x to change the debug level in the serial monitor.
#define STARTDEBUG 0

/// The hardware that is used for the spacemouse. This is used to select the correct hardware library.
/// Can only be changed in the config.h file.
/// @note Valid values are "#define HALLEFFECT" or "#define JOYSTICK"
#define HALLEFFECT

/* First Calibration: Hall effect sensors pin assignment
==============================================
Default assembly when looking from above on top of the space mouse
 *
 *    back(USB)     resulting axis (not from the single sensors)
 *
 *      7   6              Y+
 *        |                .
 *   8    |    3           .
 *     ---+---        X-...Z+...X+
 *   9    |    2           .
 *        |                .
 *      0   1              Y-
 *

Each sensor is affected by the position of the magnet. If the magnet moves closer to the sensor the output values should decrease.
If you have mounted the magnets upside-down, the values will be inverted.

1. Try to write down the each HES sensor with the corresponding pin number you chose. If you use the PCB version, the pins are shown
   on the silk screen.
2. Compile the script, type 1 into the serial interface and hit enter to enable debug output 1.
3. With the spacemouse in front of you (ie. USB connection at the backside ie. @ north), move the knob from south to north and observe the debug output:
  3.a) HES0 & HES1 both increase when moving the knob north (magnets further away) and decrease when moving the knob south -> Everything is correct.
  3.b) HES0 & HES1 both decrease when moving the knob south (magnets closer by) -> Everything is correct.
  3.c) The sensorpair acts in the opposite direction (ie. increasing when the magnets get closer by ). You probably have mixed up the poles of the magnets.
       Invert the sensorpair in the INVERTLIST.
  3.c) Another output is showing movement: Swap the pins in the PINLIST. This shouldn't happen when the PCB is used. Debugging is rather difficult when
       the spacemouse is assembled, due to the fact that all sensors act on the same movement.

4. Continue with the other sensor pairs. Moving the magnet closer to the sensor should decrease the value.

5. Optimally when not moving the knob, all sensors should output approximately the same value. You can adjuist the values a little bit
   by adjusting the height of the sensor plate using the spacernuts.

5. Repeat this with every axis and every sensor pair until you have a valid PINLIST and maybe an INVERTLIST
*/

// HES0, HES1, HES2, HES3, HES6, HES7, HES8, HES9
#define PINLIST \
    {A0, A1, A2, A3, A6, A7, A8, A9}
// Check the correct wiring with the debug output=1

// Set to 1 to invert one hall sensor.
// Values should decrease when the magnet is nearing the sensor, but if the magnet is placed with the poles reversed, you can
// invert the value. Usually the inversion should be configured by HES-pair (ie. 0 & 1, 2 & 3, 6 & 7, 8 & 9)
#define INVERTLIST \
    {0, 0, 0, 0, 0, 0, 0, 0}
// HES0, HES1, HES2, HES3, HES6, HES7, HES8, HES9

/* Second calibration: Tune deadzone   (command: DEADZONE | DEADZONE <value>)
==============================================================================
The Hardware deadzone is used to filter out unintended movements. Increase the deadzone if the space mouse has small movements when it should be idle or
when the mouse is too sensitive to subtle movements. On the other hand a small deadzone is recommended to allow full range of motion. There are two ways to calibrate the deadzone:

Semi-automatic: Use the command "DZ" (without a value) in the serial monitor. The command will automatically calculate the deadzone based on the current sensor values.
                Don't touch the mouse and observe the automatic output and the suggested value for the deadzone. Use the command "DZ <value>" to set the deadzone and store it.
                Alternatively, you can apply the suggested value to one of the DEADZONE variables below.

Manual:         Set debug = 2.  Don't touch the mouse but observe the values. They should be nearly to zero.
                                Every value around zero which is noise or should be neglected afterwards is in the following deadzone.

Expected outcome:
   Joystick:    The joystick is less prone to unintended movements. Deadzone is expected to be around 2-5
   Hall Effect: The sensor are more sensitive due to magnetic environmental influences. Deadzone is expected to be around 7-15

*/
// TODO - These values are not used yet. Have to be implemented in the hardware library.
#define JOYSTICK_DEFAULT_DEADZONE 3    // !! Adjust this value if you use the joystick sensor version. This is the deadzone for the joystick hardware.
#define HALLEFFECT_DEFAULT_DEADZONE 10 // !! Adjust this value if you use the HALL sensor version. Only effects the defaults for the Hall Effect hardware

/* Third calibration: Getting MIN and MAX values   (command: MINMAX | MINMAX <+|-><axisname> <value>)
=====================================================================================================
Can be done automatic, semi-automatic or manual

The command "SHOW" will show the current values in the serial monitor.
The command "MINMAX 1" will store the values in the EEPROM. The command "MINMAX 0" will not store the values in the EEPROM.

Semi-automatic (command: MINMAX)
--------------------------------
1. In the Serial monitor type the command "MINMAX" and hit ENTER.
2. Move the Spacemouse around for 15s to record the minimum and maximum values for each sensor.
3. Verify if there are any warnings for the Min, Max or Range. Check if your hardware is working correctly and/or retry the calibration.
   For the joystick sensors, the values should be approximately -400 to +400 and the maxVals around +400 to +400.
   For the HES sensors, the values should be approximately -400 to -520 and the maxVals around +400 to +520.
4. When satisfied you can enter the values into the config.h file below or enter them one by one using the manual commands as described below.

Automatic (command: MINMAX 1)
-----------------------------
1. In the Serial monitor type the command "MINMAX 1" and hit ENTER.
2. Move the Spacemouse around for 15s to record the minimum and maximum values for each sensor.
3. The results are shown and the values are stored in the EEPROM.
3. Verify if there are any warnings for the Min, Max or Range. Check if your hardware is working correctly and/or retry the calibration.
   For the joystick sensors, the values should be approximately -400 to +400 and the maxVals around +400 to +400.
   For the HES sensors, the values should be approximately -400 to -520 and the maxVals around +400 to +520.
4. When satisfied you can enter the values into the config.h file below or enter them one by one using the manual commands as described below.
// TODO - Do not store the values if there are too many warnings

Manual min/max calibration (command: DEBUG 2)
---------------------------------------------
1. Compile the sketch and upload it. Go to the Serial monitor type the command "DEBUG 2" and hit ENTER.
2. Get a piece of paper and write downn the following chart:

 Chart:
      maxVals          |     minVals
-------------------------------------------------
| +AX / +HES0:         | -AX / -HES0:           |
| +AY / +HES1:         | -AX / -HES1:           |
| +BX / +HES2:         | -BX / -HES2:           |
| +BY / +HES3:         | -BY / -HES3:           |
| +CX / +HES6:         | -CX / -HES6:           |
| +CY / +HES7:         | -CY / -HES7:           |
| +DX / +HES8:         | -DX / -HES8:           |
| +DY / +HES9:         | -DY / -HES9:           |


3. (a) Start out with AX+ / HES0+ (positive Values)
   (b) Start moving the your Spacemouse and try increasing the value of this sesnor till you can't get a higher value out of it.
   (c) this is your positive maximum value for AX+ / HES0 so write it down for HES0
4. Do the same for all the other sensors
5. Do the same for your negative Values to populate the minVals
6. Write all the positive Values starting from the top into the Array maxValues
7. Write all the negative Values starting from the top into the Array minValues
8. You finished calibrating the min and max settings of the hardware.

To store the values for single sensors in the EEPROM, you can use the command "MINMAX <+|-><sensorname> <value>".
   <+|->        - The sign of the value. + for max, - for min
   <sensorname> - The name of the sensor. The name of the sensor is the same as in the chart above.
   <value>      - The value to set for the sensor.

Examples:
   To set the max value for HES0 to 1000, you can use the command "MINMAX +HES0 1000".
   To set the min value for DY to -350, you can use the command "MINMAX -HES0 -350".


Insert measured Values like this:

   Joystick:       { AX, AY, BX, BY, CX, CY, DX, DY}
   Hall sensors:   { HES0, HES1, HES2, HES3, HES6, HES7, HES8, HES9}
*/
// #define MINVALS  {-400, -400, -400, -400, -400, -400, -400, -400}
// #define MAXVALS  {+175, +175, +175, +175, +175, +175, +175, +175}

// ------_______------_______------_______------_______------_______------_______------_______------_______------_______------_______------

/**
 *
 *    Sensor calibration is finished. The spacemouse is now calibrated to transform the normalized sensor readings to raw translation & rotation values.
 *    In the next phase of the calibration we can finetune this transformation, by adjusting the sensitivity of the translation and rotation values.
 *
 */

/* Fourth calibration: Sensitivity
==================================
Use debug mode 4 or use for example your CAD program to verify changes.

The sensitivity values are used in a division, thus
  - Use a fraction to make the axis MORE sensitive. F.e. 0.5 makes the axis twice as sensitive.
  - Use a value larger than 1 to make it LESS sensitive. F.e. 5 makes the axis five times less sensitive.

Recommended calibration procedure for sensitivity
-------------------------------------------------
1. Make sure modFunc is on level 0, see below. Upload the sketch. Then open serial monitor, type 4 for and hit enter.
   You will see Values TX, TY, TZ, RX, RY, RZ, the configured keys and the current status of the sensitivity parameters.
2. Start moving your Spacemouse. You will notice values changing.
3. Starting with TX try increasing this value as much as possible by moving your Spacemouse around. If you get around 350 thats great.
   If not change TRANSX_SENSITIVITY. Repeat until it is around 350 for maximum motion.
4. Repeat steps 3 for TY, TZ, RX, RY, RZ
5. Verification: Move the Joystick in funny ways. All you should get for either TX,TX,TZ,RX,RY,RZ should be approximately between -350 to 350.
6. You have finished sensitivity calibration. You can now test your Spacemouse with your favorite program (e.g. Cad software, Slicer)
7. Aftermath: You notice the movements are hard to control. Try using Modification Functions [Suggestion: ModFunc level 3]
*/

#define TRANSX_SENSITIVITY 0.80
#define TRANSY_SENSITIVITY 0.99
#define POS_TRANSZ_SENSITIVITY 2.5
#define NEG_TRANSZ_SENSITIVITY 1.5
#define GATE_NEG_TRANSZ 15 // gate value, which negative z movements will be ignored (like an additional deadzone for -z).
#define GATE_ROTX 15       // Value under which rotX values will be forced to zero
#define GATE_ROTY 15       // Value under which roty values will be forced to zero
#define GATE_ROTZ 15       // Value under which rotz values will be forced to zero

#define ROTX_SENSITIVITY 1.2
#define ROTY_SENSITIVITY 1.2
#define ROTZ_SENSITIVITY 0.90

/* Fifth calibration: Modifier Function
=======================================
Modify resulting behaviour of Spacemouse outputs to suppress small movements around zero and enforce big movements even more.

Check the README.md for more details and a plot of the different functions.
(This function is applied on the resulting velocities and not on the direct input from the joysticks)

This should be at level 0 when starting the calibration!
0: linear y = x [Standard behaviour: No modification]
1: squared function y = x^2*sign(x) [altered squared function working in positive and negative direction]
2: tangent function: y = tan(x) [Results in a linear curve near zero but increases the more you are away from zero]
3: squared tangent function: y = tan(x^2*sign(X)) [Results in a flatter curve near zero but increases a lot the more you are away from zero]
4: cubed tangent function: y = tan(x^3) [Results in a very flat curve near zero but increases drastically the more you are away from zero]

Recommendation after tuning: MODFUNC 3
*/
#define MODFUNC 0 // Used as default value as long as the data hasn't been saved in the EEPROM

/* Sixth Calibration: Direction
===============================
Modify the direction of translation/rotation depending on the CAD program you are using on your PC.
This should be done, when you are done with the pin assignment!

If all defines are set to 0 the resulting X, Y and Z axis correspond to the pictures shown in the README.md.
The suggestion in the comments for "3Dc" are often needed on windows PCs with 3dconnexion driver to get expected behavior.
*/
#define INVX 1  // pan left/right  // 3Dc: 0
#define INVY 1  // pan up/down     // 3Dc: 1
#define INVZ 1  // zoom in/out     // 3Dc: 1
#define INVRX 1 // Rotate around X axis (tilt front/back)  // 3Dc: 0
#define INVRY 1 // Rotate around Y axis (tilt left/right)  // 3Dc: 1
#define INVRZ 1 // Rotate around Z axis (twist left/right) // 3Dc: 1

// Switch Zoom direction with Up/Down Movement
#define SWITCHYZ 0 // change to 1 to switch Y and Z axis

/* Key Support
===============
If you attached keys to your Spacemouse, configure them here.
You can use the keys to report them via USB HID to the PC (either classically pressed or emulated with an encoder) or as kill-keys (described below).

How many classic keys are there in total? (0=no keys, feature disabled)
*/
#define NUMKEYS 3 // 0

// Define the PINS for the classic keys on the Arduino
// The first pins from KEYLIST may be reported via HID
#define KEY_PINLIST \
    {0, 1, 2}

/* Report KEYS over USB HID to the PC
 ----------------------------------
How many keys reported? Classical + ROTARY_KEYS in total.
*/
#define NUMHIDKEYS 3 // 0

// In order to define which key is assigned to which button, the following list must be entered in the BUTTONLIST below

#define SM_MENU 0  // Key "Menu"
#define SM_FIT 1   // Key "Fit"
#define SM_T 2     // Key "Top"
#define SM_R 4     // Key "Right"
#define SM_F 5     // Key "Front"
#define SM_RCW 8   // Key "Roll 90°CW"
#define SM_1 12    // Key "1"
#define SM_2 13    // Key "2"
#define SM_3 14    // Key "3"
#define SM_4 15    // Key "4"
#define SM_ESC 22  // Key "ESC"
#define SM_ALT 23  // Key "ALT"
#define SM_SHFT 24 // Key "SHIFT"
#define SM_CTRL 25 // Key "CTRL"
#define SM_ROT 26  // Key "Rotate"

// BUTTONLIST must have at least as many elements as NUMHIDKEYS
// The keys from KEYLIST or ROTARY_KEYS are assigned to buttons here:
#define BUTTONLIST {SM_T, SM_R, SM_F}

/* Exclusive mode
=================
Exclusive mode only permit to send translation OR rotation, but never both at the same time.
This can solve issues with classic joysticks where you get unwanted translation or rotation at the same time.

it choose to send the one with the biggest absolute value.
*/
#define EXCLUSIVEMODE

/* Kill-Key Feature
--------------------
Are there buttons to set the translation or rotation to zero?
How many kill keys are there? (disabled: 0; enabled: 2)
*/
#define NUMKILLKEYS 0
// usually you take the last two buttons from KEYLIST as kill-keys
// Index of the kill key for rotation
#define KILLROT 2
// Index of the kill key for translation
#define KILLTRANS 3
// Note: Technically you can report the kill-keys via HID as "usual" buttons, but that doesn't make much sense...

/*  Example for NO KEYS
 *  There are zero keys in total:  NUMKEYS 0
 *  KEYLIST { }
 *  NUMHIDKEYS 0
 *  BUTTONLIST { }
 *  NUMKILLKEYS 0
 *  KILLROT and KILLTRANS don't matter... KILLROT 0 and KILLTRANS 0
 */

/*  Example for three usual buttons and no kill-keys
 *  There are three keys in total:  NUMKEYS 3
 *  The keys which shall be reported to the pc are connected to pin 15, 14 and 16
 *  KEYLIST {15, 14, 16}
 *  Therefore, the first three pins from the KEYLIST apply for the HID: NUMHIDKEYS 3
 *  Those three Buttons shall be "FIT", "T" and "R": BUTTONLIST {SM_FIT, SM_T, SM_R}
 *  No keys for kill-keys NUMKILLKEYS 0
 *  KILLROT and KILLTRANS don't matter... KILLROT 0 and KILLTRANS 0
 */

/*
 *  Example for two usual buttons and two kill-keys:
 *  There are four keys in total:  NUMKEYS 4
 *  The keys which shall be reported to the pc are connected to pin 15 and 14
 *  The keys which shall be used to kill translation or rotation are connected to pin 16 and 10
 *  KEYLIST {15, 14, 16, 10}
 *  Therefore, the first two pins from the KEYLIST apply for the HID: NUMHIDKEYS 2
 *  Those two Buttons shall be "3", "4": BUTTONLIST {SM_3, SM_4}
 *  Two keys are used as kill-keys: NUMKILLKEYS 2
 *  The first kill key has the third position in the KEYLIST and due to zero-based counting third-1 => KILLROT 2
 *  The second kill key has the last position in the KEYLIST with index 3 -> KILLTRANS 3
 */

// Some simple tests for the definition of the keys
#if (NUMKILLKEYS > NUMKEYS)
#error "Number of Kill Keys can not be larger than total number of keys"
#endif
#if (NUMKILLKEYS > 0 && ((KILLROT > NUMKEYS) || (KILLTRANS > NUMKEYS)))
#error "Index of killkeys must be smaller than the total number of keys"
#endif

// time in ms which is needed to allow a new button press
#define DEBOUNCE_KEYS_MS 200

/* Encoder Wheel
================
You can attach an encoder to the mouse, which acts as an input device for one movement.
Needs the encoder library by Paul Stoffregen (https://www.pjrc.com/teensy/td_libs_Encoder.html).
*/

// Define the encoder pins
#define ENCODER_CLK 2
#define ENCODER_DT 3
// swap those two pins to change direction of encoder

/*
Axis to replace with encoder
0. None -> disable this feature completely
1. transX
2. transY (zoom in "Forward / Backward" Zoom Direction configuration, see SWITCHYZ)
3. transZ (simulates zoom in "Up / Down" Zoom Direction configuration , see SWITCHYZ)
4. rotX
5. rotY
6. rotZ
(Those are the positions in the velocity array +1, as defined in kinematics.h)
*/
#define ROTARY_AXIS 0

/* To calculate a velocity from the encoder position, the output is faded over so many loop() iterations, as defined in #ECHOES
Small number = short duration of zooming <-> Big Number = longer duration of zooming
Compare this number with the update frequency of the script, reported by debug=7: If ECHOES = frequency: the zoom is faded for 1 second.
*/
#define ECHOES 200

/* Strength of the simulated pull
Recommended range: 0 - 350
  Reason for max=350: The HID Interface reports logical max as +350, see hidInterface.h
Recommended strength = 200
*/
#define SIMSTRENGTH 200

/* ROTARY_KEYS
=============
Use the encoder and emulate a key stroke by turning the encoder.
*/
#define ROTARY_KEYS 0
// which key from the BUTTONLIST shall be emulated?
// First direction  (0 = first element from BUTTONLIST, 1 = second element, etc.)
#define ROTARY_KEY_IDX_A 2
// counter direction
#define ROTARY_KEY_IDX_B 3
// duration of simulated key
#define ROTARY_KEY_STRENGTH 19

/* LED support
===============
You can attach:
a) a simple LED to the mouse. LED shall be connected to 5V and the controller port.
b) a fancy LED strip, like the nanopixel. Check the FASTLED library for supported chips / led strips.

Which pin shall be used as LED? This pin is used either as a digital pin (a) or as the data pin (b).
Change from "//define" to "#define" to activate the LED feature.
*/
// #define LEDpin 5

/* Simple LED
-------------
// If you have connected a single LED to the controller port and GND, invert it by uncommenting this #define
*/
// #define LEDinvert

/* LED strip with data pin
---------------------------
The connected LED is not just a stupid LED, but an intelligent one, like a neopixel controlled by FASTLED library. If set, the LEDRING gives the number of LEDs on the ring.
*/

// #define LEDRING 24
//  The LEDpin is used as a data pin

// The LEDs light up, if a certain movement is reached:
#define VELOCITYDEADZONEFORLED 15

// About how many LEDs must the ring by turned to align?
#define LEDclockOffset 0

// how often shall the LEDs be updated
#define LEDUPDATERATE_MS 150

/* Advanced debug output settings
=================================
The following settings allow customization of debug output behavior */

// Generate a debug line only every DEBUGDELAY ms
#define DEBUGDELAY 100

// The standard behavior "\r" for the debug output is, that the values are always written into the same line to get a clean output. Easy readable for the human.
#define DEBUG_LINE_END "\r"
// If you need to report some debug outputs to trace errors, you can change the debug output to "\r\n" to get a newline with each debug output. (old behavior)
// #define DEBUG_LINE_END "\r\n"

/* Advanced USB HID settings
============================
The following settings are advanced and don't need to changed for normal windows users.
*/

// Definition, how many bits are used in the HID report to encode the keys
#define HIDMAXBUTTONS 32 // must be multiple of 8!

/* ADV_HID_REL and ADV_HID_JIGGLE change how the values are reported over HID protocol, see hidInterface.cpp and .h

For windows users: DON'T CHANGE / DON'T ENABLE THIS, if you don't understand what it does.

For linux / spacenavd user: Suggestions to enable #define ADV_HID_JIGGLE

Translation and rotation values are either declared as absolute or relative values in the hid descriptor in hidInterface.h.

Relative declaration (may be activated by ADV_HID_REL)
-------------------------------------------------------
With linux and spacenavd: If the space mouse didn't return to absolutely zero in one axis this axis will still report movement, when another direction is pushed, because only the changed values are emitted as events by the linux kernel.

Despite that, values events are emitted with every report send, even if they didn't changed.

Absolute declaration (default)
------------------------------
Every value is always reporting the absolute position.
This means, in contrast to relative, an axis that is left alone is reported again as zero.
On the other hand, events are only emitted, if at least some value changes.
This is not always the case, when the space mouse is held still at a non-zero position.
Solution: Jiggling (may be activated by ADV_HID_JIGGLE)
Every non-zero value is reported as it is and +1 in the next report, repeating with +0 in the next iteration and +1 in the next...
This little extra noise is called "jiggling" and ensures that a value declared as absolute is resent with every report, because is not equal to the last value.
*/

// Switch declaration of values to relative, if the following symbol is defined:
// #define ADV_HID_REL

// Add Jiggling to the value reported, if the following symbol is defined:
// #define ADV_HID_JIGGLE

#endif // CONFIG_h
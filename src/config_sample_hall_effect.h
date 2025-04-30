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

 2:  Report centered values.
     When the space mouse is started the knob should be in the idle position. The sensors are read multiple
     times and the mean idle value is calculated and stored. The values reported in this debug mode are the
     difference between the raw values and the mean idle value.

     Joystick:    The values should be approximately -500 to +500, jitter around 0 at idle.
     Hall Effect: The values should be approximately -500 to +500.   //TODO - check if this is correct for the HES sensors

 3:  Report hardware sensor output.
     The hardware component of the spacemouse ignores unintended movements on the sensors and will normalize the values
     to a predefined range. Default the output range is -350 to +350. The amount of movement that is ignored can
     be configured with the deadzone.

     Joystick:    The values should be approximately -350 to +350, small movements are ignored.
     Hall Effect: The values should be approximately -350 to +350, small movements are ignored.   //TODO - check if this is correct for the HES sensors

 4:  Report translation & rotation values.
     The translation (TX, TY, TZ) and rotation (RX, RY, RZ) values are calculated from the hardware output and sensitivity is applied. None of the modifier function, inversion, YZ switching nor Exclusivemode are
     applied. See kinematics.h for the details of the calculation. The parameters used in the calculation can be configured (see further below).

     Output:      Approximately -350 to +350 depending on the parameter.

 5:  Report centered values (2nd debug) and translation & rotation values side by side for direct reference. Modifier function and inversion are applied. Any configured YZ switching or Exclusivemode is not applied.

 6:  Report centered values (2nd debug) and translation & rotation values (5th debug) and the key state after applying the kill-key functionality.

 7: Report centered values (2nd debug) and translation & rotation values and keystatus (4th debug) after applying
     the kill-switch and the exclusive mode. (If configured).

 8:  Report the frequency of the loop().
     This is useful to check if the loop() is running fast enough. The frequency is calculated by counting the number
     of iterations in one second. The frequency is reported in Hz.

 9:  Report the bits and bytes send as button codes

10:  Report details about the encoder wheel, if ROTARY_AXIS > 0 or ROTARY_KEYS>0
*/

/// The debug level that is used when the program is started. This can be changed in the Serial monitor.
/// @note Use the command "DEBUG <value>" to change the debug level from within in the Serial monitor.
#define STARTDEBUG OFF

/// The hardware that is used for the spacemouse. This is used to select the correct hardware library.
/// This setting can only be changed in the config.h file.
/// @note Valid values are "#define HALLEFFECT" or "#define JOYSTICK"

// Hardware uses HallEffect sensors instead of joystick sensors
#define HW_HALLEFFECT
// #define HW_JOYSTICK // Hardware definition for the joystick version - does not apply to the Hall effect version

#ifdef HW_JOYSTICK // Hardware definition for the joystick version - does not apply to the Hall effect version
/* First Calibration: Joystick axis pin assignment
==============================================
Default Assembly when looking from above on top of the space mouse
   back    resulting axis (not from the single joystick)
    C           Y+
    |           .
 B--+--D   X-...Z+...X+
    |           .
    A           Y-
  front

Each joysticks has
- a horizontal axis from left to right = Y
- a vertical axis from top to bottom = X
(This definition of X and Y may not correspond to the print on your joysticks... We will find out which signal is X and Y now.)

1. Try to write down the two axis of every joystick with the corresponding pin numbers you chose.
(A4 and A5 are not used in the example by TeachingTech).
2. Compile the script, type 1 into the serial interface and hit enter to enable debug output 1.
3. At the joystick in front of you (A), move the joystick from the top -> down to bottom (X) and observe the debug output:
  3.a) AX goes from 0 (=joystick at the top) to 1023 (joystick at bottom) -> Everything is correct.
  3.b) AX goes from 1023 to 0 -> You need to invert AX, see INVERTLIST below.
  3.c) Another output is showing movement: Swap the pins in the PINLIST. Probably you have to swap the first and second element, as AX and AY may be swapped.

If you have the joystick TeachingTech recommended:
    The pins labelled X and Y on the joystick are NOT the X and Y needed here, but swapped. First joysticks Y: AX and X: AY.

4. Continue with AY: Move the joystick from left to right and the values shall increase from 0 to 1023 and follow the instructions 3a) to 3c) above.

5. Repeat this with every axis and every joystick until you have a valid PINLIST and maybe an INVERTLIST
*/

// AX, AY, BX, BY, CX, CY, DX, DY
#define PINLIST \
    {A1, A0, A3, A2, A7, A6, A9, A8}

// Set to 1 to invert one joystick axis.
// Usually all _X values shall be inverted or none of them.
// Usually all _Y values shall be inverted or none of them.
#define INVERTLIST \
    {0, 0, 0, 0, 0, 0, 0, 0}
// AX, AY, BX, BY, CX, CY, DX, DY
#endif // JOYSTICK

#ifdef HW_HALLEFFECT // Hardware definition for the Hall effect version - does not apply to the joystick version
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
#endif

/* Second calibration: Tune deadzone   (command: DZ | DZ <value>)
==============================================================================
The Hardware deadzone is used to filter out unintended movements. Increase the deadzone if the space mouse has small movements when it should be idle or
when the mouse is too sensitive to subtle movements. On the other hand a small deadzone is recommended to allow full range of motion. There are two ways to calibrate the deadzone:

Semi-automatic: Use the command "DZ" (without a value) in the serial monitor. The command will automatically calculate the deadzone based on the current sensor values.
                Don't touch the mouse and observe the automatic output and the suggested value for the deadzone.
                Use the command "DZ <value>" to set the deadzone and store it.
                Alternatively, you can apply the suggested value to one of the DEADZONE variables below.

Manual:         Set debug = 2.  Don't touch the mouse but observe the values. They should be nearly to zero.
                                Every value around zero which is noise or should be neglected afterwards is in the following deadzone.

Expected outcome:
   Joystick:    The joystick is less prone to unintended movements. Deadzone is expected to be around 2-5
   Hall Effect: The sensor are more sensitive due to magnetic environmental influences. Deadzone is expected to be around 7-15

*/

// The deadzone default value can be overridden in the config.h file by using the following defines format:
// #define DEADZONE <value>

/* Third calibration: Getting MIN and MAX values   (command: MINMAX | MINMAX <+|-><sensorname> <value>)
=====================================================================================================
Can be done automatic, semi-automatic or manual

The command "MINMAX" will show the current values in the serial monitor.
The command "MINMAX 1" will let you calibrate the values and store them in the EEPROM. The command "MINMAX 0" will not store the values in the EEPROM (ie. they will be lost at reboot).

Semi-automatic (command: MINMAX 0)
--------------------------------
1. In the Serial monitor type the command "MINMAX 0" and hit ENTER.
2. Move the Spacemouse around for 15s to record the minimum and maximum values for each sensor.
3. Verify if there are any warnings for the Min, Max or Range. Check if your hardware is working correctly and/or retry the calibration.
   For the joystick sensors, the values should be approximately -400 to +400 and the maxVals around +400 to +400.
   For the HES sensors, the values should be approximately -400 to -520 and the maxVals around +400 to +520.
4. When satisfied you can enter the values into the config.h file below or enter them one by one using the manual commands as described below. //TODO REWRITE THIS LINE

Automatic (command: MINMAX 1)
-----------------------------
1. In the Serial monitor type the command "MINMAX 1" and hit ENTER.
2. Move the Spacemouse around for 15s to record the minimum and maximum values for each sensor.
3. The results are shown and the values are stored in the EEPROM.
3. Verify if there are any warnings for the Min, Max or Range. Check if your hardware is working correctly and/or retry the calibration.
   For the joystick sensors, the values should be approximately -400 to +400 and the maxVals around +400 to +400.
   For the HES sensors, the values should be approximately -400 to -520 and the maxVals around +400 to +520.
4. When satisfied you can enter the values into the config.h file below or enter them one by one using the manual commands as described below. //TODO - REWRITE THIS LINE

Manual min/max calibration (use DEBUG 2)
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

To store the values for single sensors in the EEPROM, you can use the command "MINMAX <+|-><sensorname> <value>".
   <+|->        - Indication of the value to set. + for max, - for min
   <sensorname> - The name of the sensor. The name of the sensor is the same as in the chart above.
   <value>      - The value to set for the sensor.

Examples:
   MINMAX +AX 1000       // Set the maximum value for AX to 1000
   MINMAX -HES0 350      // Set the minimum value for HES0 to -350

Insert measured Values like this:

   Joystick:       { AX, AY, BX, BY, CX, CY, DX, DY}
   Hall sensors:   { HES0, HES1, HES2, HES3, HES6, HES7, HES8, HES9}
*/
// #define MINVALS  {-400, -400, -400, -400, -400, -400, -400, -400}
// #define MAXVALS  {+175, +175, +175, +175, +175, +175, +175, +175}

// ------_______------_______------_______------_______------_______------_______------_______------_______------_______------_______------

/**
 *
 *    Hardware sensor calibration is finished. The spacemouse is now setup to transform the normalized sensor readings to raw translation & rotation values.
 *    In the next phase of the calibration we can finetune the translation & rotation, by adjusting the parameters of the sensor to motionaxis.
 *
 */

/* Fourth calibration: Base Sensitivity   (command: SENS | SENS [G}<axisname>[+|-] <value>) | SENS 99999)
==========================================================================================================
Use debug mode 4 or use for example your CAD program to verify changes.
Note: Neither the modifier function nor the inversion are applied in debug mode 4.

Recommended calibration procedure for base sensitivity
-------------------------------------------------
1. Compile the sketch and upload it. Then open the Serial monitor, type "DEBUG 4" and hit enter.
   You will see Values TX, TY, TZ, RX, RY, RZ and the configured keys. The values are the raw translation & rotation values (ie. the
   modulation function and inversion are not applied).
2. Start moving your Spacemouse. You will notice values changing.
3. Starting with TX. Try increasing this value as much as possible by moving your Spacemouse around. If you get around +350 and -350 thats great.
   If not change SENS_TX_POS/NEG. Repeat until it is around 350 for maximum motion.
4. Repeat steps 3 for TY, TZ, RX, RY, RZ
5. Verification: Move the Joystick in funny ways. All you should get for either TX,TX,TZ,RX,RY,RZ should be approximately between -350 to 350.
6. You have finished sensitivity calibration. You can now test your Spacemouse with your favorite program (e.g. Cad software, Slicer)
7. Aftermath: You notice the movements are hard to control. Try using Modification Functions [Suggestion: ModFunc level 3]

To store the sensitivity values for a single axis in the EEPROM, you can use the command
   SENS <axisname>[+|-] <value>
      <axisname>   - The name of the axis. TX, TY, TZ, RX, RY or RZ
      [+|-]        - Optional indication of the direction for which the value will be set. + for positive, - for negative. If not set, the value is set for both directions.
      <value>      - The sensitivity value to set for the axis. Should be a float (ie 2 should be 2.0)
                     Use a fraction to make the axis MORE sensitive. F.e. 0.5 makes the axis twice as sensitive.
                     Use a value larger than 1 to make it LESS sensitive. F.e. 2.0 makes the axis two times less sensitive.

Examples:
   To adjust the sensitivity for positive rotation around the x axis to 0.5, you can use the command "SENS +RX 0.5"
   To adjuist the sensitivity for positive and negative translation in the y direction to 3, you can use the command "SENS TY 3.0"

To suppress small movements around zero, you can add an additional gate to each axis and even each axis direction. Use the command
   SENS G<axisname>[+|-] <value>    (Note the 'G' in front of the axis name)
      <axisname>   - The name of the axis. TX, TY, TZ, RX, RY or RZ
      [+|-]        - Optional indication of the direction for which the value will be set. + for positive, - for negative. If not set, the value is set for both directions.
      <value>      - The sensitivity value to set for the axis. Should be an integer value.

Use the command "SENS" (without any parameter) to show the current modifier function for all axes.

Use the command "SENS 99999" to restore the default sensitivity, modifier, gate and inversion values for all axes to start
the sensitivity calibration from scratch.
*/

// All sensitivity defaults can be overridden in the config.h file by using the following defines format:
//
// #define DEF_<SENS|GATE>_<axisname>_<POS|NEG> <value>   where axisname is TX, TY, TZ, RX, RY, RZ
//                                                       value    is a float value for SENS (0.0-10.0)
//
//
// Examples:
// #define DEF_SENS_TX_POS 2.0         // will set the default sensitivity for the positive x translation to 2.0
// #define DEF_GATE_TZ_NEG 15          // will set the default gate for the negative z translation to 15

// >>> Place your default values here. These values will be used if the EEPROM is empty or if the version number (SM_VERSION) is changed.

/* Fifth calibration: Modifier Function      (command: SENS | SENS M<axisname>[+|-] <value>) | SENS 99999)
==============================================================================================================
Use debug mode 5 ("DEBUG 5") or use for example your CAD program to verify changes.
In debug mode 5 the modifier function and the inversion are applied to the translation & rotation values.
Modify resulting behaviour of Spacemouse outputs to suppress small movements around zero and enforce big movements even more.

Check the README.md for more details and a plot of the different functions.
The modifier function is applied on the resulting translations & rotations and not on the direct input from the joysticks.

The available modifier functions are:
 0: linear y = x [Standard behaviour: No modification]
 1: squared function y = x^2*sign(x) [Squared function working in positive and negative direction]
 2: tangent function: y = tan(x) [Results in a linear curve near zero but increases the more you are away from zero]
 3*: squared tangent function: y = tan(x^2*sign(x)) [Results in a flatter curve near zero but increases a lot the more you are away from zero]
 4: cubed tangent function: y = tan(x^3) [Results in a very flat curve near zero but increases drastically the more you are away from zero]

 *) Default for all axes except TZ+ (pulling the knob up) is 3. The default for TZ+ is 0 (linear).

Update the modifier function for a single axis by sending the command

      SENS M<axisname>[+|-] <value>           (Note the 'M' in front of the axis name)
         <axisname>   - The name of the axis. TX, TY, TZ, RX, RY or RZ
         [+|-]        - Optional indication of the direction for which the value will be set. + for positive, - for negative. If not set, the value is set for both directions.
         <value>      - The modifier function to set for the axis (0-4, one of the functions described above).

Use the command "SENS" (without any parameter) to show the current modifier function for all axes.

Use the command "SENS 99999" to restore the default sensitivity, modifier, gate and inversion values for all axes to start
the sensitivity calibration from scratch.

Note: See below - after setting up any connected keys / rotary encoders - for fine-tuning of the translation & rotation values.

*/

// To set or overwrite the default settings from within config.h, you can use the following settings:
//
// Set the default modifier function for all axes except TZ+ :
//    #define DEF_MODFUNC <x>
//
// Set the default modifier function for a single axis:
//    #define DEF_SENS_<axisname>_<POS|NEG> <value>   where axisname is TX, TY, TZ, RX, RY, RZ
//                                                          value    is a float value for SENS (0.0-10.0)
//
//
// Examples:
// #define DEF_MODFUNC 3 // will set the default sensitivity for all axes except TZ+ to 3 (Squared tangent function)
// #define DEF_MODFUNC_TZ_POS 0 // will set the default modifier function for the positive z translation to the linear function (0)

// >>> Place your default values here. These values will be used if the EEPROM is empty or if the version number (SM_VERSION) is changed.

/* Sixth Calibration: Direction           (command: SENS | SENS I<axisname>[+|-] <value>) | SENS 99999)
========================================================================================================
Modify the direction of translation/rotation depending on the CAD program you are using on your PC.
Use debug mode 6 ("DEBUG 6") or use for example your CAD program to verify changes.

If no inversions are set the resulting X, Y and Z axis correspond to the pictures shown in the README.md.

Update the inversion for a single axis by sending the command

      SENS I<axisname>[+|-] <value>           (Note the 'I' in front of the axis name)
         <axisname>   - The name of the axis. TX, TY, TZ, RX, RY or RZ
         [+|-]        - Optional indication of the value to set. + for positive, - for negative. If not set, the value is set for both directions.
         <value>      - The inversion value to set for the axis (0 or 1).

Use the command "SENS" (without any parameter) to show the current modifier function for all axes.

Use the command "SENS 99999" to restore the default sensitivity, modifier, gate and inversion values for all axes to start
the sensitivity calibration from scratch.

*/

// To set or overwrite the default settings from within config.h, you can use the following settings:
//
// #define DEF_INVERT_<axisname> <value>   where axisname is TX, TY, TZ, RX, RY, RZ
//
// >>> Place your default values here. These values will be used if the EEPROM is empty or if the version number (SM_VERSION) is changed.

/* Seventh calibration: Axis used for zooming in and out.   (command: SWITCHYZ | SWITCHYZ <0|1>)
================================================================================================
You can change the movement for zooming in and out. Default behaviour for zooming is the translation at the Y axis (ie. zoom in/out by moving the knob north or south.)
Use debug mode 7 ("DEBUG 7") or use for example your CAD program to verify changes.
In order to assign the zoom function to the Z axis (ie. pulling the knob up/pushing it down), you can switch the functionality between the translation Y and Z axis:

To switch the Y and Z axis, use the "SWITCHYZ" command from the serial monitor.
   SWITCHYZ <0|1>  - Switch the Y and Z axis. 1 = switchXY, 0 = do not switch (default).

Use the command "SWITCHYZ" (without any parameter) to show the current setting.
*/

// To set or overwrite the default settings from within config.h, you can use the following settings:
//
// #define SWITCHYZ 0      // 0 = do not switch Y and Z axis (default), 1 = switch Y and Z axis
//
// >>> Place your default values here. These values will be used if the EEPROM is empty or if the version number (SM_VERSION) is changed.

/* Eighth calibration - Exclusive mode       (command: EXCL | EXCL <0|1>)
==========================================================================
Exclusive mode only permits to send translation OR rotation, but never both at the same time.
This can solve issues with classic joysticks where you get unwanted translation or rotation at the same time.
Use Debug mode 7 ("DEBUG 7") or use for example your CAD program to verify changes.

It chooses to send the one with the biggest absolute value.

The exclusive mode can be enabled by using the command "EXCLUSIVEMODE" in the serial monitor.
   EXCL <0|1> - Enable or disable exclusive mode. 1 = enable, 0 = disable (default).

Use the command "EXCL" (without any parameter) to show the current setting.
*/

// To enable the Exclusive Mode as default from within config.h, you can uncomment the following settings:
//
// #define EXCLUSIVEMODE

/* Ninth calibration - Finetuning
==================================
This section allows for fine-tuning the sensitivity and response of the Spacemouse. Adjust the parameters below to achieve the desired performance.

After finishing the calibration, you can use the command "SHOW" in the serial monitor to see the current values of the parameters. You now can finetune the parameters to your liking.
Try using the Spacemouse in your favorite CAD program and check the behaviour. Finetune the parameters until you are satisfied with the performance. If you notice that moving the knob
in a certain direction, but has some (small) movement in the other direction, you can adjust sensitivity for a single axis (or even single axis direction) with the SENS command.

You can also add an additional gate value to the translation and rotation values. This is useful if you want to suppress small spurious movements in a certain direction.
This additional gate is applied to the translation or rotation axes.


*/

/* Key( ie. button) Configuration
==================================
If you attached keys to your Spacemouse, configure them in config.h. You cannot configure the keys from within the serial monitor.
You can use the keys to report them via USB HID to the PC (either classically pressed or emulated with a rotary encoder) or use them as kill-keys to suppress either translation or rotation (described below).

EXAMPLES
--------
+ Configruation example for NO KEYS
      NUMKEYS 0                        ( there are zero keys in total )
      KEY_PINLIST { }                  ( no keys are connected to the Arduino )
      NUMHIDKEYS 0                     ( No keys are reported to the PC )
      BUTTONLIST { }                   ( doesn't matter, as there are no keys at all )
      NUMKILLKEYS 0                    ( No keys for kill-keys )
      KILLROT 0 and KILLTRANS 0        ( doesn't matter, as there are no keys at all and NUMKILLKEYS 0 )


+ Configuration example for three usual buttons and no kill-keys
      NUMKEYS 3                        ( there are three keys in total )
      KEY_PINLIST {15, 14, 16}         ( the keys which shall be reported to the pc are connected to pin 15, 14 and 16)
      NUMHIDKEYS 3                     ( The first three keys from the KEY_PINLIST apply for the HID )
      BUTTONLIST {SM_FIT, SM_T, SM_R}  ( set the functions for the three buttons to "FIT", "T" and "R" )
      NUMKILLKEYS 0                    ( No keys for kill-keys )
      KILLROT 0 and KILLTRANS 0        ( doesn't matter, as NUMKILLKEYS 0 )

+ Configuration example for two usual buttons and two kill-keys:
      NUMKEYS 4                        ( there are four keys in total )
      KEY_PINLIST {15, 14, 16, 10}     ( the normal which shall be reported to the pc are connected to pin 15 and 14, the keys which shall be used to kill translation or rotation are connected to pin 16 and 10 )
      NUMHIDKEYS 2                     ( Therefore the first two keys from the KEY_PINLIST apply for the HID )
      BUTTONLIST {SM_3, SM_4}          ( set the functions for the two buttons to "3" and "4" )
      NUMKILLKEYS 2                    ( two keys are used as kill-keys)
      KILLROT 2                        ( the first kill key has the third position in the KEY_PINLIST and due to zero-based counting third-1 => KILLROT 2 )
      KILLTRANS 3                      ( the second kill key has the last position in the KEY_PINLIST with index 3 -> KILLTRANS 3 )

// TODO - Add example for rotary encoder keys

*/

// How many classic keys are there in total ? (0 = no keys, ie.feature disabled)
#define NUMKEYS 3 // 0

// Define the PINS for the classic keys on the Arduino
// The first pins from KEY_PINLIST may be reported via HID
#define KEY_PINLIST {0, 1, 2}

// How many keys are reported to the PC? Classic keys + ROTARY_KEYS in total.
#define NUMHIDKEYS 3 // 0

// In order to define which key is assigned to which button, the following list must be entered in the BUTTONLIST below
// TODO - It is technical possible to configure the button functionality from the Serial monitor. This is not implemented yet.
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

/* Important: BUTTONLIST must have at least as many elements as specified in NUMHIDKEYS */
// The keys from KEYLIST or ROTARY_KEYS are assigned to buttons here:
#define BUTTONLIST {SM_T, SM_R, SM_F}

// How many kill keys are there? (disabled: 0; enabled: 2)
#define NUMKILLKEYS 0

/* Note: Usually you take the last two buttons from KEY_PINLIST as kill-keys */
// Index of the kill key for rotation (array starts with index 0)
#define KILLROT 2
// Index of the kill key for translation (array starts with index 0)
#define KILLTRANS 3

/* Note: Technically you can report the kill-keys via HID as "usual" buttons, but that doesn't make much sense... */

/* Some simple tests for the definition of the keys */
#if (NUMKILLKEYS > NUMKEYS)
#error "Number of Kill Keys can not be larger than total number of keys"
#endif
#if (NUMKILLKEYS > 0 && ((KILLROT > NUMKEYS) || (KILLTRANS > NUMKEYS)))
#error "Index of killkeys must be smaller than the total number of keys"
#endif

// Time in ms which is needed to allow a new button press
#define DEBOUNCE_KEYS_MS 200

/* Encoder Wheel configuration
===============================
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
   Compare this number with the update frequency of the script, reported by DEBUG 8.
   If ECHOES = frequency: the zoom is faded for 1 second.
*/
#define ECHOES 200

/* Strength of the simulated pull
   Recommended range: 0 - 350
   Reason for max=350: The HID Interface reports logical max as +350, see hidInterface.h
   Recommended strength = 200
*/
#define SIMSTRENGTH 200
// TODO - Rotary axis, echoes and simstrenght can technically be set from the Serial monitor. This is not implemented yet.

/* ROTARY_KEYS configuration
============================
You can use the attached encoder to emulate a key stroke by turning the encoder.
*/

// Should the encoder be used as a key? (0 = no, 1 = yes)
#define ROTARY_KEYS 0

/* Which keys/functions as configured in the BUTTONLIST shall be emulated by the encoder? */
// First direction  (0 = first element from BUTTONLIST, 1 = second element, etc.)
#define ROTARY_KEY_IDX_A 2
// counter direction
#define ROTARY_KEY_IDX_B 3
// duration of simulated key
#define ROTARY_KEY_STRENGTH 19

/* LED support configuration
=============================
You can attach:
   a) a simple LED to the mouse. LED shall be connected to 5V and the controller port.
   b) a fancy LED strip, like the nanopixel. Check the FASTLED library for supported chips / led strips.

Which pin shall be used as LED? This pin is used either as a digital pin (for a simple LED) or as the data pin (for a fancy LED strip).
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
The connected LED is not just a simple LED, but an intelligent one, like a neopixel controlled by FASTLED library.
The LEDpin is used as a data pin. The LEDRING is used to define the number of LEDs on the ring.
*/

// #define LEDRING 24

// The LEDs light up, if a certain movement is reached:
#define VELOCITYDEADZONEFORLED 15

// About how many LEDs must the ring be turned to align?
#define LEDclockOffset 0

// How often shall the LEDs be updated (in ms)
#define LEDUPDATERATE_MS 150

/* Advanced - Debug output settings
===================================
The following settings allow customization of debug output behavior */

// Generate a debug line only every DEBUGDELAY ms
#define DEBUGDELAY 100

// The standard behavior "\r" for the debug output is, that the values are always written into the same line to get a clean output. Easy readable for the human.
#define DEBUG_LINE_END "\r"
// If you need to report some debug outputs to trace errors, you can change the debug output to "\r\n" to get a newline with each debug output. (old behavior)
// #define DEBUG_LINE_END "\r\n"

/* Advanced - USB HID settings
==============================
The following settings are advanced and don't need to be changed for normal usage (for Windows users).
*/

// Definition, how many bits are used in the HID report to encode the keys
#define HIDMAXBUTTONS 32 // must be multiple of 8!

/* ADV_HID_REL and ADV_HID_JIGGLE change how the values are reported over HID protocol, see hidInterface.cpp and .h

For Windows users: DON'T CHANGE / DON'T ENABLE THIS if you don't understand what it does.

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
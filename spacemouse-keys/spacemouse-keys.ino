// This is the source code for the open source space mouse with keys.
// Please read the introduction and history with all contributors here:
// https://github.com/AndunHH/spacemouse

// One good starting point is the work and video by TeachingTech: https://www.printables.com/de/model/864950-open-source-spacemouse-space-mushroom-remix
// Then follow along on github, how we reached this state of the source code.
#include <Arduino.h>

// The user specific settings, like pin mappings or special configuration variables and sensitivities are stored in config.h.
// Please open config_sample.h, adjust your settings and save it as config.h
#include "config.h"

// Include inbuilt Arduino HID library by NicoHood: https://github.com/NicoHood/HID
#include "HID.h"

// header file for calibration output and helper routines
#include "calibration.h"
// header to calculate the kinematics of the mouse
#include "kinematics.h"
// header file for reading the keys
#include "spaceKeys.h"
// header for HID emulation of the spacemouse
#include "SpaceMouseHID.h"

// header file for the EEPROM storage
#include "eepromStorage.h"

// header files for the hardware abstraction layers
#include "hardware/SpaceMouseHW_Hall.h"
#include "hardware/SpaceMouseHW_Joystick.h"

// Initialize the hardware abstraction layer
// The hardware abstraction layer is used to read the values from the hardware
#ifdef HALLEFFECT
SpaceMouseHW_Hall_ Mouse_Hardware = SpaceMouseHW_Hall_();
#else
SpaceMouseHW_Joystick Mouse_Hardware = SpaceMouseHW_Joystick_();
#endif

// Initialize the Kinematics object
// The Kinematics object is used to calculate the kinematics of the mouse
Kinematics Mouse_Kinematics = Kinematics();

#if ROTARY_AXIS > 0 or ROTARY_KEYS > 0
// if an encoder wheel is used
#include "encoderWheel.h"
#endif

#ifdef LEDpin
void lightSimpleLED(boolean light);
#endif

#ifdef LEDRING
#include "ledring.h"
#endif

// the debug mode can be set during runtime via the serial interface. See config.h for a description of the different debug modes.
int debug = STARTDEBUG;

// store raw value of the keys, without debouncing
int keyVals[NUMKEYS];

// key event, after debouncing. It is 1 only for a single sample
uint8_t keyOut[NUMKEYS];

// state of the key, which stays 1 as long as the key is pressed
uint8_t keyState[NUMKEYS];

long tmpInput; // store the value, the user might input over the serial

void setup() {
// setup the keys e.g. to internal pull-ups
#if NUMKEYS > 0
    setupKeys();
#endif

    // Begin Serial for debugging or calibration
    Serial.begin(250000);
    delay(100);
    Serial.setTimeout(2); // The serial interface will look for new debug values and it will only wait 2ms

    // Clear the EEPROM if the stored version number is not equal to the version number defined in the file eepromStorage.h
    _clearEEPROM();

    // Set the analog reference voltage according to the hardware.
    Mouse_Hardware.SetAnalogReferenceVoltage(debug);

    // Read idle/centre positions for joysticks.
    // zero the joystick position 500 times (takes approx. 480 ms)
    // during setup() we are not interested in the debug output: debugFlag = false
    Mouse_Hardware.BusyZeroing(500, false);

#if ROTARY_AXIS > 0 or ROTARY_KEYS > 0
    initEncoderWheel();
#endif
#ifdef LEDpin
#ifdef LEDRING
    initLEDring();
#else
    // configure LED output for simple LED
    pinMode(LEDpin, OUTPUT);
#endif
#endif
}

void loop() {
    // check if the user entered a debug mode via serial interface
    if (Serial.available()) {
        // Read the input command from the serial interface
        // The command format is: <char> <int> <int>
        // Example: A 100 200
        // The first character is the command, followed by two integer parameters
        // The command and parameters are separated by spaces
        // The command can be any character, but the parameters must be integers

        debugInput(debug, Mouse_Kinematics, Mouse_Hardware);

#if 0
        tmpInput = Serial.parseInt(); // Read from serial interface, if a new debug value has been sent. Serial timeout has been set in setup()
        if (tmpInput != 0) {

            if (tmpInput == 2222) {
                Mouse_Kinematics.PrintSensitivities(true);
            }

            /// The sensitivities can be updated with the command 2<dd><xxxx> (<dd> the sensitivity item to update - 2digit, <xxxx> the new value - 2.2 digit) - see config.h
// The inversions can be updated with the commands 400000x -405000x
#define SENSITIVITY_CMDRANGE_START 2000000L
#define SENSITIVITY_CMDRANGE_END 3000000L
#define SENSITIVITY_CMD_DIVIDER 10000L
            if (tmpInput >= SENSITIVITY_CMDRANGE_START && tmpInput < SENSITIVITY_CMDRANGE_END) {
                // Get the command
                // fe. input 4030001, cmd = (input/10000) = 403, minus (4000000L / 10000L) = 400 => 3
                uint16_t cmd_base = (tmpInput / SENSITIVITY_CMD_DIVIDER);
                uint8_t cmd = cmd_base - (SENSITIVITY_CMDRANGE_START / SENSITIVITY_CMD_DIVIDER);

                // Get integer value (last digit) and check if this is 0 or 1
                uint8_t value = tmpInput - (cmd_base * SENSITIVITY_CMD_DIVIDER);

                Mouse_Kinematics.SetSensitivities(cmd, value);
            }

            if (tmpInput == 10) {
                Mouse_Kinematics.PrintModulationFunction(true);
            }

/// The Kinematics modulation function can be updated with the command 300000x.
#define MODFUNC_CMDRANGE_START 3000000L
#define MODFUNC_CMDRANGE_END 3000010L
            if (tmpInput >= MODFUNC_CMDRANGE_START && tmpInput < MODFUNC_CMDRANGE_END) {
                // User input wants to update modfunc
                uint8_t requestedModfunc = (uint8_t)(tmpInput - MODFUNC_CMDRANGE_START);
                Mouse_Kinematics.SetModulationFunction(requestedModfunc);
            }

            if (tmpInput == 4444) {
                Mouse_Kinematics.PrintTransRotInversions(true);
            }

// The inversions can be updated with the commands 400000x -405000x
#define INVERSION_CMDRANGE_START 4000000L
#define INVERSION_CMDRANGE_END 5000000L
#define INVERSION_CMD_DIVIDER 10000L
            if (tmpInput >= INVERSION_CMDRANGE_START && tmpInput < INVERSION_CMDRANGE_END) {
                // Get the command
                // fe. input 4030001, cmd = (input/10000) = 403, minus (4000000L / 10000L) = 400 => 3
                uint16_t cmd_base = (tmpInput / INVERSION_CMD_DIVIDER);
                uint8_t cmd = cmd_base - (INVERSION_CMDRANGE_START / INVERSION_CMD_DIVIDER);

                // Get integer value (last digit) and check if this is 0 or 1
                uint8_t value = tmpInput - (cmd_base * INVERSION_CMD_DIVIDER);

                // User input wants to update axis inversion
                Mouse_Kinematics.SetTransRotInversions(cmd, value);
            }

            if (validCalibrationOption(tmpInput)) {
                debug = tmpInput;
                if (tmpInput == -1) {
                    Serial.println(F("Please enter the debug mode now or while the script is reporting."));
                }
                // Debug is updated check if the ADC referencevoltage has to be changed.
                Mouse_Hardware.SetAnalogReferenceVoltage(debug);
            }
        }
#endif
    }

    // RAW Sensor values are read. 0-1023
    Mouse_Hardware.ReadAllFromSensors();

#if NUMKEYS > 0
    // LivingTheDream added reading of key presses
    readAllFromKeys(keyVals);
#endif
    // Report back 0-1023 raw ADC 10-bit values if enabled
    if (debug == 1) {
        debugOutput1(Mouse_Hardware, keyVals);
        // Mouse_Hardware.PrintRawReads();
    }

    if (debug == 11) {
        // Calibrate the spacemouse
        // As this is called in the debug=11, we do more iterations.
        Mouse_Hardware.BusyZeroing(2000, true);
        debug = -1; // this only done once
    }

    Mouse_Hardware.CenterSensors();

    if (debug == 20) {
        Mouse_Hardware.CalcMinMax();
    }

    // Report centered joystick/knob values if enabled. Values should be approx -500 to +500, jitter around 0 at idle
    if (debug == 2) {
        // debugOutput2(centered);
        debugOutput2(Mouse_Hardware);
    }

    Mouse_Hardware.FilterAnalogReadOuts();

    if (debug == 3) {
        debugOutput2(Mouse_Hardware);
    }

    Mouse_Kinematics.CalculcateKinematic(Mouse_Hardware);

#if (ROTARY_AXIS > 0) && ROTARY_AXIS < 7
    // If an encoder wheel is used, calculate the velocity of the wheel and replace one of the former calculated velocities
    calcEncoderWheel(velocity, debug);
#endif

#if NUMKEYS > 0
    evalKeys(keyVals, keyOut, keyState);
#endif

#if ROTARY_KEYS > 0
    // The encoder wheel shall be treated as a key
    calcEncoderAsKey(keyState, debug);
#endif
    if (debug == 4) {
        // Report translation and rotation values if enabled.
        debugOutput4(Mouse_Kinematics, keyOut);
    }

    if (debug == 5) {
        debugOutput5(Mouse_Hardware, Mouse_Kinematics);
    }

    // if the kill-key feature is enabled, rotations or translations are killed=set to zero
#if (NUMKILLKEYS == 2)
    if (keyVals[KILLROT] == LOW) {
        // check for the raw keyVal and not keyOut, because keyOut is only 1 for a single iteration. keyVals has inverse Logic due to pull-ups
        // kill rotation
        velocity[ROTX] = 0;
        velocity[ROTY] = 0;
        velocity[ROTZ] = 0;
    }
    if (keyVals[KILLTRANS] == LOW) {
        // kill translation
        velocity[TRANSX] = 0;
        velocity[TRANSY] = 0;
        velocity[TRANSZ] = 0;
    }
#endif

    // report velocity and keys after possible kill-key feature
    if (debug == 6) {
        debugOutput4(Mouse_Kinematics, keyOut);
    }

#if SWITCHYZ > 0
    Mouse_Kinematics.SwitchYZ();
#endif

#ifdef EXCLUSIVEMODE
    // exclusive mode
    // rotation OR translation, but never both at the same time
    // to avoid issues with classics joysticks
    Mouse_Kinematics.ExclusiveMode();
#endif

    // report velocity and keys after Switch or ExclusiveMode
    if (debug == 61) {
        debugOutput4(Mouse_Kinematics, keyOut);
    }

    // get the values to the USB HID driver to send if necessary
    SpaceMouseHID.send_command(Mouse_Kinematics.GetVelocity(rotX),
                               Mouse_Kinematics.GetVelocity(rotY),
                               Mouse_Kinematics.GetVelocity(rotZ),
                               Mouse_Kinematics.GetVelocity(transX),
                               Mouse_Kinematics.GetVelocity(transY),
                               Mouse_Kinematics.GetVelocity(transZ),
                               keyState,
                               debug);

    if (debug == 7) {
        // update and report the at what frequency the loop is running
        updateFrequencyReport();
    }

#ifdef LEDpin
#ifdef LEDRING
    processLED(velocity, SpaceMouseHID.updateLEDState());
#else
    lightSimpleLED(SpaceMouseHID.updateLEDState());
    // Check for the LED state by calling updateLEDState.
    // This empties the USB input buffer and checks for the corresponding report.
#endif
#endif

} // end loop()

#ifdef LEDpin
  /// @brief Turn on or off a simple led. The pin is defined by LEDpin in config.h. If the LED needs to be inverted, define LEDinvert in config.h
/// @param light turn on or off
void lightSimpleLED(boolean light) {
// Check for the LED state by calling updateLEDState.
// This empties the USB input buffer and checks for the corresponding report.
#ifdef LEDinvert
    // Swap the LED logic, if necessary
    if (light)
#else
    if (!light)
#endif
    {
        // true -> LED on -> pull kathode down
        digitalWrite(LEDpin, LOW); // turn the LED o
    } else {
        // false -> LED off -> pull kathode up
        digitalWrite(LEDpin, HIGH); // turn the LED
    }
}
#endif

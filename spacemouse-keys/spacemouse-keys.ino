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

// Initialize the hardware abstraction layer
// The hardware abstraction layer is used to read the values from the hardware
#ifdef HALLEFFECT
#include "hardware/SpaceMouseHW_Hall.h"
SpaceMouseHW_Hall_ Mouse_Hardware = SpaceMouseHW_Hall_();
#else
#include "hardware/SpaceMouseHW_Joystick.h"
SpaceMouseHW_Joystick Mouse_Hardware = SpaceMouseHW_Joystick_();
#endif

// Initialize the Kinematics object
// The Kinematics object is used to calculate the kinematics of the mouse
Kinematics Mouse_Kinematics = Kinematics(Mouse_Hardware);

#if ROTARY_AXIS > 0 or ROTARY_KEYS > 0
// if an encoder wheel is used
#include "encoderWheel.h"
#endif

#ifdef LEDpin
void lightSimpleLED(boolean light);
#endif

#ifdef LEDRING
#include "ledring.h"
LedRing *Mouse_LEDRing;
#endif

// Initialize Calibration object - the calibration object knows of the hardware and the kinematics
// The calibration object is used to calibrate the hardware and the kinematics
Calibration Mouse_Calibration = Calibration(Mouse_Kinematics, Mouse_Hardware);

// FIXME - Remove the #If brackets. The pointer to SMKEYS has to be set in the constructor of the calibration class, otherwise several debug functions will explode if NUMKEYS=0
#if NUMKEYS > 0
SpaceKeys *Keys;
#endif

void setup() {
#if NUMKEYS > 0
    // Instantiate the keys object and setup the keys to internal pull-ups
    Keys = new SpaceKeys();

    // Notify the calibration object about the keys object (necessary for debug output)
    Mouse_Calibration.SetKeysObject(*Keys);
#endif

    // Begin Serial for debugging or calibration
    Serial.begin(250000);
    delay(100);
    Serial.setTimeout(2); // The serial interface will look for new debug values and it will only wait 2ms

    // Set the analog reference voltage according to the hardware.
    Mouse_Hardware.SetAnalogReferenceVoltage(Mouse_Calibration.GetDebug());

    // Check if this is the first run of the program. If so, set the default values for the sensitivities and store them in the EEPROM.

    // Read idle/centre positions for joysticks.
    // zero the joystick position 500 times (takes approx. 480 ms)
    // during setup() we are not interested in the debug output: debugFlag = false
    Mouse_Hardware.BusyZeroing(500, false);

#if ROTARY_AXIS > 0 or ROTARY_KEYS > 0
    initEncoderWheel();
#endif
#ifdef LEDpin
#ifdef LEDRING
    // Initialize the LED ring object
    Mouse_LEDRing = new LedRing(Mouse_Kinematics);
    // Initialize the LED ring with the number of LEDs and the pin number
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
        Mouse_Calibration.DebugInput();
    }

    // RAW Sensor values are read from the hardware. 0-1023
    Mouse_Hardware.ReadAllFromSensors();

#if NUMKEYS > 0
    // LivingTheDream added reading of key presses
    Keys->ReadAllFromKeys();
#endif
    // Report back 0-1023 raw ADC 10-bit values if enabled
    Mouse_Calibration.DebugOutput1_HW_RawInverted();

    // Center the read joystick/knob rawValues.
    // The centered values are the difference between the raw values and the centerpoint values, ie. the idle position.
    // F.e. if the center position is 500 and the read value is 400, the centered value will be -100.
    Mouse_Hardware.CenterSensors();

    // After centering the joystick/knob values, calibration of the min/max values can be executed.
    // The minmax calibration takes ~15 seconds and needs the loop() to continue running.
    // When the calibration is started, the processCalcMinMax() function is called to register the
    // min and max values of the sensors.
    Mouse_Hardware.ProcessCalcMinMax(); // --> Only executed if MIN/MAX calibration is started.

    // Report centered joystick/knob values if enabled. Values should be approx -500 to +500,
    // jitter around 0 when the knob is in idle position.
    Mouse_Calibration.DebugOutput2_HW_Centered();

    // The centered values are filtered for deadzone and mapped to the velocity range of -350 to +350.
    // The deadzone is the value that is used to filter out small movements of the joystick/knob.
    // The mapping is done to the velocity range of -350 to +350, which is the range of the HID interface
    Mouse_Hardware.FilterAnalogReadOuts();

    // Report output after deadzone filtering and mapping is applied. Values should be approx -350 to +350,
    Mouse_Calibration.DebugOutput3_HW_DeadzonedMapped();

    // The mouse hardware is finished, and the hardware can be translated to kinematics.
    // The kinematics are calculated based on the filtered values from the hardware.
    // The kinematics are the velocities of the mouse in the x, y and z direction and the rotation around the x, y and z axis.
    Mouse_Kinematics.CalculcateKinematic();

#if (ROTARY_AXIS > 0) && ROTARY_AXIS < 7
    // If an encoder wheel is used, calculate the velocity of the wheel and replace one of the former calculated velocities
    calcEncoderWheel(Mouse_Kinematics, Mouse_Calibration.GetDebug());
#endif

#if NUMKEYS > 0
    // Check if the keys are pressed and report the status of the keys
    // The keys are debounced and the status will be reported to the HID interface
    Keys->evalKeys();
#endif

#if ROTARY_KEYS > 0
    // The encoder wheel shall be treated as a key.
    calcEncoderAsKey(Keys, Mouse_Calibration.GetDebug());
#endif

    Mouse_Calibration.DebugOutput4_KIN_Velocity(); // Report translation & rotation values - without arithmetic functions, inversion, YZ switching or Exclusivemode

    Mouse_Calibration.DebugOutput5_HWKIN_CenteredAndVelocity(); // Report centered sensor values (debug 2) and translation & rotation values and keystatus (4th debug) side by side for direct reference.

    // If the kill-key feature is enabled, rotations or translations are killed (ie. set to zero)
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

    // Report sensor output, velocity and key status after possible kill-key feature
    Mouse_Calibration.DebugOutput6_HWKINKEY_CenteredAndVelocityAndKeystate();

    // Switch Y and Z axis if configured. This can be used to move zooming functionality in CAD programs from the transY to the trans Z axis.
    Mouse_Kinematics.ProcessSwitchYZ();

    // Rotation OR translation, but never both at the same time. To avoid issues with classics joysticks.
    Mouse_Kinematics.ProcessExclusiveMode();

    // Report sensor output, velocity and keys status after all arithmetic and configured functions are applied.
    Mouse_Calibration.DebugOutput7_HWKINKEY_CenteredAndVelocityAndKeystate();

    // get the values to the USB HID driver to send if necessary
    SpaceMouseHID.send_command(Mouse_Kinematics.GetVelocity(rotX),
                               Mouse_Kinematics.GetVelocity(rotY),
                               Mouse_Kinematics.GetVelocity(rotZ),
                               Mouse_Kinematics.GetVelocity(transX),
                               Mouse_Kinematics.GetVelocity(transY),
                               Mouse_Kinematics.GetVelocity(transZ),
                               Keys,
                               Mouse_Calibration.GetDebug());

    // Report the frequency at which the loop is running
    Mouse_Calibration.DebugOutput8_UpdateFrequencyReport();

#ifdef LEDpin
#ifdef LEDRING
    Mouse_LEDRing->ProcessLED(SpaceMouseHID.updateLEDState());
#else
    lightSimpleLED(SpaceMouseHID.updateLEDState());
    // Check for the LED state by calling updateLEDState.
    // This empties the USB input buffer and checks for the corresponding report.
#endif
#endif

} // end loop()

#ifdef LEDpin
/**
 * @brief Turn on or off a simple led. The pin is defined by LEDpin in config.h. If the LED needs to be inverted, define LEDinvert in config.h
 * @param light Turn led ON(true) or OFF(false)
 */
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

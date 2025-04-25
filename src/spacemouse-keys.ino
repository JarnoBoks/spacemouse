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

// Header to calculate the kinematics of the mouse
#include "kinematics/kinematics.h"

// header file for reading the keys
#include "spaceKeys.h"

// header for HID emulation of the spacemouse
#include "SpaceMouseHID.h"

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

// FIXME - Remove the #If brackets. The pointer to SMKEYS has to be set in the constructor of the calibration class, otherwise several debug functions will explode if NUMKEYS=0
#if NUMKEYS > 0
SpaceKeys *Keys;
#else
SpaceKeys *Keys = nullptr;
#endif

#include "commandhandler/commandhandler.h"
#include "commandhandler/debugcommand.h"
#include "commandhandler/showcommand.h"
CommandHandler myCommandHandler; // Command handler object to handle the commands from the serial interface

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

    // Setup the Kinematics object
    Kinematics::getInstance();

    // Setup the Command Handler and register the commands
    myCommandHandler.registerCommand(0, new ShowCommand());  // Register the show command
    myCommandHandler.registerCommand(1, new DebugCommand()); // Register the debug command

    // Check if this is the first run of the program. If so, set the default values for the sensitivities and store them in the EEPROM.

    // Read idle/centre positions for joysticks.
    // zero the joystick position 500 times (takes approx. 480 ms)
    // during setup() we are not interested in the debug output: debugFlag = false
    // Mouse_Hardware.BusyZeroing(500, false);

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
        // Read the input command from the serial interface and send it to the Command Handler
        myCommandHandler.parseSerialMonitorInput();
    }

    Kinematics *myKinematics = Kinematics::getInstance();
    myKinematics->processKinematics(); // Process the kinematics of the mouse

#if NUMKEYS > 0
    // LivingTheDream added reading of key presses
    Keys->ReadAllFromKeys();
#endif
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
#if 0
    // get the values to the USB HID driver to send if necessary
    SpaceMouseHID.send_command(Mouse_Kinematics.GetVelocity(rotX),
                               Mouse_Kinematics.GetVelocity(rotY),
                               Mouse_Kinematics.GetVelocity(rotZ),
                               Mouse_Kinematics.GetVelocity(transX),
                               Mouse_Kinematics.GetVelocity(transY),
                               Mouse_Kinematics.GetVelocity(transZ),
                               Keys,
                               Mouse_Calibration.GetDebug());
#endif
    SpaceMouseHID.send_command(Keys, 0);

    // Check for the LED state by calling updateLEDState.
    // This empties the USB input buffer and checks for the corresponding report.

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

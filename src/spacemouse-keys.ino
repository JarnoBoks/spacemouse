// This is the source code for the open source space mouse with keys.
// Please read the introduction and history with all contributors here:
// https://github.com/AndunHH/spacemouse

// One good starting point is the work and video by TeachingTech: https://www.printables.com/de/model/864950-open-source-spacemouse-space-mushroom-remix
// Then follow along on github, how we reached this state of the source code.
#include <Arduino.h>

// The user specific settings, like pin mappings or special configuration variables and sensitivities are stored in config.h.
// Please open config_sample.h, adjust your settings and save it as config.h
#include "config.h"

#ifdef ARDUINO_ARCH_AVR // For Arduino boards like Leonardo, Micro, etc.
// FIXME - The HID library is not compatible with the ESP32. The ESP32 uses the BLE HID library instead.
//  Include inbuilt Arduino HID library by NicoHood: https://github.com/NicoHood/HID
#include "HID.h"

// header for HID emulation of the spacemouse
#include "SpaceMouseHID.h"
#endif // ARDUINO_ARCH_AVR

// Definitions & Header for the various hardware types
// ------------------ PREPOCESSOR DIRECTIVES USED IN THE SOFTWARE - DO NOT CHANGE

#include "hardware/hardware_hall.h"
#include "hardware/hardware_joystick.h"

// FIXME - Something is not going ok with the preprocessor directives. The hardware type is not set correctly in the config.h file. The default values are not used in the hardware setup.
#ifdef JOYSTICK
#define HW_TYPE Hardware_JOYSTICK
// #define DEFAULTS_TYPE "defaults_joystick.h"
#endif

#ifdef HALLEFFECT
#define HW_TYPE Hardware_HALL
// #define DEFAULTS_TYPE "defaults_hall.h"
#endif

// Header to calculate the kinematics of the mouse
#include "kinematics/kinematics.h"

// header file for reading the keys
#include "spaceKeys.h"

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

// Include the header files for the command handler and the commands that can be received through the serial interface
#include "commandhandler/commandhandler.h"
#include "commandhandler/debugcommand.h"
#include "commandhandler/idlecommand.h"
#include "commandhandler/minmaxcommand.h"
#include "commandhandler/senscommand.h"
#include "commandhandler/gatecommand.h"
#include "commandhandler/modfunccommand.h"
#include "commandhandler/invertcommand.h"
#include "commandhandler/showcommand.h"
#include "commandhandler/exclusivecommand.h"
#include "commandhandler/switchyzcommand.h"
CommandHandler myCommandHandler; // Command handler object to handle the commands from the serial interface

#include "calibration/sensorcalibrationmanager.h"

// #include <ArduinoShrink.h>
void setup() {
    millis(); // Wait for the serial interface to be ready
#if NUMKEYS > 0
    // Instantiate the keys object and setup the keys to internal pull-ups
    Keys = new SpaceKeys();

    // Notify the calibration object about the keys object (necessary for debug output)
    Mouse_Calibration.SetKeysObject(*Keys);
#endif

    // Begin Serial for debugging or calibration
    Serial.begin(250000);
    delay(100);
    Serial.setTimeout(2); // The serial interface will look for new commands and it will only wait 2ms

    // Setup the Hardware object. This will setup the hardware and sensors of the mouse. The hardware type is defined in config.h
    HW_TYPE::getInstance();

    // Setup the Kinematics object. This will setup the kinematic axes of the mouse.
    // The setup will check the EEPROM for the configuration of the sensors and the axes.
    // If the configuration is not available, the default values as set in config.h will be used (and stored in the EEPROM)
    Kinematics::getInstance();

    // Start the idle calibration of the sensors. This will zero the sensors during the loop.
    // TODO - During setup we aren't interested in the output of the calibration process.
    // TODO - We do not want to send output to the HID while the calibration isn't finished.
    SensorCalibrationManager::getInstance()->activateIdleCalibration(500); // Start the idle calibration with 500 iterations

    // Setup the Command Handler and register the commands that can be handled via the serial interface
    myCommandHandler.registerCommand(0, new DebugCommand());
    myCommandHandler.registerCommand(1, new IdleCommand());
    myCommandHandler.registerCommand(2, new MinMaxCommand());
    myCommandHandler.registerCommand(3, new SensCommand());
    myCommandHandler.registerCommand(4, new GateCommand());
    myCommandHandler.registerCommand(5, new ModFuncCommand());
    myCommandHandler.registerCommand(6, new InvertCommand());
    myCommandHandler.registerCommand(7, new ShowCommand());
    myCommandHandler.registerCommand(8, new ExclusiveCommand());
    myCommandHandler.registerCommand(9, new SwitchYZCommand());

    // When debugging with SimAVR thorugh PlatformIO the serial monitor is not available. The command handler will not be able to parse the input from the serial monitor.
    // Use this comamnd to initialize a debug state.
    // char buffer[32] = "DEBUG 1";
    // myCommandHandler.handleInput(buffer, 32, 1);

#if ROTARY_AXIS > 0 or ROTARY_KEYS > 0
    initEncoderWheel();
#endif
#ifdef LEDpin
#ifdef LEDRING
    // Initialize the LED ring object
    Mouse_LEDRing = new LedRing();
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

    Kinematics::getInstance()->processKinematics(); // Process the kinematics of the mouse

// TESTING
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
#ifdef ARDUINO_ARCH_AVR
    // FIXME - ESP32 does not support the HID library. The HID library is not compatible with the ESP32. The ESP32 uses the BLE HID library instead.
    SpaceMouseHID.send_command(Keys, 0);
#endif
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

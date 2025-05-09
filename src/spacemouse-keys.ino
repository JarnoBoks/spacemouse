// This is the source code for the open source space mouse with keys.
// Please read the introduction and history with all contributors here:
// https://github.com/AndunHH/spacemouse
// One good starting point is the work and video by TeachingTech: https://www.printables.com/de/model/864950-open-source-spacemouse-space-mushroom-remix
// Then follow along on github, how we reached this state of the source code.

#include <Arduino.h>

// The user specific settings, like pin mappings or special configuration variables and sensitivities are stored in config.h.
// Please open config_sample.h, adjust your settings and save it as config.h
#include "config.h"

#ifdef ARDUINO_ARCH_AVR
// Include header files for the HID interface
#include <hidhandler/SpaceMouseHID.h> // Include the HID interface header
SpaceMouseHID *mySpaceMouseHID;
#else
// FIXME - The HID library is not compatible with the ESP32. The ESP32 uses the BLE HID library instead.
#endif // ARDUINO_ARCH_AVR

#if defined(HW_JOYSTICK)
#define HW_TYPE Hardware_JOYSTICK // REFACTOR - Change casing /naming convention to match the other files
#elif defined(HW_JOYSTICK)
#elif defined(HW_HALLEFFECT)
#define HW_TYPE Hardware_HALL // REFACTOR - Change casing /naming convention to match the other files
#else
#error "No hardwaretype defined, define HW_HALLEFFECT or HW_JOYSTICK in config.h"
#endif

// Header to calculate the kinematics of the mouse
#include "kinematics/kinematics.h"

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

// Include the header file for the key factory & collection
#include "key/factory/KeyFactory.hpp"
#include "key/KeyCollection.hpp"
KeyCollection myKeyCollection; // Key collection object to hold the keys and the key configuration (initialized empty)

// Include the header file for the sensor factory & collection
// REVIEW #include "sensor/factory/SensorFactory.hpp" // Include the sensor factory header file
#include "sensor/SensorCollection.hpp" // Include the sensor collection header file
SensorCollection mySensorCollection;   // Sensor collection object to hold the sensors and the sensor configuration (initialized empty)

// Include the header file for the Hardware objects (Interface between sensors and the axis collection)
#ifdef HW_HALLEFFECT
#include "sensorscalculator/SensorsCalculatorHall.hpp"         // Include the header file for the Hall effect sensors
SensorsCalculatorHall mySensorCalculator(&mySensorCollection); // Sensor calculator object to calculate the sensor values
#else
#include "sensorscalculator/SensorsCalculatorJoystick.hpp" // Include the header file for the joystick sensors
SensorsCalculatorJoystick mySensorCalculator(&mySensorCollection); // Sensor calculator object to calculate the sensor values
#endif

// Include the header file for the axis collection
#include "axis/AxisCollection.hpp" // Include the axis collection header file
#include "axis/axes/Axis.hpp"      // Include the axis collection header file
AxisCollection myAxisCollection;   // Axis collection object to hold the axes and the axis configuration (initialized empty)

// Include the header files for the HID commands
#include "hidhandler/commands/HIDCommandStoreKeyPress.hpp"

// Include the header files for the Translators between the commands send by the axis and keys towards the HID interface
#include "hidhandler/translator/TranslatorKeys.h"
TranslatorKeys myTranslatorKeys; // Translator object to translate the commands from the keys to the HID interface

// Include the header files for the command handler that will handle the commands send by the serial interface
#include "commandhandler/commandhandler.h"

// Include the header files for the commands that can be received through the serial interface
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
#include "commandhandler/bootloadercommand.h"
CommandHandler myCommandHandler; // Command handler object to handle the commands from the serial interface

// Include the header file for the calibration manager (used to calibrate center position of the sensors on startup)
#include "sensor/calibration/SensorCalibrationManagerIdle.hpp" // Include the sensor calibration manager header file
SensorCalibrationManagerIdle *mySensorCalibrationManagerIdle;  // Sensor calibration manager object to handle the calibration of the sensors

void cstmDelay(unsigned long ms) {
    // This function is used to delay the program for a certain amount of time.
    // It is used to wait for the serial interface to be ready.
    // We could use delay(ms), but this costs another 100bytes in the program size.
    unsigned long now = millis(); // Get the current time
    while (millis() - now < ms) {
        // Wait for the specified amount of time
    }
}

#include "hidhandler/usbinterface/SpaceMouseUSBInterface.h" // Include the HID interface header
// #include <ArduinoShrink.h>
void setup() {

    cstmDelay(100); // Wait for the serial interface to be ready

    // Begin Serial for debugging or calibration
    Serial.begin(250000);
    cstmDelay(100);       // Wait for the serial interface to be ready
    Serial.setTimeout(2); // The serial interface will look for new commands and it will only wait 2ms

    // Setup the Sensor collection. This will setup the sensors and load or create the sensor configuration.
    mySensorCollection.setup();

    // Setup the Axis collection. This will setup the axes and the axis configuration.
    myAxisCollection.setup(&mySensorCalculator); // Setup the axis collection with the sensor calculator

    // Setup the Kinematics object. This will setup the kinematic axes of the mouse.
    // The setup will check the EEPROM for the configuration of the sensors and the axes.
    // If the configuration is not available, the default values as set in config.h will be used (and stored in the EEPROM)
    // FIXME - Kinematics should be removed
    Kinematics::getInstance()->setAxisCollection(&myAxisCollection); // Set the axis collection for the kinematics object

    // FIXME - For now a manual start. Should be done automatically.
    SpaceMouseUSBInterface_::getInstance();

    // Call the setup function of the button factory. This will setup the buttons and the button configuration.
    // REVIEW - Not necessary for now: KeyFactory::getInstance()->setupKeys(); // Updated from setupButtons() to setupKeys()

    // Start the idle calibration of the sensors. This will zero the sensors during the loop.
    // TODO - During setup we aren't interested in the output of the calibration process.
    // TODO - We do not want to send output to the HID while the calibration isn't finished.
    // FIXME - Cleanup the calibration manager when the calibration is finished.
    mySensorCalibrationManagerIdle = new SensorCalibrationManagerIdle(&mySensorCollection); // Initialize the sensor calibration manager
    mySensorCalibrationManagerIdle->activate();                                             // Start the idle calibration with 500 iterations

    //  Setup the Command Handler and register the commands that can be handled via the serial interface.
    myCommandHandler.registerCommand(new DebugCommand(&mySensorCollection));
    myCommandHandler.registerCommand(new IdleCommand());
    myCommandHandler.registerCommand(new MinMaxCommand(&mySensorCollection));
    myCommandHandler.registerCommand(new SensCommand());
    myCommandHandler.registerCommand(new GateCommand());
    myCommandHandler.registerCommand(new ModFuncCommand());
    myCommandHandler.registerCommand(new InvertCommand());
    myCommandHandler.registerCommand(new ShowCommand());
    myCommandHandler.registerCommand(new ExclusiveCommand());
    myCommandHandler.registerCommand(new SwitchYZCommand());
    myCommandHandler.registerCommand(new BootloaderCommand());

    // When debugging with SimAVR thorugh PlatformIO the serial monitor is not available. The command handler will not be able to parse the input from the serial monitor.
    // Use this command to initialize a debug state.
    // char buffer[32] = "DEBUG 1";
    // myCommandHandler.handleInput(buffer, 32, 1);

    // Populate the key collection with the keys that are configured in config.h
    myKeyCollection.setup(); // Setup the keys for the key collection, based on the configuration in config.h

#if 0
    cstmDelay(7500); // Debugging: give the user some time to open the serial monitor and start the debugging process
    KeyCollection tstCollection;
#endif
    // REVIEW - Can we fall back to a solution without the "new" operator and just setup a 'global' variable?
    mySpaceMouseHID = new SpaceMouseHID(); // Initialize the HID interface

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
    // Check if the user entered a command through the Serial monitor
    if (Serial.available()) {
        myCommandHandler.parseSerialMonitorInput();
    }

    // Update all the sensor values & apply the calibration to the read sensor values & notify collection observers
    mySensorCollection.evaluate();

    // Calculate from sensor data and apply all config- & calibration settings to the axis values
    myAxisCollection.evaluate();

#if (ROTARY_AXIS > 0) && ROTARY_AXIS < 7
    // If an encoder wheel is used, calculate the velocity of the wheel and replace one of the former calculated velocities
    calcEncoderWheel(Mouse_Kinematics, Mouse_Calibration.GetDebug());
#endif

    // Evaluate the status of the keys
    myKeyCollection.evaluate();

#if ROTARY_KEYS > 0
    // The encoder wheel shall be treated as a key.
    calcEncoderAsKey(Keys, Mouse_Calibration.GetDebug());
#endif

#ifdef ARDUINO_ARCH_AVR
    // REVIEW - The HID library is not compatible with the ESP32. The ESP32 uses the BLE HID library instead.
    mySpaceMouseHID->execute();
#endif

    // Check for the LED state by calling updateLEDState.
    // This empties the USB input buffer and checks for the corresponding report.

#if 0
// FIXME - LedState has to be set in the HID interface.
#ifdef LEDpin
#ifdef LEDRING
    Mouse_LEDRing->ProcessLED(SpaceMouseHID.updateLEDState());
#else
    lightSimpleLED(SpaceMouseHID.updateLEDState());
    // Check for the LED state by calling updateLEDState.
    // This empties the USB input buffer and checks for the corresponding report.
#endif
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

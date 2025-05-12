// This is the source code for the open source space mouse with keys.
// Please read the introduction and history with all contributors here:
// https://github.com/AndunHH/spacemouse
// One good starting point is the work and video by TeachingTech: https://www.printables.com/de/model/864950-open-source-spacemouse-space-mushroom-remix
// Then follow along on github, how we reached this state of the source code.

#include <Arduino.h>

// The user specific settings, like pin mappings or special configuration variables and sensitivities are stored in config.h.
// Please open config_sample.h, adjust your settings and save it as config.h
#include "config.h"

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
#include "key/KeyCollection.hpp"
KeyCollection myKeyCollection; // Key collection object to hold the keys and the key configuration (initialized empty)

// Include the header file for the sensor factory & collection
#include "sensor/SensorCollection.hpp" // Include the sensor collection header file
SensorCollection mySensorCollection;   // Sensor collection object to hold the sensors and the sensor configuration (initialized empty)

// Include the header file for the calculation from sensors to the axes, this is hardware specific.
#ifdef HW_HALLEFFECT
#include "sensorscalculator/SensorsCalculatorHall.hpp"
SensorsCalculatorHall mySensorCalculator(&mySensorCollection);
#else
#include "sensorscalculator/SensorsCalculatorJoystick.hpp"
SensorsCalculatorJoystick mySensorCalculator(&mySensorCollection);
#endif

// Include the header file for the Axis and the Axis collection
#include "axis/AxisCollection.hpp" // Include the axis collection header file
AxisCollection myAxisCollection;   // Axis collection object to hold the axes and the axis configuration (initialized empty)

// Include the header files for the HID commands
#include "hidhandler/commands/HIDCommandStoreKeyPress.hpp"

// Include the header files for the command handler that will handle the commands send by the serial interface
#include "commandhandler/commandhandler.h"
#include "commandhandler/collectionidentifier/CollectionIdentifier.hpp"

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
// NOTE  #include "commandhandler/bootloadercommand.h"
CommandHandler myCommandHandler;                                                              // Command handler object to handle the commands from the serial interface
CollectionIdentifier myCollections(&mySensorCollection, &myAxisCollection, &myKeyCollection); // Collection identifier object to identify the collection of the command

// Include the header file for the calibration manager (used to calibrate center position of the sensors on startup)
#include "sensor/calibration/SensorCalibrationManagerIdle.hpp" // Include the sensor calibration manager header file
SensorCalibrationManagerIdle *mySensorCalibrationManagerIdle;  // Sensor calibration manager object to handle the calibration of the sensors

// Include the header file for the HID Event Buffer (used as interface between Axis & Keys and the HID Handler)
#include "observers/HIDEventBuffer/HIDEventBufferKeys.hpp"
#include "observers/HIDEventBuffer/HIDEventBufferRotation.hpp"
#include "observers/HIDEventBuffer/HIDEventBufferTranslation.hpp"
HIDEventBufferKeys myHIDEventBufferKeys;
HIDEventBufferRotation myHIDEventBufferRotation;
HIDEventBufferTranslation myHIDEventBufferTranslation;

#ifdef ARDUINO_ARCH_AVR
#include "hidhandler/usbinterface/SpaceMouseUSBInterface.h"
#include "hidhandler/HIDHandlerController.h"
#include "hidhandler/SpaceMouseHID.h"
SpaceMouseHID mySpaceMouseHID;
#endif

#include "common/CustomDelay.h" // Include the custom delay header
#include "common/FreeRAM.h"     // Include the free RAM header

// #include <ArduinoShrink.h>
void setup() {

    CustomDelay::delay(100); // Wait for the serial interface to be ready

    // Begin Serial for debugging or calibration
    Serial.begin(250000);
    CustomDelay::delay(100); // Wait for the serial interface to be ready
    Serial.setTimeout(2);    // The serial interface will look for new commands and it will only wait 2ms

    CustomDelay::delay(7000); // Wait for the serial interface to be ready
    //  Setup the Sensor collection. This will setup the sensors and load or create the sensor configuration.
    mySensorCollection.setup();

    // Setup the Axis collection. This will setup the axes and the axis configuration, and attaches the HID event buffers.
    // TODO - Create a AxisFactory that will create the axes based on the configuration.
    myAxisCollection.setup(&mySensorCalculator, &myHIDEventBufferTranslation, &myHIDEventBufferRotation); // Setup the axis collection with the sensor calculator

    // Populate the key collection with the keys that are configured in config.h
    // FIXME myKeyCollection.setup(); // Setup the keys for the key collection, based on the configuration in config.h

    // FIXME myKeyCollection.attachKeyObserver(&myHIDEventBuffer);

    // Setup the Kinematics object. This will setup the kinematic axes of the mouse.
    // The setup will check the EEPROM for the configuration of the sensors and the axes.
    // If the configuration is not available, the default values as set in config.h will be used (and stored in the EEPROM)
    // FIXME - Kinematics should be removed
    // FIXME Kinematics::getInstance()->setAxisCollection(&myAxisCollection); // Set the axis collection for the kinematics object

#ifdef ARDUINO_ARCH_AVR
    // FIXME - For now a manual start. Should be done automatically.
    SpaceMouseUSBInterface_::getInstance();
#endif

    // Call the setup function of the button factory. This will setup the buttons and the button configuration.
    // REVIEW - Not necessary for now: KeyFactory::getInstance()->setupKeys(); // Updated from setupButtons() to setupKeys()

    //  Setup the Command Handler and register the commands that can be handled via the serial interface.
    // NOTE: Memory wise is is allowed to allocate memory Dynamically, while the commands will never be deleted.
    // REVIEW - The entire commmand handler uses ~450 bytes of RAM, mainly due to the vtables for the command & command parameter classes.
    myCommandHandler.registerCommand(new DebugCommand(&myCollections));
    myCommandHandler.registerCommand(new IdleCommand(&myCollections));
    myCommandHandler.registerCommand(new MinMaxCommand(&myCollections));
    myCommandHandler.registerCommand(new SensCommand());
    myCommandHandler.registerCommand(new GateCommand());
    myCommandHandler.registerCommand(new ModFuncCommand());
    myCommandHandler.registerCommand(new InvertCommand());
    myCommandHandler.registerCommand(new ShowCommand());
    myCommandHandler.registerCommand(new ExclusiveCommand());
    myCommandHandler.registerCommand(new SwitchYZCommand());
#if SIMULATOR_DEBUGGING
    // When debugging with SimAVR through PlatformIO the serial monitor is not available.
    // Use this line to initialize a debug state if necessary and the corresponding output.
    char buffer[32] = "DEBUG 1";
    myCommandHandler.handleInput(buffer, 32, 1);
#endif

#ifdef ARDUINO_ARCH_AVR
    // Connect the HID interface to the axes and keys
    mySpaceMouseHID.getController()->setHIDEventBufferKeys(&myHIDEventBufferKeys);               // Connect the HID event buffer to the HID interface
    mySpaceMouseHID.getController()->setHIDEventBufferRotation(&myHIDEventBufferRotation);       // Connect the HID event buffer to the HID interface
    mySpaceMouseHID.getController()->setHIDEventBufferTranslation(&myHIDEventBufferTranslation); // Connect the HID event buffer to the HID interface
#endif
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

void setup2() {
    // This function is called after the setup() function. It is used to initialize the HID interface and the key collection.
    // The HID interface is used to send the data to the computer and the key collection is used to handle the keys.
    // The setup2() function is called after the setup() function to allow for a delay before starting the HID interface.
    // This is useful for debugging purposes, as it allows for a delay before starting the HID interface.
    Serial.println(F("Setup2() called!")); // Print a message to the serial monitor
    FreeRAM::display_freeram();
    // Start the idle calibration of the sensors. This will zero the sensors during the loop.
    // TODO - During setup we aren't interested in the output of the calibration process.
    // TODO - We do not want to send output to the HID while the calibration isn't finished.
    // FIXME - Cleanup the calibration manager when the calibration is finished.
    mySensorCalibrationManagerIdle = new SensorCalibrationManagerIdle(&mySensorCollection); // Initialize the sensor calibration manager
    mySensorCalibrationManagerIdle->activate();                                             // Start the idle calibration with 500 iterations
    FreeRAM::display_freeram();                                                             // Print the free RAM to the serial monitor
    Serial.println(F("Setup done!"));                                                       // Print a message to the serial monitor
}

bool firstrun = true; // Flag to check if the setup2() function has been called
void loop() {
    if (firstrun) {
        setup2();         // Call the setup2() function to initialize the HID interface and the key collection
        firstrun = false; // Set the flag to false to prevent calling the setup2() function again
    }
    // FreeRAM::display_freeram(); // Print the free RAM to the serial monitor
    //  Check if the user entered a command through the Serial monitor
    if (Serial.available()) {
        myCommandHandler.parseSerialMonitorInput();
    }

    // Update all the sensor values & apply the calibration to the read sensor values & notify collection observers
    mySensorCollection.evaluate();

    // Calculate from sensor data and apply all config- & calibration settings to the axis values & notify collection observers
    myAxisCollection.evaluate();

#if (ROTARY_AXIS > 0) && ROTARY_AXIS < 7
    // If an encoder wheel is used, calculate the velocity of the wheel and replace one of the former calculated velocities
    calcEncoderWheel(Mouse_Kinematics, Mouse_Calibration.GetDebug());
#endif

    // Evaluate the status of the keys & notify collection observers
    myKeyCollection.evaluate();

#if ROTARY_KEYS > 0
    // The encoder wheel shall be treated as a key.
    calcEncoderAsKey(Keys, Mouse_Calibration.GetDebug());
#endif

#ifdef ARDUINO_ARCH_AVR
    mySpaceMouseHID.execute();
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

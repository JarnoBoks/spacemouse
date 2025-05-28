// This is the source code for the open source space mouse with keys.
// Please read the introduction and history with all contributors here:
// https://github.com/AndunHH/spacemouse
// One good starting point is the work and video by TeachingTech: https://www.printables.com/de/model/864950-open-source-spacemouse-space-mushroom-remix
// Then follow along on github, how we reached this state of the source code.

#include <Arduino.h>

// The user specific settings, like pin mappings or special configuration variables and sensitivities are stored in config.h.
// Please open config_sample.h, adjust your settings and save it as config.h
#include "config.h"

// Architecture specific includes
#include <sensor/sensors/ArchitectureADC.hpp> // Include the architecture specific ADC functions

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

// Include the header file for the KnobAxis and the KnobAxisCollection
#include <knob/KnobAxisCollection.hpp> // Include the axis collection header file
KnobAxisCollection myKnobAxes;         // KnobAxisCollection object to hold the axes and the axis configuration (initialized empty)

// Header to calculate the kinematics of the mouse
#include "kinematics/Kinematics.hpp"
Kinematics myKinematics(&myKnobAxes); // Kinematics object to evaluate the kinematics of the mouse

// Include the header files for the command handler that will handle the commands send by the user through the serial monitor.
// For the ESP32 and AVR architecture, the command handler is different.
#include "commandhandler/factory/CommandHandlerFactory.hpp"
#ifdef ARDUINO_ARCH_ESP32
#include "commandhandler/EspCommandHandler/EspCommandHandler.hpp"
#endif
#ifdef ARDUINO_ARCH_AVR
#include "commandhandler/AvrCommandHandler/AvrCommandHandler.hpp"
#endif
CommandHandler *myCommandHandler; // Command handler object to handle the commands from the serial interface

// Include the header file for the collections carrier
#include "commandhandler/CollectionCarrier/CollectionCarrier.hpp"
CollectionCarrier myCollections(&mySensorCollection, &myKnobAxes, &myKeyCollection, &myKinematics); // Collection identifier object to identify the collection of the command

// Include the header file for the calibration manager (used to calibrate center position of the sensors on startup)
#include "sensor/calibration/SensorCalibrationManagerIdle.hpp" // Include the sensor calibration manager header file
SensorCalibrationManagerIdle *mySensorCalibrationManagerIdle;  // Sensor calibration manager object to handle the calibration of the sensors

// Include the header file for the HID Event Buffer (used as interface between KnobAxis & Keys and the HID Handler)
#include "observers/HIDEventBuffer/HIDEventBufferKeys.hpp"
#include "observers/HIDEventBuffer/HIDEventBufferRotation.hpp"
#include "observers/HIDEventBuffer/HIDEventBufferTranslation.hpp"
HIDEventBufferKeys myHIDEventBufferKeys;
HIDEventBufferRotation myHIDEventBufferRotation;
HIDEventBufferTranslation myHIDEventBufferTranslation;

// Include the header file for the HID handler (used to handle the HID events)
#include "hidhandler/HIDHandlerController.h"
#include "hidhandler/SpaceMouseHID.h"
SpaceMouseHID mySpaceMouseHID;

// Include the header file for the EEPROM storage (used to store the configuration of the sensors and axes)
#include "eeprom/eepromstore.h"

#include "common/CustomDelay.h" // Include the custom delay header
#include "common/FreeRAM.h"     // Include the free RAM header

// Include the header file for the USB interface (used to connect to the computer)
#include "usbstack/USBInterface.hpp"

// Include the header file for the WiFi manager (used to connect to WiFi and handle OTA updates)
#include "wifi/WifiManager.h"

void setup() {

    // Setup USB, WiFi and OTA
    USBStart;
    WifiManager::setup_Wifi(); // Setup the WiFi connection (only if ESP32 and if configured in config.h)
    WifiManager::setup_OTA();  // Setup the OTA connection (only if ESP32 and if selected environment)

    ArchitectureADC::setupADC(); // Initialize the ADC for the architecture (ESP32 or AVR)

    CustomDelay::delay(100); // Wait for the serial interface to be ready
    // Begin Serial for debugging or calibration
    Serial.begin(250000);
    CustomDelay::delay(100); // Wait for the serial interface to be ready
    Serial.setTimeout(2);    // The serial interface will look for new commands and it will only wait 2ms
    CustomDelay::delay(100); // Wait for CPU to start all peripherals

    //  Setup the Sensor collection. This will setup the sensors and load or create the sensor configuration.
    mySensorCollection.setup();

    // Setup the KnobAxis collection. This will setup the axes and the axis configuration, and attaches the HID event buffers.
    myKnobAxes.setup(&mySensorCalculator,
                     &myHIDEventBufferTranslation,
                     &myHIDEventBufferRotation);

    // Populate the key collection with the keys that are configured in config.h
    myKeyCollection.setup(); // Setup the keys for the key collection, based on the configuration in config.h
    myKeyCollection.attachKeysObserver(&myHIDEventBufferKeys);

    CommandHandlerFactory myCommandHandlerFactory(&myCollections);     // Create the command handler factory
    myCommandHandler = myCommandHandlerFactory.createCommandHandler(); // Create the command handler object
    myCommandHandlerFactory.setupCommandHandler(myCommandHandler);     // Setup the command handler and register the commands
#if SIMULATOR_DEBUGGING
    // When debugging with SimAVR through PlatformIO the serial monitor is not available.
    // Use this line to initialize a debug state if necessary and the corresponding output.
    char buffer[32] = "DEBUG 1";
    myCommandHandler->handleInput(buffer, 32, 1);
#endif

    // Start the idle calibration of the sensors. This will zero the sensors during the loop.
    mySensorCalibrationManagerIdle = new SensorCalibrationManagerIdle(&mySensorCollection); // Initialize the sensor calibration manager
    mySensorCalibrationManagerIdle->activate();                                             // Start the idle calibration with 500 iterations

    // Connect the HID interface to the axes and keys
    mySpaceMouseHID.getController()->setHIDEventBufferKeys(&myHIDEventBufferKeys);               // Connect the HID event buffer to the HID interface
    mySpaceMouseHID.getController()->setHIDEventBufferRotation(&myHIDEventBufferRotation);       // Connect the HID event buffer to the HID interface
    mySpaceMouseHID.getController()->setHIDEventBufferTranslation(&myHIDEventBufferTranslation); // Connect the HID event buffer to the HID interface

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
    WifiManager::handle_OTA(); // Handle the OTA connection (only if configured in platformio.ini)

    //  Check if the user entered a command through the Serial monitor
    if (Serial.available()) {
        myCommandHandler->parseSerialMonitorInput();
    }

    // Update all the sensor values & apply the calibration to the read sensor values & notify collection observers
    mySensorCollection.evaluate();
    // Calculate from sensor data and apply all config- & calibration settings to the axis values & notify collection observers
    myKnobAxes.evaluate();

#if (ROTARY_AXIS > 0) && ROTARY_AXIS < 7
    // If an encoder wheel is used, calculate the velocity of the wheel and replace one of the former calculated velocities
    calcEncoderWheel(Mouse_Kinematics, Mouse_Calibration.GetDebug());
#endif

    // Evaluate the status of the keys & notify collection observers
    myKeyCollection.evaluate();

#if ROTARY_KEYS > 0
    // The encoder wheel shall be treated as a key.
    calcEncoderAsKey(Keys, Moue_Calibration.GetDebug());
#endif

    // Decorators for the Axes / Keys (SwitchYZ, ExclusiveMode)
    myKinematics.evaluate();

    mySpaceMouseHID.execute();

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

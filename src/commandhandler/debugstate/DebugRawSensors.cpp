#include "DebugRawSensors.h"
#include <Arduino.h>
#include "serialoutput/output_rawsensorvalues.h"
#include "config.h" // For Hardware type
#include "hardware/hardware_hall.h"
#include "hardware/hardware_joystick.h"

#ifdef HALLEFFECT
#define HW_TYPE Hardware_HALL
#endif
#ifdef JOYSTICK
#define HW_TYPE Hardware_Joystick
#endif

/**
 * @brief Constructor to initialize the DebugRawSensors class
 * @details This constructor initializes the rawSensorObserver pointer to nullptr.
 *          It is used to create an instance of the DebugRawSensors class.
 *          The constructor does not take any parameters and does not perform any operations.
 *          It is a simple constructor that initializes the class member variables to their default values.
 */
DebugRawSensors::DebugRawSensors() : IDebugState(), rawSensorObserver(nullptr) {} // Constructor to initialize currentLevel

/**
 * @brief Destructor to clean up the observer instance
 * @details This destructor checks if the rawSensorObserver pointer is not null.
 *          If it is not null, it detaches the observer from the hardware and deletes the observer instance.
 *          This ensures that the observer is properly cleaned up and does not cause memory leaks.
 */
DebugRawSensors::~DebugRawSensors() {
    if (rawSensorObserver != nullptr) {
        Hardware *hardware = HW_TYPE::getInstance(); // Get the hardware instance
        hardware->detachObserver(rawSensorObserver); // Detach the observer from the hardware
        delete rawSensorObserver;                    // Clean up the observer instance
        rawSensorObserver = nullptr;
    }
}

void DebugRawSensors::apply() {
    // REMOVE Serial.println("Debug Raw Sensors applied");

    // Instantiate the Observer for the RawSensor values and attach it to the hardware
    rawSensorObserver = new Output_RawSensorValues();
    Hardware *hardware = HW_TYPE::getInstance(); // Get the hardware instance

    hardware->attachObserver(rawSensorObserver); // Attach the observer to the hardware
}

void DebugRawSensors::report() {
#ifdef ARDUINO_ARCH_ESP32
    Serial.println(F("Debug Raw Sensors active"));
#endif
}
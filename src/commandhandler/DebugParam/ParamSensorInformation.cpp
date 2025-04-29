#include "ParamSensorInformation.h"
#include <Arduino.h>
#include "serialoutput/output_sensorvalues.h"
#include "config.h" // For Hardware type
#include "hardware/hardware_hall.h"
#include "hardware/hardware_joystick.h"

// Only log to serial if not using Arduino AVR architecture
#ifndef ARDUINO_ARCH_AVR
#ifndef ESP_PRINT(x)
#define ESP_PRINT(x) Serial.println(x)
#endif
#else
#define ESP_PRINT(x)
#endif

/**
 * @brief Constructor to initialize the DebugParamSensorInformation class
 * @details This constructor initializes the rawSensorObserver pointer to nullptr.
 *          It is used to create an instance of the DebugParamSensorInformation class.
 *          The constructor does not take any parameters and does not perform any operations.
 *          It is a simple constructor that initializes the class member variables to their default values.
 */
IDebugParamSensorInformation::IDebugParamSensorInformation() : IDebugParam(), SensorObserver(nullptr) {} // Constructor to initialize currentLevel

/**
 * @brief Destructor to clean up the observer instance
 * @details This destructor checks if the rawSensorObserver pointer is not null.
 *          If it is not null, it detaches the observer from the hardware and deletes the observer instance.
 *          This ensures that the observer is properly cleaned up and does not cause memory leaks.
 */
IDebugParamSensorInformation::~IDebugParamSensorInformation() {
    if (SensorObserver != nullptr) {
        Hardware *hardware = HW_TYPE::getInstance(); // Get the hardware instance
        hardware->detachObserver(SensorObserver);    // Detach the observer from the hardware
        delete SensorObserver;                       // Clean up the observer instance
        SensorObserver = nullptr;
    }
}

// ----------------- RAW SENSORS ----------------

void DebugParamSensorInformationRaw::apply() {

    // Instantiate the Observer for the RawSensor values and attach it to the hardware
    SensorObserver = new Output_SensorValuesRaw();

    Hardware *hardware = HW_TYPE::getInstance(); // Get the hardware instance
    hardware->attachObserver(SensorObserver);    // Attach the observer to the hardware
}

void DebugParamSensorInformationRaw::report() {
    ESP_PRINT(F("DebugParamSensorInformationRaw active")); // Report the active state of the class
}

// ----------------- CENTERED SENSORS ----------------

void DebugParamSensorInformationCentered::apply() {

    // Instantiate the Observer for the CenteredSensor values and attach it to the hardware
    SensorObserver = new Output_SensorValuesCentered();

    Hardware *hardware = HW_TYPE::getInstance(); // Get the hardware instance
    hardware->attachObserver(SensorObserver);    // Attach the observer to the hardware
}

void DebugParamSensorInformationCentered::report() {
    ESP_PRINT(F("DebugParamSensorInformationCentered active")); // Report the active state of the class
}

// ----------------- FILTERED SENSORS -----------------

void DebugParamSensorInformationFiltered::apply() {

    // Instantiate the Observer for the FilteredSensor values and attach it to the hardware
    SensorObserver = new Output_SensorValuesFiltered();

    Hardware *hardware = HW_TYPE::getInstance(); // Get the hardware instance
    hardware->attachObserver(SensorObserver);    // Attach the observer to the hardware
}

void DebugParamSensorInformationFiltered::report() {
    ESP_PRINT(F("DebugParamSensorInformationFiltered active")); // Report the active state of the class
}
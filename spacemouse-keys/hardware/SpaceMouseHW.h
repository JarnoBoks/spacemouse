#ifndef SPACEMOUSEHW_h
#define SPACEMOUSEHW_h

#include <Arduino.h>
// #include "config.h" // TODO Remove inclusion!

#define NUM_SENSORS 8

/**
 * @brief Class to abstract from used hardware
 *
 *
 */
class SpaceMouseHW_ {
public:
    SpaceMouseHW_();

    bool busyZeroing(uint16_t numIterations, boolean debugFlag);
    void readAllFromSensors();
    virtual void setAnalogReferenceVoltage(int debug);
    virtual void _calculateKinematicSensors(int16_t *velocity);

protected:
    /**
     * TODO Used in debugOutput1
     */
    void _printRawRead(const char *axisname, int idx);

    /**
     * TODO Used in debugOutput1
     */
    void _printCentered(const char *axisname, int idx);

private:
    /// @brief  Array containing ADC pin configuration for the sensors.
    int _pinList[NUM_SENSORS];

    /// @brief  Array containing inversion parameters for the raw sensor readings.
    int _invertList[NUM_SENSORS];

    /// @brief  Array containing (inverted) raw sensor readings.
    int _rawReads[NUM_SENSORS] = {0, 0, 0, 0, 0, 0, 0, 0};

    int _maxVals[NUM_SENSORS];
    int _minVals[NUM_SENSORS];

    void _printValue(const char *axisname, int value);

    ///@brief Store the zero position of the joysticks
    int centerPoints[NUM_SENSORS];

protected:
    /// @brief Stores the values from the joysticks after zeroing and mapping
    int centered[NUM_SENSORS] = {0, 0, 0, 0, 0, 0, 0, 0};
};

#endif // SPACEMOUSEHW_h
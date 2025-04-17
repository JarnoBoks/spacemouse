#ifndef SPACEMOUSEHW_h
#define SPACEMOUSEHW_h

#include <Arduino.h>
// #include "config.h" // TODO Remove inclusion!

#define NUM_SENSORS 8

// Type to carry data in the busyZeroing proces between Base & Derived class
struct zeroing_t {
    uint32_t mean[NUM_SENSORS]; // Array to count all values during the averaging
    int minValue[NUM_SENSORS];  // Array to store the minimum values
    int maxValue[NUM_SENSORS];  // Array to store the maximum values

    int16_t deadZone[NUM_SENSORS]; // TODO
    int16_t maxDeadZone;           // TODO

    uint16_t count; // Track the number of iterations during the zeroing proces
};

/**
 * @brief Class to abstract from used hardware
 *
 *
 */
class SpaceMouseHW_ {
public:
    // --- Constructors/Destructors
    SpaceMouseHW_(const int warnCpntMax, const int warnCpntMin, const int warnMMMax, const int warnMMMin, const char *axs[]);
    ~SpaceMouseHW_();

    void ReadAllFromSensors();
    void CenterSensors();
    void FilterAnalogReadOuts();

    void CalcMinMax();
    virtual void PrintRawReads();
    virtual void PrintCentered();

    // --- Implemented by Derived Hardware Classes
    virtual void SetAnalogReferenceVoltage(int debug) = 0;
    virtual void CalculateKinematicSensors(int16_t *velocities) = 0;

    // --- Overriden by Derived Hardware Classes
    virtual bool BusyZeroing(uint16_t numIterations, boolean debugFlag);

protected:
    /// @brief Stores the values from the sensors after zeroing and mapping.
    /// Used in the derived classes for velocity calculation
    int centered[NUM_SENSORS] = {0, 0, 0, 0, 0, 0, 0, 0};

private:
    // -- Output to serial interface
    void _printZeroedValue(zeroing_t *params, const char *axisname, int i);
    void _printValue(const char *axisname, int value);
    void _printArray(int arr[], int size);

    // --- Calibrations
    bool _busyZeroing(zeroing_t *params, uint16_t numIterations);

    /// @brief Array containing the names of each Axis.
    const char **_axisNames;

    /// @brief  Array containing ADC pin configuration for the sensors.
    const int _pinList[NUM_SENSORS];

    /// @brief  Array containing inversion parameters for the raw sensor readings.
    const int _invertList[NUM_SENSORS];

    ///@brief Store the zero position of the joysticks/knop axis. Calculated on startup or on zeroing.
    int _centerPoints[NUM_SENSORS] = {0, 0, 0, 0, 0, 0, 0, 0};

    /// @brief  Array containing (inverted) raw sensor readings.
    int _rawReads[NUM_SENSORS] = {0, 0, 0, 0, 0, 0, 0, 0};

    /// @brief  Arrays containing the configured min- and max values of the spacemouse.
    ///         On startup these values are set to the value in config.h, but can be changed during
    ///         the min/max calibration.
    int _maxVals[NUM_SENSORS];
    int _minVals[NUM_SENSORS];

    /// @brief Contains the state of the statemachine servicing the minmax calibration.
    uint8_t _minMaxCalcState = 0;

    /// @brief Used for tracking the starttime of the calibration procedure (used for Zeroing & minmaxCalibration)
    unsigned int long _startMillis = 0;

    /// @brief Warning levels used in the calibration, and set by the childs on initialization (values are different for hardware types)
    const int _warningCenterpointMin; // Warning level for the minimal centerpoint value (centerpoint below this value throws a warning)
    const int _warningCenterpointMax; // Warning level for the maximum centerpoint value (centerpoint above this value throws a warning)
    const int _warningMinMaxMinimum;  // Warning level for the minimum value (absolute minimum below this value throws a warning)
    const int _warningMinMaxMaximum;  // Warning level for the maximum value (absolute maximum below this value throws a warning)
};

#endif // SPACEMOUSEHW_h
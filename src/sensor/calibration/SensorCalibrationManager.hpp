#pragma once

class IObserver;
class SensorCollection;

class SensorCalibrationManager {
protected:
    IObserver *currentCalibration = nullptr;        // Pointer to the current calibration observer
    SensorCollection *m_SensorCollection = nullptr; // Pointer to the sensor collection

    /**
     * @brief Constructor for the SensorCalibrationManager class.
     * @param sensorCollection Pointer to the SensorCollection instance.
     * @details Initializes the sensor calibration manager with the provided sensor collection.
     */
    SensorCalibrationManager(SensorCollection *sensorCollection) : m_SensorCollection(sensorCollection) {};

public:
    SensorCalibrationManager() = delete;           // Delete the default constructor
    virtual ~SensorCalibrationManager() = default; // Default destructor

    virtual void activate() = 0;
    virtual void deactivate(const bool warningsOccurred);

#if 0 // REMOVE
    void activateIdleCalibration(const int iterations = 500);
    void deactivateIdleCalibration(const bool warningsOccurred);

    void activateMinMaxCalibration();
    void deactivateMinMaxCalibration(const bool warningsOccurred);
#endif
};

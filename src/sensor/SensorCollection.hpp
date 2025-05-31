#pragma once

#include <base/collection/CollectionBase.hpp> // Include the ICollection interface header file
#include <common/Observable.hpp>              // Include the ICollection interface header file
#include <base/visitable/VisitableBase.hpp>   // Include the IVisitable interface header file
#include <stdint.h>

/// @brief Number of sensors that can be added to the collection.
/// @details This is a constant value that defines the maximum number of sensors that can be added to the collection.
/// @note As long as the specific hardware options for the SpaceMouse all have the same number of sensors, this is a good solution.
constexpr uint8_t cHW_MAX_SENSORS = 8;

/// @brief Number of observers that can be added to this collection.
/// @details This is a constant value that defines the maximum number of observers that can be added to the collection.
constexpr uint8_t c_MAX_SENSORCOLLECTION_OBSERVERS = 4;

class Sensor;
class Calibrator;

/**
 * @brief Class representing a collection of sensors for the SpaceMouse.
 * @details This class implements the ICollection interface and provides functionality for managing a collection of sensors.
 *          It allows adding, removing, and notifying observers of changes in the sensor collection.
 * @note The SensorCollection class is designed to manage a fixed number of sensors and their associated observers.
 */
class SensorCollection : public CollectionBase, public Observable, public VisitableBase {
private:
    Calibrator *m_Calibrator = nullptr; // Pointer to the calibrator instance for this sensor collection
public:
    /// @brief Constructor for empty SensorCollection
    SensorCollection();
    ~SensorCollection();

    /**
     * @brief   Set up the sensor collection based on the configuration.
     * @details This function initializes the sensor collection and sets up the sensors based on the configuration.
     *          It creates instances of the sensors and configures them according to the provided configuration.
     * @note    This function should be called once during the initialization phase of the application.
     * @param   None
     */
    void setup();

    Sensor *getSensor(const uint8_t id) const;
    Sensor *getSensor(const char *descriptor) const;

    inline Calibrator *getCalibrator() const { return m_Calibrator; }

    /**
     * @brief Evaluate all items in the collection and notify observers of the changes.
     * @details This function iterates through all items in the collection and calls their evaluate method.
     *          This allows each item to perform its own evaluation and update its state accordingly.
     * @note Derived classes are allowed to override this method to provide custom evaluation logic.
     */
    void evaluate() override {
        CollectionBase::evaluate();    // Evaluate each Sensor in the collection
        Observable::notifyObservers(); // Notify observers to this Collection of changes in the Collection
    };

    /**
     * @brief Accept the Visitor for each Sensor in the collection.
     */
    void accept(IVisitor &visitor) override;
};

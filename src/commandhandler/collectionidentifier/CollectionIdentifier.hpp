#pragma once

class SensorCollection; // Forward declaration of SensorCollection class
class AxisCollection;   // Forward declaration of AxisCollection class
class KeyCollection;    // Forward declaration of KeyCollection class

/**
 * @brief   Class containing pointers to collections of sensors, axes, and keys.
 * @details This class is used as a data container to access collections of sensors, axes, and keys. It avoids separate parameters in
 *          every function and makes it easier to pass around the collections as a single object and to extend the functionality in the future.
 */
class CollectionIdentifier {
private:
    SensorCollection *m_SensorCollection = nullptr; // Pointer to the sensor collection
    AxisCollection *m_AxisCollection = nullptr;     // Pointer to the axis collection
    KeyCollection *m_KeyCollection = nullptr;       // Pointer to the key collection

public:
    CollectionIdentifier() = delete;                                        // Delete the default constructor to prevent instantiation without parameters
    CollectionIdentifier(const CollectionIdentifier &) = delete;            // Delete the copy constructor to prevent copying
    CollectionIdentifier &operator=(const CollectionIdentifier &) = delete; // Delete the copy assignment operator to prevent copying

    ~CollectionIdentifier() = default;

    /**
     * @brief Constructor for the CollectionIdentifier class.
     * @param sensorCollection Pointer to the SensorCollection instance.
     * @param axisCollection Pointer to the AxisCollection instance.
     * @param keyCollection Pointer to the KeyCollection instance.
     * @details Initializes the collection identifier with the provided collections.
     */
    CollectionIdentifier(SensorCollection *sensorCollection, AxisCollection *axisCollection, KeyCollection *keyCollection)
        : m_SensorCollection(sensorCollection), m_AxisCollection(axisCollection), m_KeyCollection(keyCollection) {}

    SensorCollection *getSensorCollection() const { return m_SensorCollection; } // Get the sensor collection
    AxisCollection *getAxisCollection() const { return m_AxisCollection; }       // Get the axis collection
    KeyCollection *getKeyCollection() const { return m_KeyCollection; }          // Get the key collection
};
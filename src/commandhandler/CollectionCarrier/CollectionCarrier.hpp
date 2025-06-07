#pragma once

class SensorCollection;
class KnobAxisCollection;
class KeyCollection;
class Kinematics;

/**
 * @brief   Class containing pointers to collections of sensors, axes, and keys.
 * @details This class is used as a data container to access collections of sensors, axes, and keys. It avoids separate parameters in
 *          every function and makes it easier to pass around the collections as a single object and to extend the functionality in the future.
 */
class CollectionCarrier {
private:
    SensorCollection *m_SensorCollection = nullptr;     // Pointer to the sensor collection
    KnobAxisCollection *m_KnobAxisCollection = nullptr; // Pointer to the knob motionvectors collection
    KeyCollection *m_KeyCollection = nullptr;           // Pointer to the key collection
    Kinematics *m_Kinematics = nullptr;                 // Pointer to the kinematics collection

public:
    CollectionCarrier() = delete;                                     // Delete the default constructor to prevent instantiation without parameters
    CollectionCarrier(const CollectionCarrier &) = delete;            // Delete the copy constructor to prevent copying
    CollectionCarrier &operator=(const CollectionCarrier &) = delete; // Delete the copy assignment operator to prevent copying

    ~CollectionCarrier() = default;

    /**
     * @brief Constructor for the CollectionIdentifier class.
     * @param sensorCollection Pointer to the SensorCollection instance.
     * @param knobAxisCollection Pointer to the AxisCollection instance.
     * @param keyCollection Pointer to the KeyCollection instance.
     * @param kinematics Pointer to the Kinematics instance.
     * @details Initializes the collection identifier with the provided collections.
     */
    CollectionCarrier(SensorCollection *sensorCollection,
                      KnobAxisCollection *knobAxisCollection,
                      KeyCollection *keyCollection,
                      Kinematics *kinematics)
        : m_SensorCollection(sensorCollection),
          m_KnobAxisCollection(knobAxisCollection),
          m_KeyCollection(keyCollection),
          m_Kinematics(kinematics) {}

    inline SensorCollection *getSensorCollection() const { return m_SensorCollection; }       // Get the sensor collection
    inline KnobAxisCollection *getKnobAxisCollection() const { return m_KnobAxisCollection; } // Get the axis collection
    inline KeyCollection *getKeyCollection() const { return m_KeyCollection; }                // Get the key collection
    inline Kinematics *getKinematics() const { return m_Kinematics; }                         // Get the kinematics collection
};
#pragma once
#include <common/IVisitor.hpp> // Include the IVisitor interface header file

#include <knob/KnobAxisCollection.hpp>
#include <knob/axis/KnobAxis.hpp> // For KnobAxis class
#include <common/esp_print.h>     // For ESP_PRINT

/**
 * @brief Visitor class for updating the minimum and maximum values of a sensor.
 * @details This class implements the IVisitor interface and is used to visit Sensor objects to update their minimum and maximum values based on the centered value.
 *          It checks if the sensor and its configuration are valid before performing the update.
 * @note The visit method retrieves the sensor ID, gets the sensor configuration, and updates the minimum and maximum values if they are lower or higher than the current values.
 */
class KnobAxisCollectionGetMainAxis : public IVisitor {
private:
    KnobAxis *m_mainAxis = nullptr;           // Pointer to the main axis, initialized to nullptr
    const int16_t m_LedVelocityDeadzone = 10; // Deadzone for velocity, can be adjusted as needed

    // Private method to set the main axis
    void setMainAxis(KnobAxis *axis) {
        m_mainAxis = axis; // Set the main axis pointer
    }

public:
    KnobAxisCollectionGetMainAxis(const int16_t VelocityDeadzone)
        : m_LedVelocityDeadzone(VelocityDeadzone) {} // Default constructor
    ~KnobAxisCollectionGetMainAxis() = default;      // Destructor

    void visit(VisitableBase &visitable) override {

        // Cast the visitable to knobAxisCollection
        KnobAxisCollection &axisCollection = static_cast<KnobAxisCollection &>(visitable);

        int16_t maximumVelocity = 0; // Variable to store the maximum velocity found

        // Loop through all axes to find the one with the biggest velocity
        for (uint8_t i = 0; i < axisCollection.getItemCount(); i++) {
            int16_t absvalue = abs(axisCollection.getAxis(i)->getGateValue()); // Get the value of the motionVector

            // Is the value of this motionVector greater than deadzone and greater than any of the motionVector before?
            if ((absvalue > maximumVelocity) && (absvalue > m_LedVelocityDeadzone)) {
                maximumVelocity = absvalue;
                m_mainAxis = axisCollection.getAxis(i); // Set the main axis to the current motionVector
            }
        }
    }

    // Private method to get the main axis
    KnobAxis *getMainAxis() const {
        return m_mainAxis; // Return the main axis pointer
    }
};

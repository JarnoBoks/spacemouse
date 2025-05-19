#pragma once
#include "IVisitor.hpp"

#include <Knob/MotionVector/KnobMotionVector.hpp>
#include <Knob/KnobMotionVectorCollection.hpp>

/**
 * @brief Visitor class for handling exclusive mode for translational or rotational movement.
 * @warning This class should only be used for visiting AxisCollection objects.
 */
class ExclusiveMovementVisitor : public IVisitor {
public:
    ExclusiveMovementVisitor() = default;  // Default constructor
    ~ExclusiveMovementVisitor() = default; // Destructor
    /**
     * @brief Visit method for AxisCollection objects.
     * @param knobMotionVectors The AxisCollection object to visit.
     * @details This method calculates the total rotation and translation values of the axes in the collection.
     *          It then sets the fin value of either the translation or rotation axes to 0, depending on which has a greater total value.
     */
    void visit(Visitable &knobMotionVectors) override {

        // Cast the Visitable to AxisCollection
        KnobMotionVectorCollection *axisCol = static_cast<KnobMotionVectorCollection *>(&knobMotionVectors);

        if (!axisCol) {
            Serial.println(F("Invalid AxisCollection"));
            return;
        }

        uint16_t totalRotation = 0;    // Total rotation value
        uint16_t totalTranslation = 0; // Total translation value

        for (uint8_t i = 0; i < axisCol->getItemCount(); i++) {
            KnobMotionVector *axis = static_cast<KnobMotionVector *>(axisCol->getItem(i));
            if (axis) {
                if (axis->isTranslation()) {
                    totalTranslation += abs(axis->getFinValue());
                } else {
                    totalRotation += abs(axis->getFinValue());
                }
            }
        }

        // If the total rotation is greater than the total translation, set translation axes to 0
        // Otherwise, set rotation axes to 0
        bool actOnTranslationAxis = (totalRotation > totalTranslation); // Zero the translation (true) or the rotation (false) axes
        if (actOnTranslationAxis) {
            Serial.println(F("Zeroing translation axes"));
        } else {
            Serial.println(F("Zeroing rotation axes"));
        }

        for (uint8_t i = 0; i < axisCol->getItemCount(); i++) {
            KnobMotionVector *axis = static_cast<KnobMotionVector *>(axisCol->getItem(i));
            if (axis && (axis->isTranslation() == actOnTranslationAxis)) {
                axis->setFinValue(0); // Set the fin value to 0 for the selected axes
            }
        }
    }
};

#pragma once
#include "IVisitor.hpp" // Include the IVisitor interface header file

#include <axis/axes/Axis.hpp> // Include the Axis class header file
#include <axis/KnobMotionVectorCollection.hpp>

/**
 * @brief Visitor class for handling exclusive mode for translational or rotational movement.
 * @warning This class should only be used for visiting AxisCollection objects.
 */
class KillSwitchVisitor : public IVisitor {
public:
    KillSwitchVisitor() = default;  // Default constructor
    ~KillSwitchVisitor() = default; // Destructor

    void visit(Visitable &knobMotionVectors) override {

        // Cast the Visitable to AxisCollection
        KnobMotionVectorCollection *axisCol = static_cast<KnobMotionVectorCollection *>(&knobMotionVectors);

        if (!axisCol) {
            Serial.println(F("Invalid AxisCollection"));
            return;
        }

        // If the total rotation is greater than the total translation, set translation axes to 0
        // Otherwise, set rotation axes to 0
        bool actOnTranslationAxis = true; // Zero the translation (true) or the rotation (false) axes   //FIXME!!!

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

#pragma once
#include <common/IVisitor.hpp> // Include the IVisitor interface header file

#include <knob/axis/KnobAxis.hpp> // Include the Axis class header file
#include <knob/KnobAxisCollection.hpp>

/**
 * @brief Visitor class for handling exclusive mode for translational or rotational movement.
 * @warning This class should only be used for visiting AxisCollection objects.
 */
class KillSwitchVisitor : public IVisitor {
public:
    KillSwitchVisitor() = default;  // Default constructor
    ~KillSwitchVisitor() = default; // Destructor

    void visit(VisitableBase &knobMotionVectors) override {

        // Cast the VisitableBase to AxisCollection
        KnobAxisCollection *axisCol = static_cast<KnobAxisCollection *>(&knobMotionVectors);

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
            KnobAxis *axis = static_cast<KnobAxis *>(axisCol->getItem(i));
            if (axis && (axis->isTranslation() == actOnTranslationAxis)) {
                axis->setFinValue(0); // Set the fin value to 0 for the selected axes
            }
        }
    }
};

#pragma once
#include "IVisitor.hpp" // Include the IVisitor interface header file

#include <Knob/MotionVector/KnobMotionVector.hpp> // Include the Axis class header file
#include <Knob/KnobMotionVectorCollection.hpp>

/**
 * @brief Visitor class for switching the Y and Z axes in the AxisCollection.
 * @details This class implements the IVisitor interface and provides functionality for switching the Y and Z axes in the AxisCollection.
 * @warning This class should only be used for visiting AxisCollection objects.
 */
class SwitchYZVisitor : public IVisitor {
public:
    SwitchYZVisitor() = default;  // Default constructor
    ~SwitchYZVisitor() = default; // Destructor

    void visit(Visitable &knobMotionVectors) override {

        // Cast the Visitable to AxisCollection
        KnobMotionVectorCollection *axisCol = static_cast<KnobMotionVectorCollection *>(&knobMotionVectors);

        if (!axisCol) {
            Serial.println(F("Invalid AxisCollection"));
            return;
        }

        KnobMotionVector *axisY = axisCol->getMotionVector(TRANSY);
        KnobMotionVector *axisZ = axisCol->getMotionVector(TRANSZ);

        int16_t tmp = 0;
        tmp = axisY->getFinValue();
        axisY->setFinValue(axisZ->getFinValue());
        axisZ->setFinValue(tmp);

        axisY = axisCol->getMotionVector(ROTY);
        axisZ = axisCol->getMotionVector(ROTZ);

        tmp = axisY->getFinValue();
        axisY->setFinValue(axisZ->getFinValue());
        axisZ->setFinValue(tmp);
    }
};

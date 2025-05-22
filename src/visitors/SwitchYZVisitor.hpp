#pragma once
#include <common/IVisitor.hpp> // Include the IVisitor interface header file

#include <knob/axis/KnobAxis.hpp> // Include the Axis class header file
#include <knob/KnobAxisCollection.hpp>

/**
 * @brief Visitor class for switching the Y and Z axes in the AxisCollection.
 * @details This class implements the IVisitor interface and provides functionality for switching the Y and Z axes in the AxisCollection.
 * @warning This class should only be used for visiting AxisCollection objects.
 */
class SwitchYZVisitor : public IVisitor {
public:
    SwitchYZVisitor() = default;  // Default constructor
    ~SwitchYZVisitor() = default; // Destructor

    void visit(VisitableBase &knobMotionVectors) override {

        // Cast the VisitableBase to AxisCollection
        KnobAxisCollection *axisCol = static_cast<KnobAxisCollection *>(&knobMotionVectors);

        if (!axisCol) {
            Serial.println(F("Invalid AxisCollection"));
            return;
        }

        KnobAxis *axisY = axisCol->getAxis(TRANSY);
        KnobAxis *axisZ = axisCol->getAxis(TRANSZ);

        int16_t tmp = 0;
        tmp = axisY->getFinValue();
        axisY->setFinValue(axisZ->getFinValue());
        axisZ->setFinValue(tmp);

        axisY = axisCol->getAxis(ROTY);
        axisZ = axisCol->getAxis(ROTZ);

        tmp = axisY->getFinValue();
        axisY->setFinValue(axisZ->getFinValue());
        axisZ->setFinValue(tmp);
    }
};

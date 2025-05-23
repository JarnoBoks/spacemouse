#if 0
/// Depreated class - KillSwitchVisitor
/// @deprecated This class is not used
#pragma once
#include <common/IVisitor.hpp> // Include the IVisitor interface header file

#include <kinematics/axis/KinematicsAxis.hpp>                     // Include the Axis class header file
#include <kinematics/axiscollection/KinematicsAxisCollection.hpp> // Include the AxisCollection class header file

/**
 * @brief Visitor class for handling exclusive mode for translational or rotational movement.
 * @warning This class should only be used for visiting KinematicsAxis objects.
 * @deprecated This class is not used in the current implementation and may be removed in future versions.
 */
class KillSwitchVisitor : public IVisitor {
public:
    KillSwitchVisitor() = default;  // Default constructor
    ~KillSwitchVisitor() = default; // Destructor

    void visit(VisitableBase &kinematicsAxes) override {

        // Cast the VisitableBase to AxisCollection
        KinematicsAxisCollection *axisCol = static_cast<KinematicsAxisCollection *>(&kinematicsAxes);

        if (!axisCol) {
            Serial.println(F("Invalid AxisCollection"));
            return;
        }

        axisCol->setAllToZero(); // Set all axis in the collection to zero
    }
};
#endif
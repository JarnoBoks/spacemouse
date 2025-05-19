#pragma once

#include <kinematics/MotionVectors/MotionVectorCollectionType.h> // Include the MotionVectorCollection_t enum header file
#include <stdint.h>

class KinematicsMotionVectorCollection;

/**
 * @brief KinematicsMVCFactory base class for creating MotionVectorCollection instances.
 */
class KinematicsMVCFactory { // Forward declaration of KinematicsMotionVectorCollection class{
protected:
public:
    KinematicsMVCFactory() = default;
    virtual ~KinematicsMVCFactory() {};

    virtual KinematicsMotionVectorCollection *create(const MotionVectorCollection_t groupType); // Pure virtual function to create a sensor instance.
};

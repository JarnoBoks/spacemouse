#include "KinematicsMVCFactory.hpp"

#include <kinematics/MotionVectors/KinematicsMotionVectorCollection.hpp>
#include <kinematics/MotionVectors/Vector/KinematicsMotionVector.hpp>
#include <common/esp_print.h>

KinematicsMotionVectorCollection *KinematicsMVCFactory::create(const MotionVectorCollection_t groupType) {
    // Create and return a new KinematicsMotionVectorCollection instance based on the groupType
    KinematicsMotionVectorCollection *collection = new KinematicsMotionVectorCollection();
    if (groupType == MotionVectorCollection_t::TRANSLATION) {
        collection->add(new KinematicsMotionVector(TRANSX));
        collection->add(new KinematicsMotionVector(TRANSY));
        collection->add(new KinematicsMotionVector(TRANSZ));
    } else if (groupType == MotionVectorCollection_t::ROTATION) {
        collection->add(new KinematicsMotionVector(ROTX));
        collection->add(new KinematicsMotionVector(ROTY));
        collection->add(new KinematicsMotionVector(ROTZ));
    } else {
        ESP_WARN("Invalid group type");
        delete collection; // Clean up the collection if the group type is invalid
        return nullptr;
    }

    return collection;
}

#include "MotionVector.hpp"
#include <Knob/ModifierFunctionType.h> // Include the header file for ModFunc_t enum

#include <math.h>
#define sign(x) ((x) < 0 ? -1 : ((x) > 0 ? 1 : 0)) // Define Signum Function

/**
 * @brief Checks if the provided descriptor matches the MotionVector's descriptor.
 * @param descriptor The descriptor to compare against the MotionVector's descriptor.
 * @return True if the descriptors match, false otherwise.
 */
const bool MotionVector::isCurrent(const char *descriptor) const {
    if (m_descriptor != nullptr) {
        return (strcmp(m_descriptor, descriptor) == 0); // NOTE - Change necessary if PROGMEN is used.
    }
    return false;
}

#include "AxisDirectionConfig.hpp"
#include "axis/config/AxisConfig.hpp"

AxisDirectionConfig::AxisDirectionConfig() : data{1.0f, 0, mfLINEAR}, context(nullptr) {}

AxisDirectionConfig::AxisDirectionConfig(float sensitivity, uint8_t gate, ModFunc_t type)
    : data{sensitivity, gate, type}, context(nullptr) {
#if 0
    this->setModfunc(type); // Initialize the modifier function based on the provided type
#endif
}

/**
 * @brief  Sets the modifier function type and creates a new modifier function.
 * @details Cleans up the old modifier function and creates a new one based on the specified type.
 * @param mfType The type of the modifier function to be set.
 * @deprecated This function is not used in the current implementation and may be removed in future versions.
 */
void AxisDirectionConfig::setModfunc(ModFunc_t type) {

    // Update the modifier function type
    this->data.modFuncType = type;

#if 0
    // Clean up the old modifier function
    delete modifier;

    // Create a new modifier function based on the new type
    switch (type) {
    case mfLINEAR:
        modifier = new LinearModifier();
        break;
    case mfSQUARED:
        modifier = new SquaredModifier();
        break;
    case mfTANGENT:
        modifier = new TangentModifier();
        break;
    case mfSQUARDED_TANGENT:
        modifier = new SquaredTangentModifier();
        break;
    case mfCUBED_TANGENT:
        modifier = new CubedTangentModifier();
        break;
    default:
        modifier = new LinearModifier(); // Default to linear if unknown type
        break;
    }
#endif
}

AxisDirectionConfig::~AxisDirectionConfig() {
#if 0
    delete modifier; // Clean up the modifier function
#endif
}

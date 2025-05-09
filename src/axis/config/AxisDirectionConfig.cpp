#include "AxisDirectionConfig.hpp"

AxisDirectionConfig::AxisDirectionConfig() : sensitivity(1.0f), gate(0), modFuncType(mfLINEAR) {}

AxisDirectionConfig::AxisDirectionConfig(float sensitivity, uint8_t gate, ModFunc_t type)
    : sensitivity(sensitivity), gate(gate), modFuncType(type) {
#if 0
    this->setModfunc(type); // Initialize the modifier function based on the provided type
#endif
}

/**
 * @brief  Sets the modifier function type and creates a new modifier function.
 * @details Cleans up the old modifier function and creates a new one based on the specified type.
 * @param mfType The type of the modifier function to be set.
 */
void AxisDirectionConfig::setModfunc(ModFunc_t type) {

    // Update the modifier function type
    this->modFuncType = type;

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
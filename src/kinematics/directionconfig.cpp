#include "directionconfig.h"

DirectionConfig::DirectionConfig() : sensitivity(1.0f), gate(0), modifier(nullptr), modFuncType(mfLINEAR) {}

DirectionConfig::DirectionConfig(float sensitivity, uint8_t gate, ModFunc_t modFuncType)
    : sensitivity(sensitivity), gate(gate), modFuncType(modFuncType) {
    switch (modFuncType) {
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
}

/**
 * @brief  Sets the modifier function type and creates a new modifier function.
 * @details Cleans up the old modifier function and creates a new one based on the specified type.
 * @param mfType The type of the modifier function to be set.
 */
void DirectionConfig::setModfunc(ModFunc_t type) {

    // Update the modifier function type
    this->modFuncType = type;

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
}

DirectionConfig::~DirectionConfig() {
    delete modifier; // Clean up the modifier function
}
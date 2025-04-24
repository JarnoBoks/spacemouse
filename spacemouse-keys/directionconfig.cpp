
#include "directionconfig.h"

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

// Add any other configuration parameters you need here
void DirectionConfig::setModfunc(ModFunc_t modFuncType) {
    modFuncType = modFuncType;
    delete modifier; // Clean up the old modifier function
    // Create a new modifier function based on the new type

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

DirectionConfig::~DirectionConfig() {
    delete modifier; // Clean up the modifier function
}
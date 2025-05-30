#include "DefaultKnobAxisConfig.hpp"
#include "config.h" // Include the config file to know the hardware type and load the correct defaults for the hardware.
#include <knob/Axis/KnobAxis.hpp>

#if defined(HW_HALLEFFECT)
#include "defaults_hall.h"
#elif defined(HW_JOYSTICK)
#include "defaults_joystick.h"
#else
#error "No hardwaretype defined"
#endif

#include <common/esp_print.h> // For ESP_ERROR

// Preprocessor macros to convert the default values to the correct types
// These macros are used to convert the default values to the correct types for the KnobAxisConfig constructor.
#define S_TP(x) static_cast<float>(x)
#define G_TP(x) static_cast<uint8_t>(x)
#define M_TP(x) static_cast<ModFunc_t>(x)

/**
 * @brief Get the default configuration for the specified axis type.
 * @param type The axis type for which to get the default configuration.
 * @return The default KnobAxisConfig for the specified axis type.
 */
KnobAxisConfig DefaultKnobAxisConfig::create(const KnobAxis *axis) {
    // This function will return the default configuration for the given axis type.
    // The default configuration is used if there isn't a configuration in the EEPROM or if the EEPROM version is changed.
    switch (axis->getType()) {
    case MotionVector_t::TRANSX:
        return KnobAxisConfig(axis, S_TP(DEF_SENS_TX_POS), S_TP(DEF_SENS_TX_NEG), G_TP(DEF_GATE_TX_POS), G_TP(DEF_GATE_TX_NEG), M_TP(DEF_MF_TX_POS), M_TP(DEF_MF_TX_NEG), DEF_INVERT_TX);
    case MotionVector_t::TRANSY:
        return KnobAxisConfig(axis, S_TP(DEF_SENS_TY_POS), S_TP(DEF_SENS_TY_NEG), G_TP(DEF_GATE_TY_POS), G_TP(DEF_GATE_TY_NEG), M_TP(DEF_MF_TY_POS), M_TP(DEF_MF_TY_NEG), DEF_INVERT_TY);
    case MotionVector_t::TRANSZ:
        return KnobAxisConfig(axis, S_TP(DEF_SENS_TZ_POS), S_TP(DEF_SENS_TZ_NEG), G_TP(DEF_GATE_TZ_POS), G_TP(DEF_GATE_TZ_NEG), M_TP(DEF_MF_TZ_POS), M_TP(DEF_MF_TZ_NEG), DEF_INVERT_TZ);
    case MotionVector_t::ROTX:
        return KnobAxisConfig(axis, S_TP(DEF_SENS_RX_POS), S_TP(DEF_SENS_RX_NEG), G_TP(DEF_GATE_RX_POS), G_TP(DEF_GATE_RX_NEG), M_TP(DEF_MF_RX_POS), M_TP(DEF_MF_RX_NEG), DEF_INVERT_RX);
    case MotionVector_t::ROTY:
        return KnobAxisConfig(axis, S_TP(DEF_SENS_RY_POS), S_TP(DEF_SENS_RY_NEG), G_TP(DEF_GATE_RY_POS), G_TP(DEF_GATE_RY_NEG), M_TP(DEF_MF_RY_POS), M_TP(DEF_MF_RY_NEG), DEF_INVERT_RY);
    case MotionVector_t::ROTZ:
        return KnobAxisConfig(axis, S_TP(DEF_SENS_RZ_POS), S_TP(DEF_SENS_RZ_NEG), G_TP(DEF_GATE_RZ_POS), G_TP(DEF_GATE_RZ_NEG), M_TP(DEF_MF_RZ_POS), M_TP(DEF_MF_RZ_NEG), DEF_INVERT_RZ);
    default:
        // This situation should not happen.
        ESP_ERROR("Unknown axis type");
        return nullptr; // Return a null pointer if the axis type is unknown
    }
}

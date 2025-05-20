#include "MotionVectorConfigDefaults.hpp"
#include "config.h" // Include the config file to know the hardware type and load the correct defaults for the hardware.

#if defined(HW_HALLEFFECT)
#include "defaults_hall.h"
#elif defined(HW_JOYSTICK)
#include "defaults_joystick.h"
#else
#error "No hardwaretype defined"
#endif

// Preprocessor macros to convert the default values to the correct types
// These macros are used to convert the default values to the correct types for the KnobVectorConfig constructor.
#define S_TP(x) static_cast<float>(x)
#define G_TP(x) static_cast<uint8_t>(x)
#define M_TP(x) static_cast<ModFunc_t>(x)

/**
 * @brief Creates a defaults filled MotionVectorConfig instance
 * @details This function will return a MotionVectorConfig instance with the default configuration
 *          for the given MotionVector type. The default configuration is retrieved from the specific
 *          hardware defaults.h file, or from the config.h file if a default value is overidden there.
 * @param type The MotionVector type (fe. TRANSX) for which to get the default configuration.
 * @return A MotionVectorConfig instance with the default values set.
 */
MotionVectorConfig MotionVectorConfigDefaults::create(MotionVector_t type) {
    // This function will return the default configuration for the given axis type.
    // The default configuration is used if there isn't a configuration in the EEPROM or if the EEPROM version is changed.
    switch (MotionVector_t(type)) {
    case MotionVector_t::TRANSX:
        return MotionVectorConfig(S_TP(DEF_SENS_TX_POS), S_TP(DEF_SENS_TX_NEG), G_TP(DEF_GATE_TX_POS), G_TP(DEF_GATE_TX_NEG), M_TP(DEF_MF_TX_POS), M_TP(DEF_MF_TX_NEG), DEF_INVERT_TX);
    case MotionVector_t::TRANSY:
        return MotionVectorConfig(S_TP(DEF_SENS_TY_POS), S_TP(DEF_SENS_TY_NEG), G_TP(DEF_GATE_TY_POS), G_TP(DEF_GATE_TY_NEG), M_TP(DEF_MF_TY_POS), M_TP(DEF_MF_TY_NEG), DEF_INVERT_TY);
    case MotionVector_t::TRANSZ:
        return MotionVectorConfig(S_TP(DEF_SENS_TZ_POS), S_TP(DEF_SENS_TZ_NEG), G_TP(DEF_GATE_TZ_POS), G_TP(DEF_GATE_TZ_NEG), M_TP(DEF_MF_TZ_POS), M_TP(DEF_MF_TZ_NEG), DEF_INVERT_TZ);
    case MotionVector_t::ROTX:
        return MotionVectorConfig(S_TP(DEF_SENS_RX_POS), S_TP(DEF_SENS_RX_NEG), G_TP(DEF_GATE_RX_POS), G_TP(DEF_GATE_RX_NEG), M_TP(DEF_MF_RX_POS), M_TP(DEF_MF_RX_NEG), DEF_INVERT_RX);
    case MotionVector_t::ROTY:
        return MotionVectorConfig(S_TP(DEF_SENS_RY_POS), S_TP(DEF_SENS_RY_NEG), G_TP(DEF_GATE_RY_POS), G_TP(DEF_GATE_RY_NEG), M_TP(DEF_MF_RY_POS), M_TP(DEF_MF_RY_NEG), DEF_INVERT_RY);
    case MotionVector_t::ROTZ:
        return MotionVectorConfig(S_TP(DEF_SENS_RZ_POS), S_TP(DEF_SENS_RZ_NEG), G_TP(DEF_GATE_RZ_POS), G_TP(DEF_GATE_RZ_NEG), M_TP(DEF_MF_RZ_POS), M_TP(DEF_MF_RZ_NEG), DEF_INVERT_RZ);
    default:
        return MotionVectorConfig(); // Return an empty MotionVectorConfig if the type is not recognized
    }
}

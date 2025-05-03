
#include "defaultaxisconfig.h"
#include "config.h" // Include the config file to know the hardware type

#ifdef HALLEFFECT
#include "../defaults_hall.h"
#elif defined(JOYSTICK)
#include "../defaults_joystick.h"
#endif

// Initialize the static instance pointer to nullptr
DefaultAxisConfig *DefaultAxisConfig::instance = nullptr;

/**
 * @brief Get the instance of DefaultAxisConfig.
 * @return Reference to the singleton instance of DefaultAxisConfig.
 */
DefaultAxisConfig &DefaultAxisConfig::getInstance() {
    if (!instance) {
        instance = new DefaultAxisConfig();
    }
    return *instance;
}

/**
 * @brief DefaultAxisConfig constructor.
 * @details This constructor is private to enforce the singleton pattern.
 * It initializes the default axis configuration for the given hardware type.
 */
DefaultAxisConfig::DefaultAxisConfig() {
}

// Preprocessor macros to convert the default values to the correct types
// These macros are used to convert the default values to the correct types for the AxisConfig constructor.
#define S_TP(x) static_cast<float>(x)
#define G_TP(x) static_cast<uint8_t>(x)
#define M_TP(x) static_cast<ModFunc_t>(x)

/**
 * @brief Get the default configuration for the specified axis type.
 * @param type The axis type for which to get the default configuration.
 * @return The default AxisConfig for the specified axis type.
 */
AxisConfig DefaultAxisConfig::getDefaultConfig(AxisType_t type) {
    // This function will return the default configuration for the given axis type.
    // The default configuration is used if there isn't a configuration in the EEPROM or if the EEPROM version is changed.
    switch (AxisType_t(type)) {
    case AxisType_t::TRANSX:
        return AxisConfig(S_TP(DEF_SENS_TX_POS), S_TP(DEF_SENS_TX_NEG), G_TP(DEF_GATE_TX_POS), G_TP(DEF_GATE_TX_NEG), M_TP(DEF_MF_TX_POS), M_TP(DEF_MF_TX_NEG), DEF_INVERT_TX);
    case AxisType_t::TRANSY:
        return AxisConfig(S_TP(DEF_SENS_TY_POS), S_TP(DEF_SENS_TY_NEG), G_TP(DEF_GATE_TY_POS), G_TP(DEF_GATE_TY_NEG), M_TP(DEF_MF_TY_POS), M_TP(DEF_MF_TY_NEG), DEF_INVERT_TY);
    case AxisType_t::TRANSZ:
        return AxisConfig(S_TP(DEF_SENS_TZ_POS), S_TP(DEF_SENS_TZ_NEG), G_TP(DEF_GATE_TZ_POS), G_TP(DEF_GATE_TZ_NEG), M_TP(DEF_MF_TZ_POS), M_TP(DEF_MF_TZ_NEG), DEF_INVERT_TZ);
    case AxisType_t::ROTX:
        return AxisConfig(S_TP(DEF_SENS_RX_POS), S_TP(DEF_SENS_RX_NEG), G_TP(DEF_GATE_RX_POS), G_TP(DEF_GATE_RX_NEG), M_TP(DEF_MF_RX_POS), M_TP(DEF_MF_RX_NEG), DEF_INVERT_RX);
    case AxisType_t::ROTY:
        return AxisConfig(S_TP(DEF_SENS_RY_POS), S_TP(DEF_SENS_RY_NEG), G_TP(DEF_GATE_RY_POS), G_TP(DEF_GATE_RY_NEG), M_TP(DEF_MF_RY_POS), M_TP(DEF_MF_RY_NEG), DEF_INVERT_RY);
    case AxisType_t::ROTZ:
        return AxisConfig(S_TP(DEF_SENS_RZ_POS), S_TP(DEF_SENS_RZ_NEG), G_TP(DEF_GATE_RZ_POS), G_TP(DEF_GATE_RZ_NEG), M_TP(DEF_MF_RZ_POS), M_TP(DEF_MF_RZ_NEG), DEF_INVERT_RZ);
    default:
        return AxisConfig(); // Return an empty AxisConfig if the type is not recognized
    }
}

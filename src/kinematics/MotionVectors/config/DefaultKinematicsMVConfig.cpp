#include "DefaultKinematicsMVConfig.hpp"
#include "KinematicsMVConfig.hpp"
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
 * @brief Get the default configuration for the specified MotionVector type.
 * @param type The MotionVector type for which to get the default configuration.
 * @return The default KinematicsMVConfig for the specified MotionVector type.
 */
KinematicsMVConfig DefaultKinematicsMVConfig::create(MotionVector_t type) {
    switch (MotionVector_t(type)) {
    case MotionVector_t::TRANSX:
        return KinematicsMVConfig(DEF_INVERT_TX);
    case MotionVector_t::TRANSY:
        return KinematicsMVConfig(DEF_INVERT_TY);
    case MotionVector_t::TRANSZ:
        return KinematicsMVConfig(DEF_INVERT_TZ);
    case MotionVector_t::ROTX:
        return KinematicsMVConfig(DEF_INVERT_RX);
    case MotionVector_t::ROTY:
        return KinematicsMVConfig(DEF_INVERT_RY);
    case MotionVector_t::ROTZ:
        return KinematicsMVConfig(DEF_INVERT_RZ);
    default:
        return KinematicsMVConfig(); // Return an empty KinematicsMVConfig if the type is not recognized
    }
}

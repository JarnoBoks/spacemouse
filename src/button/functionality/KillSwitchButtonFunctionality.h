// KillSwitchButtonFunctionality.h
#pragma once
#include "IButtonFunctionality.h"

/**
 * @brief Interface class for handling kill switch button functionality.
 */
class KillSwitchButtonFunctionality : public IButtonFunctionality {
public:
    virtual ~KillSwitchButtonFunctionality() = default; // Destructor
    virtual void onPress() override = 0;                // pure virtual function to handle button press
    virtual void onRelease() override = 0;              // pure virtual function to handle button release
};
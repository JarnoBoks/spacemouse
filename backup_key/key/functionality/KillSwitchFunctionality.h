// KillSwitchButtonFunctionality.h
#pragma once
#include "IKeyFunctionality.h"

/**
 * @brief Interface class for handling kill switch key functionality.
 */
class KillSwitchFunctionality : public IKeyFunctionality {
public:
    virtual ~KillSwitchFunctionality() = default; // Destructor
    virtual void onPress() override = 0;          // pure virtual function to handle key press
    virtual void onRelease() override = 0;        // pure virtual function to handle key release
};
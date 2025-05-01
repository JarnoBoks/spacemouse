#pragma once
#include "IDebugParam.h"
#include "observers/output_hidvalues.h"

class ParamHIDInformation : public IDebugParam {
    // This class is responsible for handling the debug state related to the HID information.
    // It inherits from IDebugParam and implements the apply and report methods.
    // The class uses an instance of Output_HIDValues to output HID values for debugging purposes.
protected:
    Output_HIDValues *HIDObserver = nullptr; // Pointer to the raw HID observer instance       // REVIEW - Why is this necessary?
public:
    ParamHIDInformation() = default;          // Constructor
    virtual ~ParamHIDInformation() = default; // Destructor to clean up the observer instance
    virtual void apply() override;
    virtual void report() override;
};

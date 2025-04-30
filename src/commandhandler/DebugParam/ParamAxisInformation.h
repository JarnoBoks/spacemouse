#ifndef DP_PARAMAXISINFORMATION_H
#define DP_PARAMAXISINFORMATION_H
#include "IDebugParam.h"
#include "observers/output_axisvalues.h"

class DebugParamAxisInformation : public IDebugParam {
protected:
    Output_AxisValues *AxisObserver = nullptr; // Pointer to the raw sensor observer instance       // REVIEW - Why is this necesar
public:
    DebugParamAxisInformation() = default; // Constructor
    ~DebugParamAxisInformation();          // Destructor to clean up the observer instance

    void apply() override;
    void report() override;
};
#endif // DP_PARAMAXISINFORMATION_H
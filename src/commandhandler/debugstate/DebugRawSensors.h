#ifndef DEBUGRAWSENSORS_H
#define DEBUGRAWSENSORS_H
#include "IDebugState.h"
#include "serialoutput/output_rawsensorvalues.h"

class DebugRawSensors : public IDebugState {
private:
    Output_RawSensorValues *rawSensorObserver = nullptr; // Pointer to the raw sensor observer instance
public:
    DebugRawSensors();
    ~DebugRawSensors(); // Destructor to clean up the observer instance
    void apply() override;
    void report() override;
};
#endif // DEBUGRAWSENSORS_H
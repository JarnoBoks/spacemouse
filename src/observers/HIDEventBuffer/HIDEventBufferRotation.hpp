#pragma once
#include "HIDEventBuffer.hpp"

class HIDEventBufferRotation : public HIDEventBuffer {
private:
public:
    HIDEventBufferRotation() = default;
    virtual ~HIDEventBufferRotation() = default;

    void update(IObservable *kinVectorRotation) override final;
};
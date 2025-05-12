#pragma once
#include "HIDEventBuffer.hpp"

class HIDEventBufferRotation : public HIDEventBuffer {
private:
public:
    HIDEventBufferRotation() = default;
    virtual ~HIDEventBufferRotation() override = default;

    void update(IObservable *rotationAxis) override final;
};
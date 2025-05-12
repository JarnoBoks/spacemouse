#pragma once
#include "HIDEventBuffer.hpp"

class HIDEventBufferKeys : public HIDEventBuffer {
private:
public:
    HIDEventBufferKeys() = default;
    virtual ~HIDEventBufferKeys() override = default;

    void update(IObservable *key) override final;
};
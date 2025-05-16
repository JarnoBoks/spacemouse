#pragma once
#include "HIDEventBuffer.hpp"

class HIDEventBufferTranslation : public HIDEventBuffer {
private:
public:
    HIDEventBufferTranslation() = default;
    virtual ~HIDEventBufferTranslation() = default;

    void update(IObservable *translationAxis) override final;
};
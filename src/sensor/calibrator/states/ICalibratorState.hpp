#pragma once

// REFACTOR - This interface file and the base class should be refactored.
class Calibrator;

class ICalibratorState {
protected:
    Calibrator *context = nullptr; // Pointer to the context (Calibrator) that this state belongs to
public:
    ICalibratorState() = default;
    virtual ~ICalibratorState() = default;

    inline void set_context(Calibrator *context) { this->context = context; }

    virtual void start() = 0;
    virtual void update() = 0;
    virtual void finish() = 0;
};
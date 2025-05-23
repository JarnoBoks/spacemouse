#pragma once

#include <common/IVisitor.hpp>
#include <knob/axis/KnobAxis.hpp>
#include <knob/axis/config/KnobAxisConfig.hpp>

#define FMT_NOCOMMA false
#define FMT_COMMA true
#define NO_PREFIX ""
#define NO_SIGN " "
#define TWO_DECIMALS 2
#define ONE_DECIMAL 1
#define NO_DECIMALS 0

class AxisConfigPrinter : public IVisitor {
private:
    void _helper_PrintConfig(const char *name, const float posval, const float negval, const boolean printseparator, const char *typeprefix, const uint8_t precision, const int8_t minwidth);
    size_t _helper_PrintItem(const char *separator, const char *typeprefix, const char *name, const char *sign);

public:
    AxisConfigPrinter() {}

    void visit(VisitableBase &visitable) override {
        // Cast the VisitableBase to KnobAxis
        KnobAxis &knobAxis = static_cast<KnobAxis &>(visitable);

        // Print the sensitivity
        _helper_PrintConfig(knobAxis.getDescriptor(),
                            knobAxis.getConfig()->posConfig.getSensitivity(),
                            knobAxis.getConfig()->negConfig.getSensitivity(),
                            FMT_NOCOMMA, NO_PREFIX, TWO_DECIMALS, 21);

        // Print the gate
        _helper_PrintConfig(knobAxis.getDescriptor(),
                            knobAxis.getConfig()->posConfig.getGate(),
                            knobAxis.getConfig()->negConfig.getGate(),
                            FMT_COMMA, "G", NO_DECIMALS, 21);

        // Print the modfunc
        _helper_PrintConfig(knobAxis.getDescriptor(),
                            knobAxis.getConfig()->posConfig.getModFuncType(),
                            knobAxis.getConfig()->negConfig.getModFuncType(),
                            FMT_COMMA, "M", NO_DECIMALS, 19);

        // Print the inversion
        _helper_PrintConfig(knobAxis.getDescriptor(),
                            knobAxis.getConfig()->inversion,
                            knobAxis.getConfig()->inversion,
                            FMT_COMMA, "I", NO_DECIMALS, -1);
        Serial.println(); // Print a newline after the last knobAxis status was printed to the serial monitor
    }
};

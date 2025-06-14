#pragma once

#include <common/IVisitor.hpp>

class KnobAxis;
#include <knob/axis/KnobAxis.hpp>

class ExclusiveMovementVisitor : public IVisitor {
    int _transTotal = 0;
    int _rotTotal = 0;
    KnobAxis *_transAxes[3];
    KnobAxis *_rotAxes[3];
    uint8_t _transCount = 0;
    uint8_t _rotCount = 0;

public:
    ExclusiveMovementVisitor() = default;

    void visit(VisitableBase &ax) {

        KnobAxis *axis = static_cast<KnobAxis *>(&ax);

        int16_t v = abs(axis->getFinValue());
        if (axis->isTranslation()) {
            if (_transCount < 3) {
                _transAxes[_transCount++] = axis;
                _transTotal += v;
            }
        } else {
            if (_rotCount < 3) {
                _rotAxes[_rotCount++] = axis;
                _rotTotal += v;
            }
        }
    }

    void finalize() {
        KnobAxis **loser = nullptr;
        uint8_t count = 0;

        if (_transTotal > _rotTotal) {
            loser = _rotAxes;
            count = _rotCount;
        } else {
            loser = _transAxes;
            count = _transCount;
        }

        for (uint8_t i = 0; i < count; ++i) {
            loser[i]->setFinValue(0);
        }
    }
};

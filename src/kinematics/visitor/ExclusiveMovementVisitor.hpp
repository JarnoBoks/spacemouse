#pragma once

#include <common/IVisitor.hpp>

class KinematicsAxis;
#include <kinematics/axis/KinematicsAxis.hpp>

class ExclusiveMovementVisitor : public IVisitor {
    int _transTotal = 0;
    int _rotTotal = 0;
    KinematicsAxis *_transAxes[3];
    KinematicsAxis *_rotAxes[3];
    uint8_t _transCount = 0;
    uint8_t _rotCount = 0;

public:
    ExclusiveMovementVisitor() = default;

    void visit(KinematicsAxis *ax) {
        int16_t v = abs(ax->getFinValue());
        if (ax->isTranslation()) {
            if (_transCount < 3) {
                _transAxes[_transCount++] = ax;
                _transTotal += v;
            }
        } else {
            if (_rotCount < 3) {
                _rotAxes[_rotCount++] = ax;
                _rotTotal += v;
            }
        }
    }

    void finalize() {
        KinematicsAxis **loser = nullptr;
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
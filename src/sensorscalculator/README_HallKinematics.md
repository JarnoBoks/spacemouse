# Advanced Hall Sensor Kinematics Algorithm

## Overview

This advanced algorithm translates readings from 4 Hall sensor pairs to 6 motion vectors (3 translation + 3 rotation) with improved cross-axis coupling compensation. The algorithm is specifically designed for a spacemouse with sensor pairs mounted at 12, 3, 6, and 9 o'clock positions.

## Key Features

* **Cross-axis coupling compensation**: Minimizes unwanted translation when rotating and vice versa
* **Enhanced rotation detection**: Uses sensor pair asymmetries for better rotation sensitivity
* **Configurable sensitivity**: Individual axis sensitivity adjustment
* **Deadzone handling**: Eliminates noise while preserving precise small movements
* **Filtering**: Smooth rotation response through low-pass filtering

## Sensor Layout

```
        12 o'clock (North)
         HES1  HES2
            |
9 o'clock   |   3 o'clock (East)
HES7 HES8---+---HES5 HES6
            |
         HES3  HES4
        6 o'clock (South)
```

## Algorithm Phases

### Phase 1: Sensor Preprocessing

* Normalize sensor readings to -1.0 to +1.0 range
* Calculate pair averages for gross motion detection
* Calculate pair asymmetries for fine rotation detection

### Phase 2: Raw Motion Calculation

Uses a sensitivity matrix to map sensor pair readings to motion vectors:

| Axis | 12 o'clock | 3 o'clock | 6 o'clock | 9 o'clock |
|----|----|----|----|----|
| TX | 0 | +1 | 0 | -1 |
| TY | +1 | 0 | -1 | 0 |
| TZ | +1 | +1 | +1 | +1 |
| RX | +1 | 0 | -1 | 0 |
| RY | 0 | +1 | 0 | -1 |
| RZ | +1 | -1 | +1 | -1 |

### Phase 3: Enhanced Rotation Detection

* **Roll (RX)**: Asymmetry difference between 12 and 6 o'clock pairs
* **Pitch (RY)**: Asymmetry difference between 3 and 9 o'clock pairs
* **Yaw (RZ)**: Circular asymmetry pattern across all pairs

### Phase 4: Cross-coupling Compensation

* Reduces translation Z when significant rotation is detected
* Reduces translation X,Y during yaw rotation
* Configurable compensation factor (0.0-1.0)

### Phase 5: Filtering and Output

* Low-pass filtering for smooth rotation response
* Scaling to ±350 output range
* Deadzone application
* Range constraining

## Calibration Procedure

### 1. Electrical Zero Calibration

```cpp
// Place spacemouse in idle position
// Read sensors 500+ times and average
int zeroes[8];
for(int i = 0; i < 8; i++) {
    long sum = 0;
    for(int j = 0; j < 500; j++) {
        sum += analogRead(SENSOR_PINS[i]);
    }
    zeroes[i] = sum / 500;
}
kinematics.setCalibration(zeroes, mins, maxs);
```

### 2. Range Calibration

For each sensor pair, apply maximum motion in each direction:

| Motion | Expected Sensor Response |
|----|----|
| +X translation | 3 o'clock pair increases, 9 o'clock decreases |
| +Y translation | 12 o'clock pair increases, 6 o'clock decreases |
| +Z translation | All pairs increase equally |
| Roll (+RX) | 12 o'clock asymmetry positive, 6 o'clock negative |
| Pitch (+RY) | 3 o'clock asymmetry positive, 9 o'clock negative |
| Yaw (+RZ) | Alternating asymmetry pattern |

### 3. Sensor Pair Verification

Test each motion type and verify the expected sensor response:

```cpp
// Test translation X
Serial.println("Move knob east (positive X)");
// Expect: HES5,HES6 average increases, HES7,HES8 average decreases

// Test rotation Z (yaw)
Serial.println("Rotate knob clockwise");
// Expect: Asymmetries in alternating pattern
```

### 4. Cross-coupling Tuning

Adjust compensation factors based on your hardware:

```cpp
// If yaw causes unwanted Z translation:
#define COUPLING_COMPENSATION 0.85f  // Increase (0.0-1.0)

// If rotations are too jittery:
rotationFilter = 0.8f;  // Increase (0.0-0.9)
```

### 5. Sensitivity Balancing

Use `setSensitivity()` to balance axis responses:

```cpp
kinematics.setSensitivity(0, 0.8f);  // Reduce TX sensitivity
kinematics.setSensitivity(4, 1.2f);  // Increase RY sensitivity
```

## Integration Guide

### Option 1: Drop-in Replacement

Replace existing `calculateKinematic()` calls:

```cpp
// Old way:
calculateKinematic(centered, velocity);

// New way:
setHallCalibrationFromConfig();
calculateHallKinematics(centered, velocity);
```

### Option 2: Direct Class Usage

```cpp
HallSensorKinematics kinematics;
int sensorReadings[8];
int16_t motionVectors[6];

// Read sensors
for(int i = 0; i < 8; i++) {
    sensorReadings[i] = analogRead(SENSOR_PINS[i]);
}

// Calculate motion
kinematics.calculateMotion(sensorReadings, motionVectors);
```

### Option 3: Modify SensorsCalculatorHall.cpp

Replace the switch statement in `evaluate()`:

```cpp
void SensorsCalculatorHall::evaluate(KnobAxis *knobVector) {
    static HallSensorKinematics* kinematics = nullptr;
    if(!kinematics) {
        kinematics = hallKinematics_init();
        setHallCalibrationFromConfig();
    }

    // Read all sensors
    int sensorReadings[8];
    for(int i = 0; i < 8; i++) {
        sensorReadings[i] = /* read sensor i */;
    }

    // Calculate all axes at once
    int16_t motionVectors[6];
    kinematics->calculateMotion(sensorReadings, motionVectors);

    // Set the requested axis value
    switch(knobVector->getType()) {
        case MotionVector_t::TRANSX: knobVector->setRawValue(motionVectors[0]); break;
        case MotionVector_t::TRANSY: knobVector->setRawValue(motionVectors[1]); break;
        case MotionVector_t::TRANSZ: knobVector->setRawValue(motionVectors[2]); break;
        case MotionVector_t::ROTX:   knobVector->setRawValue(motionVectors[3]); break;
        case MotionVector_t::ROTY:   knobVector->setRawValue(motionVectors[4]); break;
        case MotionVector_t::ROTZ:   knobVector->setRawValue(motionVectors[5]); break;
    }
}
```

## Troubleshooting

### Problem: Yaw rotation causes Z translation

**Solution**: Increase `COUPLING_COMPENSATION` (try 0.9)

### Problem: Rotations are jittery

**Solution**: Increase `rotationFilter` (try 0.8-0.9)

### Problem: One axis is insensitive

**Solution**: Use `setSensitivity()` to increase that axis

### Problem: Cross-coupling between X and Y translation

**Solution**: Check sensor pair wiring, verify 12/6 vs 3/9 o'clock positioning

### Problem: Noise when spacemouse is idle

**Solution**: Increase `DEADZONE_THRESHOLD`

### Problem: Can't make small precise movements

**Solution**: Decrease `DEADZONE_THRESHOLD`

### Problem: Output range is not ±350

**Solution**: Verify calibration min/max values, check `OUTPUT_SCALE`

## Performance Characteristics

* **Update Rate**: Suitable for 100Hz+ operation
* **Memory Usage**: \~200 bytes for class instance
* **CPU Usage**: Minimal, mostly simple arithmetic
* **Latency**: Sub-millisecond processing time
* **Resolution**: Full 16-bit signed integer output

## Advantages Over Previous Algorithm


1. **Better Cross-axis Decoupling**: Yaw rotation doesn't affect Z translation
2. **Enhanced Rotation Detection**: Uses sensor asymmetries for better sensitivity
3. **Configurable**: Easy to tune for different hardware setups
4. **Smoother Response**: Filtering reduces jitter in rotation axes
5. **Maintainable**: Clear separation of concerns and well-documented code

## Future Enhancements

* Adaptive deadzone based on motion history
* Temperature compensation for sensor drift
* Machine learning calibration optimization
* Multi-point calibration for non-linear sensors
* Real-time cross-coupling coefficient adjustment



#ifndef ROBOTGUIDE_L298NWHEEL_H
#define ROBOTGUIDE_L298NWHEEL_H

#include "Direction.h"

#include <stdint.h>

class L298NWheel
{
public:
    L298NWheel(uint8_t directionPin1, uint8_t directionPin2, uint8_t pwmPin);
    L298NWheel(const L298NWheel& other) = delete;
    L298NWheel& operator=(const L298NWheel&) = delete;
    ~L298NWheel() = default;

    void SetWheel(Direction direction, uint8_t power) const;
    void SetWheelPower(uint8_t power) const;
    void SetWheelDirection(Direction direction) const;
    void Brake() const;

private:
    const uint8_t directionPin1;
    const uint8_t directionPin2;
    const uint8_t pwmPin;
};

#endif
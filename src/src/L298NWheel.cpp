#include "L298NWheel.h"

#include <Arduino.h>

L298NWheel::L298NWheel(uint8_t directionPin1, uint8_t directionPin2, uint8_t pwmPin)
    : directionPin1(directionPin1)
    , directionPin2(directionPin2)
    , pwmPin(pwmPin)
{
    pinMode(directionPin1, OUTPUT);
    pinMode(directionPin2, OUTPUT);
    pinMode(pwmPin, OUTPUT);
}

void L298NWheel::SetWheel(Direction direction, uint8_t power) const
{
    SetWheelDirection(direction);
    SetWheelPower(power);
}

void L298NWheel::SetWheelDirection(Direction direction) const
{
    switch (direction)
    {
    case Direction::FORWARD:
        digitalWrite(directionPin1, LOW);
        digitalWrite(directionPin2, HIGH);
        break;

    case Direction::BACKWARD:
        digitalWrite(directionPin1, HIGH);
        digitalWrite(directionPin2, LOW);
        break;
    }  
}

void L298NWheel::SetWheelPower(uint8_t power) const
{
    analogWrite(pwmPin, power);
}

void L298NWheel::Brake() const
{
    digitalWrite(directionPin1, LOW);
    digitalWrite(directionPin2, LOW);
    analogWrite(pwmPin, 255);
}
#include "RotaryEncoders.h"

#include <Arduino.h>

RotaryEncoders RotaryEncoders::instance;

RotaryEncoders& RotaryEncoders::GetInstance()
{
    return instance;
}

void RotaryEncoders::SetupInterrupts(int encoderPinL, int encoderPinR)
{
    pinMode(encoderPinL, INPUT);
    pinMode(encoderPinR, INPUT);

    attachInterrupt(digitalPinToInterrupt(encoderPinL), &Isr_renc_L, CHANGE);
    attachInterrupt(digitalPinToInterrupt(encoderPinR), &Isr_renc_R, CHANGE);
}

void RotaryEncoders::ClearCounts()
{
    encoderCounterL = 0;
    encoderCounterR = 0;
}

unsigned long RotaryEncoders::GetEncoderCountL() const
{
    return encoderCounterL;
}

unsigned long RotaryEncoders::GetEncoderCountR() const
{
    return encoderCounterR;
}

void RotaryEncoders::Isr_renc_L()
{
    instance.encoderCounterL++;
}

void RotaryEncoders::Isr_renc_R()
{
    instance.encoderCounterR++;
}


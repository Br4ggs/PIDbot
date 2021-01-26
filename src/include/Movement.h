#ifndef ROBOTGUIDE_MOVEMENT_H
#define ROBOTGUIDE_MOVEMENT_H

#include "RotaryEncoders.h"
#include "L298NWheel.h"
#include "PIDcontroller.h"
#include "ILoopComponent.h"

#include <stdint.h>

class Movement : public ILoopComponent
{
public:
    Movement(RotaryEncoders& rotaryEncoders, 
            L298NWheel& leftWheel,
            L298NWheel& rightWheel,
            PIDcontroller& leftPID,
            PIDcontroller& rightPID,
            PIDcontroller& deltaPID,
            int rencCountsPerRev,
            int wheelDiameter,
            int platformDiameter,
            uint8_t maxPower,
            uint8_t errorBound,
            float controlSignalPercentile,
            float correctionPercentile,
            float integratorCutoffBound,
            unsigned int delay);
    Movement(const Movement& other) = delete;
    Movement& operator=(const Movement&) = delete;
    ~Movement() override = default;

    bool NeedsUpdate(unsigned long time) const override;
    void Update(unsigned long time) override;

    bool IsMoving() const;
    void Move(int millimeters);
    void Rotate(int degrees);
    void Brake();
    
private:
    RotaryEncoders& rotaryEncoders;
    L298NWheel& leftWheel;
    L298NWheel& rightWheel;
    PIDcontroller& leftPID;
    PIDcontroller& rightPID;
    PIDcontroller& deltaPID;
    
    const unsigned long countsPerRev;
    const float wheelCircumference;
    const float platformCircumference;
    const uint8_t maxPower;
    const uint8_t errorBound;

    const float controlSignalPercentile;
    const float correctionPercentile;
    const float integratorCutoffBound;

    bool moving;
    bool leftMoving;
    bool rightMoving;
    unsigned long targetCount;

    unsigned long prevTime;
    const unsigned int delay;

    float CalculateEncoderTicks(float millimeters) const;
    void FinishIfDestinationReached(long errorL, long errorR);
    static float CalculateCircumference(float diameter);
};

#endif
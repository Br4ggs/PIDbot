#include "Movement.h"

#include <Arduino.h>
#include <math.h>

Movement::Movement(RotaryEncoders& rotaryEncoders, 
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
                  unsigned int delay)
    : rotaryEncoders(rotaryEncoders)
    , leftWheel(leftWheel)
    , rightWheel(rightWheel)
    , leftPID(leftPID)
    , rightPID(rightPID)
    , deltaPID(deltaPID)
    , countsPerRev(rencCountsPerRev)
    , wheelCircumference(CalculateCircumference((float)wheelDiameter))
    , platformCircumference(CalculateCircumference((float)platformDiameter))
    , maxPower(maxPower)
    , errorBound(errorBound)
    , controlSignalPercentile(controlSignalPercentile)
    , correctionPercentile(correctionPercentile)
    , integratorCutoffBound(integratorCutoffBound)
    , moving(false)
    , leftMoving(false)
    , rightMoving(false)
    , targetCount(0)
    , prevTime(0)
    , delay(delay)
{

}

bool Movement::NeedsUpdate(unsigned long time) const
{
    return (IsMoving() && time >= prevTime + delay);
}

void Movement::Update(unsigned long time)
{
    const unsigned long encoderL = rotaryEncoders.GetEncoderCountL();
    const unsigned long encoderR = rotaryEncoders.GetEncoderCountR();

    const long errorL = targetCount - encoderL;
    const long errorR = targetCount - encoderR;
    const long errorDelta = errorL - errorR;
    const long timeDelta = time - prevTime;

    prevTime = time;
    
    float controlSignalDelta = deltaPID.CalculateControlSignal(errorDelta, timeDelta);
    float clampedControlSignalDelta = constrain(controlSignalDelta, -(correctionPercentile * maxPower), correctionPercentile * maxPower);

    if(leftMoving)
    {
        float controlSignalL = leftPID.CalculateControlSignal(errorL, timeDelta);
        float clampedControlSignalL = constrain(controlSignalL, 0, controlSignalPercentile * maxPower);
        
        leftPID.IntegratorEnabled(fabs(controlSignalL - clampedControlSignalL) < integratorCutoffBound);

        uint8_t clampedFinalSignalL = constrain(clampedControlSignalL + clampedControlSignalDelta, 0, maxPower);
        leftWheel.SetWheelPower(clampedFinalSignalL);
    }

    if(rightMoving)
    {
        float controlSignalR = rightPID.CalculateControlSignal(errorR, timeDelta);
        float clampedControlSignalR = constrain(controlSignalR, 0, controlSignalPercentile * maxPower);

        rightPID.IntegratorEnabled(fabs(controlSignalR - clampedControlSignalR) < integratorCutoffBound);

        uint8_t clampedFinalSignalR = constrain(clampedControlSignalR - clampedControlSignalDelta, 0, maxPower);
        rightWheel.SetWheelPower(clampedFinalSignalR);
    }

    FinishIfDestinationReached(errorL, errorR);
}

bool Movement::IsMoving() const
{
    return moving;
}

void Movement::Move(int millimeters)
{
    moving = true;
    leftMoving = true;
    rightMoving = true;

    targetCount = CalculateEncoderTicks(labs(millimeters));

    if(millimeters > 0)
    {
        leftWheel.SetWheel(Direction::FORWARD, 0);
        rightWheel.SetWheel(Direction::FORWARD, 0);
    }
    else
    {
        leftWheel.SetWheel(Direction::BACKWARD, 0);
        rightWheel.SetWheel(Direction::BACKWARD, 0);
    }

    rotaryEncoders.ClearCounts();
}

void Movement::Rotate(int degrees)
{
    moving = true;
    leftMoving = true;
    rightMoving = true;

    const float millimeters = ((float)labs(degrees) * platformCircumference) / 360.0f;
    
    targetCount = CalculateEncoderTicks(millimeters);

    if(degrees > 0)
    {
        leftWheel.SetWheel(Direction::BACKWARD, 0);
        rightWheel.SetWheel(Direction::FORWARD, 0);
    }
    else
    {
        leftWheel.SetWheel(Direction::FORWARD, 0);
        rightWheel.SetWheel(Direction::BACKWARD, 0);
    }

    rotaryEncoders.ClearCounts();
}

void Movement::Brake()
{
    FinishIfDestinationReached(-1, -1);
}

float Movement::CalculateEncoderTicks(float millimeters) const
{
    float revolutions = millimeters / wheelCircumference;
    return revolutions * countsPerRev;
}

void Movement::FinishIfDestinationReached(long errorL, long errorR)
{
    if(errorL <= 0 && leftMoving)
    {
        leftWheel.Brake();
        leftMoving = false;
    }

    if(errorR <= 0 && rightMoving)
    {
        rightWheel.Brake();
        rightMoving = false;
    }

    if(!(leftMoving || rightMoving))
    {
        rightPID.ResetController();
        leftPID.ResetController();
        moving = false;
    }
}

float Movement::CalculateCircumference(float diameter)
{
    return PI * diameter;
}
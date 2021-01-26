#include "PIDcontroller.h"

PIDcontroller::PIDcontroller(float pScale, float iScale, float dScale)
    : pScale(pScale)
    , iScale(iScale)
    , dScale(dScale)
    , lastError(0)
    , errorIntegral(0)
    , integratorEnabled(true)
{
    
}

float PIDcontroller::GetPScale() const
{
    return pScale;
}

float PIDcontroller::GetIScale() const
{
    return iScale;
}

float PIDcontroller::GetDScale() const
{
    return dScale;
}

void PIDcontroller::SetPScale(float scale)
{
    pScale = scale;
}

void PIDcontroller::SetIScale(float scale)
{
    iScale = scale;
}

void PIDcontroller::SetDScale(float scale)
{
    dScale = scale;
}

void PIDcontroller::IntegratorEnabled(bool enabled)
{
    integratorEnabled = enabled;
}

float PIDcontroller::CalculateControlSignal(long error, long deltaTime)
{
    const float errorFloat = (float)error;
    const float deltaTimeFloat = (float)deltaTime;

    float compP = 0;
    float compI = 0;
    float compD = 0;

    compP = error * pScale;

    if(integratorEnabled)
    {
        errorIntegral += (errorFloat * deltaTimeFloat);
        compI = errorIntegral * iScale;
    }

    const float derivativeValue = (error - lastError) / deltaTime;
    compD = derivativeValue * dScale;

    return compP + compI + compD;
}

void PIDcontroller::ResetController()
{
    errorIntegral = 0;
}
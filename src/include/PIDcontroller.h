#ifndef ROBOTGUIDE_PIDCONTROLLER_H
#define ROBOTGUIDE_PIDCONTROLLER_H

class PIDcontroller
{
public:
    PIDcontroller(float pScale, float iScale, float dScale);
    PIDcontroller(const PIDcontroller& other) = delete;
    PIDcontroller& operator=(const PIDcontroller&) = delete;
    ~PIDcontroller() = default;

    float GetPScale() const;
    float GetIScale() const;
    float GetDScale() const;

    void SetPScale(float scale);
    void SetIScale(float scale);
    void SetDScale(float scale);

    void IntegratorEnabled(bool enabled);

    float CalculateControlSignal(long error, long deltaTime);

    void ResetController();

private:
    float pScale;
    float iScale;
    float dScale;

    float lastError;
    float errorIntegral;

    bool integratorEnabled;
};

#endif
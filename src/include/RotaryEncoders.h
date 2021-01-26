#ifndef ROBOTGUIDE_ROTARYENCODERS_H
#define ROBOTGUIDE_ROTARYENCODERS_H

class RotaryEncoders
{
public:
    static RotaryEncoders& GetInstance();
    void SetupInterrupts(int encoderPinL, int encoderPinR);
    void ClearCounts();
    unsigned long GetEncoderCountL() const;
    unsigned long GetEncoderCountR() const; 

private:
    RotaryEncoders() = default;
    RotaryEncoders(const RotaryEncoders& other) = delete;
    RotaryEncoders& operator=(const RotaryEncoders&) = delete;
    ~RotaryEncoders() = default;

    static void Isr_renc_L();
    static void Isr_renc_R();

    static RotaryEncoders instance;
    volatile unsigned long encoderCounterL;
    volatile unsigned long encoderCounterR;
};

#endif
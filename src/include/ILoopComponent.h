#ifndef ROBOTGUIDE_ILOOPCOMPONENT_H
#define ROBOTGUIDE_ILOOPCOMPONENT_H

class ILoopComponent
{
public:
    virtual ~ILoopComponent() = default;

    virtual bool NeedsUpdate(unsigned long time) const = 0;
    virtual void Update(unsigned long time) = 0;
};

#endif
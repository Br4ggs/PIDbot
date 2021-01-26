#ifndef ROBOTGUIDE_NAVIGATOR_H
#define ROBOTGUIDE_NAVIGATOR_H

#include "ILoopComponent.h"
#include "INavigatorCallback.h"
#include "ObstacleDetection.h"
#include "Movement.h"

class Navigator : public ILoopComponent
{
public:
    Navigator(Movement& movement, ObstacleDetection& obstacles, unsigned int delay);
    Navigator(const Navigator& other) = delete;
    Navigator& operator=(const Navigator&) = delete;
    ~Navigator() override = default;

    bool NeedsUpdate(unsigned long time) const override;
    void Update(unsigned long time) override;

    int MoveForward(int millimeters, INavigatorCallback* callback);
    int MoveBackward(int millimeters, INavigatorCallback* callback);
    int RotateLeft(int degrees, INavigatorCallback* callback);
    int RotateRight(int degrees, INavigatorCallback* callback);

private:
    Movement& movement;
    ObstacleDetection& obstacles;
    INavigatorCallback* callback;
    bool executingCommand;
    unsigned long nextUpdateTime;
    const unsigned int delay;

    void NavigationFinished(NavigatorStatus status);
};

#endif
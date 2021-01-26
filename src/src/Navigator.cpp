#include "Navigator.h"

#include <Arduino.h>

Navigator::Navigator(Movement& movement, ObstacleDetection& obstacles, unsigned int delay)
    : movement(movement)
    , obstacles(obstacles)
    , callback(nullptr)
    , executingCommand(false)
    , nextUpdateTime(0)
    , delay(delay)
{

}

bool Navigator::NeedsUpdate(unsigned long time) const
{
    return (executingCommand && time >= nextUpdateTime);
}

void Navigator::Update(unsigned long time)
{
    nextUpdateTime = time + delay;

    if(!movement.IsMoving() && executingCommand)
    {
        NavigationFinished(NavigatorStatus::ARRIVED);
    }

    if(obstacles.ObstacleDetected())
    {
        movement.Brake();
        NavigationFinished(NavigatorStatus::BLOCKED);
    }
}

int Navigator::MoveForward(int millimeters, INavigatorCallback* callback)
{
    if(callback == nullptr)
    {
        return -1;
    }

    this->callback = callback;
    executingCommand = true;
    movement.Move(millimeters);

    return 0;
}

int Navigator::MoveBackward(int millimeters, INavigatorCallback* callback)
{
    if(callback == nullptr)
    {
        return -1;
    }

    this->callback = callback;
    executingCommand = true;
    movement.Move(-millimeters);

    return 0;
}

int Navigator::RotateLeft(int degrees, INavigatorCallback* callback)
{
    if(callback == nullptr)
    {
        return -1;
    }

    this->callback = callback;
    executingCommand = true;
    movement.Rotate(degrees);

    return 0;
}

int Navigator::RotateRight(int degrees, INavigatorCallback* callback)
{
    if(callback == nullptr)
    {
        return -1;
    }

    this->callback = callback;
    executingCommand = true;
    movement.Rotate(-degrees);

    return 0;
}

void Navigator::NavigationFinished(NavigatorStatus status)
{
    executingCommand = false;
    callback->OnNavigationFinished(status);
}
#ifndef ROBOTGUIDE_INAVIGATORCALLBACK_H
#define ROBOTGUIDE_INAVIGATORCALLBACK_H

#include "NavigatorStatus.h"

class INavigatorCallback
{
public:
    virtual ~INavigatorCallback() = default;

    virtual void OnNavigationFinished(NavigatorStatus status) = 0;
};

#endif
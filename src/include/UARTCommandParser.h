#ifndef ROBOTGUIDE_UARTCOMMANDPARSER_H
#define ROBOTGUIDE_UARTCOMMANDPARSER_H

#include "ILoopComponent.h"
#include "INavigatorCallback.h"
#include "Navigator.h"
#include "PIDcontroller.h"
#include "Commands.h"

class UARTCommandParser : public ILoopComponent, public INavigatorCallback
{
public:
    UARTCommandParser(Navigator& navigator,
                     PIDcontroller& leftPID,
                     PIDcontroller& rightPID,
                     PIDcontroller& deltaPID,
                     unsigned int delay);
    UARTCommandParser(const UARTCommandParser& other) = delete;
    UARTCommandParser& operator=(const UARTCommandParser&) = delete;
    ~UARTCommandParser() override = default;

    void OnNavigationFinished(NavigatorStatus status) override;

    bool NeedsUpdate(unsigned long time) const override;
    void Update(unsigned long time) override;

private:
    Navigator& navigator;
    PIDcontroller& leftPID;
    PIDcontroller& rightPID;
    PIDcontroller& deltaPID;

    unsigned long nextUpdateTime;
    const unsigned int delay;

    Commands ParseStringToCommand(char* data);
    void ExecuteCommand(Commands command, float arg);
    void SendSerialResponse(const char* rsp) const;
    void PrintPIDControllerValues() const;
};

#endif
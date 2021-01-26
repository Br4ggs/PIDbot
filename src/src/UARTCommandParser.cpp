#include "UARTCommandParser.h"

#include <Arduino.h>

//currently available serial commands for robot
static const char* FORN = "FORN";
static const char* BACN = "BACN";
static const char* TULN = "TULN";
static const char* TURN = "TURN";

static const char* NAVS = "NAVS";
static const char* NAVF = "NAVF";

//commands for setting pid controller variables
static const char* SETMTRVARP = "SETMTRVARP";
static const char* SETMTRVARI = "SETMTRVARI";
static const char* SETMTRVARD = "SETMTRVARD";

static const char* SETDLTVARP = "SETDLTVARP";
static const char* SETDLTVARI = "SETDLTVARI";
static const char* SETDLTVARD = "SETDLTVARD";

static const char* PRINTPIDVARS = "PRINTPIDVARS";

static const char* VARSETS = "VARSETS";

UARTCommandParser::UARTCommandParser(Navigator& navigator,
                                    PIDcontroller& leftPID,
                                    PIDcontroller& rightPID,
                                    PIDcontroller& deltaPID,
                                    unsigned int delay)
    : navigator(navigator)
    , leftPID(leftPID)
    , rightPID(rightPID)
    , deltaPID(deltaPID)
    , nextUpdateTime(0)
    , delay(delay)
{
    // do not call serial.begin from here
    // calling serial.begin from outside the Setup() function
    // will cause a crash
}

void UARTCommandParser::OnNavigationFinished(NavigatorStatus status)
{
    switch (status)
    {
    case NavigatorStatus::ARRIVED:
        SendSerialResponse(NAVS);
        break;

    case NavigatorStatus::BLOCKED:
        SendSerialResponse(NAVF);
        break;
    
    default:
        break;
    }
}

bool UARTCommandParser::NeedsUpdate(unsigned long time) const
{
    return (Serial.available() && time >= nextUpdateTime);
}

void UARTCommandParser::Update(unsigned long time)
{
    nextUpdateTime = time + delay;

    char msg[20];
    memset(msg, ' ', 20);

    size_t msgLength = Serial.readBytesUntil('\n', msg, 20);

    if(msgLength == 0)
    {
        SendSerialResponse(NAVF);
        return;
    }

    char* cmd = strtok(msg, " ");
    char* argc = strtok(NULL, " ");
    float arg = atof(argc);

    Commands command = ParseStringToCommand(cmd);

    if(command == Commands::NOT_A_COMMAND || 
      (command != Commands::PRINTPIDVARS && (argc == NULL || arg == 0)))
    {
        SendSerialResponse(NAVF);
        return;
    }

    ExecuteCommand(command, arg);
}

Commands UARTCommandParser::ParseStringToCommand(char* data)
{
    if(strcmp(data, FORN) == 0)
    {
        return Commands::FORN;
    }
    else if(strcmp(data, BACN) == 0)
    {
        return Commands::BACN;
    }
    else if(strcmp(data, TURN) == 0)
    {
        return Commands::TURN;
    }
    else if(strcmp(data, TULN) == 0)
    {
        return Commands::TULN;
    }
    else if(strcmp(data, SETMTRVARP) == 0)
    {
        return Commands::SETMTRVARP;
    }
    else if(strcmp(data, SETMTRVARI) == 0)
    {
        return Commands::SETMTRVARI;
    }
    else if(strcmp(data, SETMTRVARD) == 0)
    {
        return Commands::SETMTRVARD;
    }
    else if(strcmp(data, SETDLTVARP) == 0)
    {
        return Commands::SETDLTVARP;
    }
    else if(strcmp(data, SETDLTVARI) == 0)
    {
        return Commands::SETDLTVARI;
    }
    else if(strcmp(data, SETDLTVARD) == 0)
    {
        return Commands::SETDLTVARD;
    }
    else if(strcmp(data, PRINTPIDVARS) == 0)
    {
        return Commands::PRINTPIDVARS;
    }
    else
    {
        return Commands::NOT_A_COMMAND;
    }
}

void UARTCommandParser::ExecuteCommand(Commands command, float arg)
{
    switch (command)
    {
    case Commands::FORN:
        navigator.MoveForward(arg, this);
        break;

    case Commands::BACN:
        navigator.MoveBackward(arg, this);
        break;

    case Commands::TULN:
        navigator.RotateLeft(arg, this);
        break;

    case Commands::TURN:
        navigator.RotateRight(arg, this);
        break;

    case Commands::SETMTRVARP:
        leftPID.SetPScale(arg);
        rightPID.SetPScale(arg);
        SendSerialResponse(VARSETS);
        break;

    case Commands::SETMTRVARI:
        leftPID.SetIScale(arg);
        rightPID.SetIScale(arg);
        SendSerialResponse(VARSETS);
        break;

    case Commands::SETMTRVARD:
        leftPID.SetDScale(arg);
        rightPID.SetDScale(arg);
        SendSerialResponse(VARSETS);
        break;

    case Commands::SETDLTVARP:
        deltaPID.SetPScale(arg);
        SendSerialResponse(VARSETS);
        break;

    case Commands::SETDLTVARI:
        deltaPID.SetIScale(arg);
        SendSerialResponse(VARSETS);
        break;

    case Commands::SETDLTVARD:
        deltaPID.SetDScale(arg);
        SendSerialResponse(VARSETS);
        break;

    case Commands::PRINTPIDVARS:
        PrintPIDControllerValues();
        break;
        
    default:
        break;
    }
}

void UARTCommandParser::SendSerialResponse(const char* rsp) const
{
    char data[strlen(rsp) + 2];
    strcpy(data, rsp);
    strcat(data, "\n");

    Serial.print(data);
}

void UARTCommandParser::PrintPIDControllerValues() const
{
    Serial.print("Left controller P value:----");
    Serial.println(leftPID.GetPScale());
    Serial.print("Left controller I value:----");
    Serial.println(leftPID.GetIScale());
    Serial.print("Left controller D value:----");
    Serial.println(leftPID.GetDScale());

    Serial.println("");

    Serial.print("Right controller P value:---");
    Serial.println(rightPID.GetPScale());
    Serial.print("Right controller I value:---");
    Serial.println(rightPID.GetIScale());
    Serial.print("Right controller D value:---");
    Serial.println(rightPID.GetDScale());

    Serial.println("");

    Serial.print("Delta controller P value:---");
    Serial.println(deltaPID.GetPScale());
    Serial.print("Delta controller I value:---");
    Serial.println(deltaPID.GetIScale());
    Serial.print("Delta controller D value:---");
    Serial.println(deltaPID.GetDScale());
}
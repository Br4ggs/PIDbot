#include "RotaryEncoders.h"
#include "Movement.h"
#include "L298NWheel.h"
#include "UARTCommandParser.h"
#include "Navigator.h"
#include "Constants.h"

#include <Arduino.h>

L298NWheel leftWheel(DRIVER_IN3_PIN, DRIVER_IN4_PIN, DRIVER_ENB_PIN);
L298NWheel rightWheel(DRIVER_IN1_PIN, DRIVER_IN2_PIN, DRIVER_ENA_PIN);

PIDcontroller leftPID(LEFT_MOTOR_P_VALUE, LEFT_MOTOR_I_VALUE, LEFT_MOTOR_D_VALUE);
PIDcontroller rightPID(RIGHT_MOTOR_P_VALUE, RIGHT_MOTOR_I_VALUE, RIGHT_MOTOR_D_VALUE);
PIDcontroller deltaPID(ERROR_DELTA_P_VALUE, ERROR_DELTA_I_VALUE, ERROR_DELTA_D_VALUE);

Movement movement(RotaryEncoders::GetInstance(), 
                 leftWheel,
                 rightWheel,
                 leftPID,
                 rightPID,
                 deltaPID,
                 ENCODER_DISK_TICS,
                 WHEEL_DIAMETER,
                 PLATFORM_DIAMETER,
                 MAX_MOTOR_POWER,
                 TARGET_ERROR_BOUND,
                 CONTROL_SIGNAL_PERCENTILE,
                 SIGNAL_CORRECTION_PERCENTILE,
                 INTEGRATOR_CUTOFF_BOUND,
                 MOVEMENT_DELAY);

ObstacleDetection obstacles;

Navigator navigator(movement, obstacles, NAVIGATOR_DELAY);

UARTCommandParser uart(navigator,
                      leftPID,
                      rightPID,
                      deltaPID,
                      UART_DELAY);

void setup()
{
  RotaryEncoders::GetInstance().SetupInterrupts(RENC_PIN_L, RENC_PIN_R);
  RotaryEncoders::GetInstance().ClearCounts();

  Serial.setTimeout(TIMEOUT);
  Serial.begin(BAUDRATE);
  deltaPID.IntegratorEnabled(false);
};

void loop()
{
  unsigned long time = millis();

  if(uart.NeedsUpdate(time))
  {
    uart.Update(time);
  }

  if(navigator.NeedsUpdate(time))
  {
    navigator.Update(time);
  }

  if(movement.NeedsUpdate(time))
  {
    movement.Update(time);
  }
};
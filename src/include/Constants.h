#ifndef ROBOTGUIDE_CONSTANTS_H
#define ROBOTGUIDE_CONSTANTS_H

#include <stdint.h>

//-----------------------------------------
//         movement class constants
//-----------------------------------------

//pwm input pins for the L298N motor driver
const int DRIVER_ENA_PIN = 10;
const int DRIVER_ENB_PIN = 11;

//direction/mode pins for L298N motor driver
const int DRIVER_IN1_PIN = 9;
const int DRIVER_IN2_PIN = 8;
const int DRIVER_IN3_PIN = 12;
const int DRIVER_IN4_PIN = 13;

//units are in millimeters
const int WHEEL_DIAMETER = 70;
const int PLATFORM_DIAMETER = 125;

//resolution of encoder disks
const int ENCODER_DISK_TICS = 40;

const uint8_t MAX_MOTOR_POWER = 100;
const uint8_t TARGET_ERROR_BOUND = 0;

//percentage of control signal composition
//CONTROL_SIGNAL_PERCENTILE is the initial control signal from the motor's PID-controller
//SIGNAL_CORRECTION_PERCENTILE is the control signal from the error delta correction from the delta PID-controller
//these added together should always be 1
const float CONTROL_SIGNAL_PERCENTILE = 0.85f;
const float SIGNAL_CORRECTION_PERCENTILE = 0.15f;

//if signal saturation delta is higher than this integrator will be cut off
const float INTEGRATOR_CUTOFF_BOUND = 0.5f;

//-----------------------------------------
//     rotary encoders class constants
//-----------------------------------------

//interrupt pins for rotary encoders
const int RENC_PIN_L = 2;
const int RENC_PIN_R = 3;

//-----------------------------------------
//      uart command parser constants
//-----------------------------------------

const int BAUDRATE = 9600;
const int TIMEOUT = 10;

//-----------------------------------------
//         loop component constants
//-----------------------------------------

//delay times for loop components in milliseconds
const unsigned int MOVEMENT_DELAY = 15;
const unsigned int NAVIGATOR_DELAY = 25;
const unsigned int UART_DELAY = 50;

//-----------------------------------------
//         PID controller constants
//-----------------------------------------

//scalers for pid controllers
const float LEFT_MOTOR_P_VALUE = 2.0f;
const float LEFT_MOTOR_I_VALUE = 0.05f;
const float LEFT_MOTOR_D_VALUE = 0.5f;

const float RIGHT_MOTOR_P_VALUE = 2.0f;
const float RIGHT_MOTOR_I_VALUE = 0.05f;
const float RIGHT_MOTOR_D_VALUE = 0.5f;

const float ERROR_DELTA_P_VALUE = 4.0f;
const float ERROR_DELTA_I_VALUE = 0.0f;
const float ERROR_DELTA_D_VALUE = 0.1f;

#endif
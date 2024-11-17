#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "AD.h"
#include "serial.h"
#include "MPU6050.h"
#include "angle.h"
#include <math.h>


typedef struct {
    float Kp;
    float Ki;
    float Kd;
    float prevError;
    float prevPrevError;
    float integral;
} PID_Controller;

void PID_Init(PID_Controller *pid, float Kp, float Ki, float Kd) {
    pid->Kp = Kp;
    pid->Ki = Ki;
    pid->Kd = Kd;
    pid->prevError = 0.0;
    pid->prevPrevError = 0.0;
    pid->integral = 0.0;
}

void PID_MOTOR_Init(PID_Controller *pid, float Kp, float Ki, float Kd) {
    pid->Kp = Kp;
    pid->Ki = Ki;
    pid->Kd = Kd;
    pid->prevError = 0.0;
    pid->prevPrevError = 0.0;
    pid->integral = 0.0;
}

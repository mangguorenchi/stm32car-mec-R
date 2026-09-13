#include "control.h"
#include "motor.h"
#include "sensor.h"
#include <stdio.h>

#define Control_Speed 2400
#define Turn_Speed
#define OBSTACLE_DISTANCE 450.0f

void Control_Init(void) {
  Motor_Init();
  Sensor_Init();
}

void Control_Forward(uint16_t speed) {
  Motor_SetSpeed(MOTOR_M1, speed);
  Motor_SetSpeed(MOTOR_M2, speed);
  Motor_SetSpeed(MOTOR_M3, speed);
  Motor_SetSpeed(MOTOR_M4, speed);
}
void Control_Backward(uint16_t speed) {
  Motor_SetSpeed(MOTOR_M1, -speed);
  Motor_SetSpeed(MOTOR_M2, -speed);
  Motor_SetSpeed(MOTOR_M3, -speed);
  Motor_SetSpeed(MOTOR_M4, -speed);
}

void Control_TurnLeft(uint16_t speed) {
  Motor_SetSpeed(MOTOR_M1, -speed);
  Motor_SetSpeed(MOTOR_M2, speed);
  Motor_SetSpeed(MOTOR_M3, -speed);
  Motor_SetSpeed(MOTOR_M4, speed);
}
void Control_TurnRight(uint16_t speed) {
  Motor_SetSpeed(MOTOR_M1, speed);
  Motor_SetSpeed(MOTOR_M2, -speed);
  Motor_SetSpeed(MOTOR_M3, speed);
  Motor_SetSpeed(MOTOR_M4, -speed);
}

void Control_Stop(void) { Motor_StopAll(); }

void Control_AvoidanceTask(void) {
  float distance = HCSR04_FrontRead();

  if (distance < 0) {
    Control_Stop();
    printf("Ultrasonic ERROR\r\n");
    return;
  }
  if (distance <= OBSTACLE_DISTANCE) {

    Control_Stop();
    HAL_Delay(1000);

    Control_TurnRight(4800);
    HAL_Delay(300);

    Control_Stop();
    HAL_Delay(100);

    Control_Forward(Control_Speed);
  } else {
    Control_Forward(Control_Speed);
  }

  HAL_Delay(50);
}
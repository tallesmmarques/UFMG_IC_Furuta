#pragma once

#ifdef ESP32
  #include <ESP32Encoder.h>
  #include <Arduino.h>

  #define LED_BUILTIN 2

  #define ENC_GREEN   18
  #define ENC_WHITE   19

  const float count2round_pendulum = 2*PI/(4*360);
  extern ESP32Encoder pendulum_encoder;
#elif ARDUINO
  #include <Encoder.h>

  #define ENA           6
  #define IN1           5
  #define IN2           4
  #define M_ENC_A       3
  #define M_ENC_B       2
  #define ENC_ANALOG    A0

  // const float count2round = 1.0/(4*11*4.4);
  const float count2round = 1.0/(4*11*78);
  extern Encoder motor_encoder;
#endif

float read_pendulum_encoder();
float read_motor_encoder();
float motor(float pwm);
float map_float(float x, float in_min, float in_max, float out_min, float out_max);
void init_lib(float min_pwm);
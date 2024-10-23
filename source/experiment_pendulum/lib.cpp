#include "lib.hpp"

float global_min_pwm = 0.0;

#ifdef ESP32
  #include "esp32-hal-gpio.h"

  ESP32Encoder pendulum_encoder;

  float map_float(float x, float in_min, float in_max, float out_min, float out_max) 
  {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
  }

  float read_pendulum_encoder()
  {
    return pendulum_encoder.getCount()*count2round_pendulum;
  }

  float read_motor_encoder()
  {
    return 0;
  }

  float motor(float pwm)
  {
    float new_pwm = constrain(map_float(abs(pwm), 0, 1, global_min_pwm, 1), 0, 1);

    return new_pwm;
  }

  void init_lib(float min_pwm)
  {
    global_min_pwm = min_pwm;

    ESP32Encoder::useInternalWeakPullResistors = puType::up;
    pendulum_encoder.attachFullQuad(19, 18); 

    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
    for(int i=0; i<200; i++) 
    {
      delay(10);
    }
    pendulum_encoder.clearCount();
    digitalWrite(LED_BUILTIN, LOW);
  }

#elif ARDUINO
  Encoder motor_encoder(M_ENC_A, M_ENC_B);
  float pendulum_offset = 0;

  float map_float(float x, float in_min, float in_max, float out_min, float out_max) 
  {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
  }

  float read_pendulum_encoder()
  {
    float voltage =  (1023 - analogRead(ENC_ANALOG))/1023.0 * 5; 
    return map_float(voltage, 0.5, 4.5, 0, 2*PI) - pendulum_offset;
  }

  float read_motor_encoder()
  {
    return motor_encoder.read()*count2round*2*PI;
  }

  float motor(float pwm)
  {
    float new_pwm = constrain(map_float(abs(pwm), 0, 1, global_min_pwm, 1), 0, 1);
    if (pwm > 0) 
    {
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
    } 
    else if (pwm < 0) 
    {
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
    } 
    else 
    {
      new_pwm = 0;
    }
    analogWrite(ENA, (int)(new_pwm * 255));
    return new_pwm;
  }

  void init_lib(float min_pwm) 
  {
    global_min_pwm = min_pwm;

    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(ENA, OUTPUT);
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);

    digitalWrite(LED_BUILTIN, HIGH);
    float tmp_pendulum_offset = 0;
    for(int i=0; i<200; i++) 
    {
      tmp_pendulum_offset += read_pendulum_encoder();
      delay(10);
    }
    pendulum_offset = tmp_pendulum_offset/200.0;
    digitalWrite(LED_BUILTIN, LOW);

    motor(0.00);
  }
#endif
#include "lib.hpp"

#define MULT 1 // escala para aumentar casas decimais dos números
#define rad_s2RPM 60.0/(2*PI)

float pwm = 0;
const float Ts = 10e-3; // tempo de amostragem para a frequência de amostragem 10x maior que f1

float t0, t, t_trig, last_t, diff;
float u;

void setup() 
{
  Serial.begin(115200);
  init_lib(0.0);


  t0 = micros()/1e6;
  t = t - t0;
}

float position, last_position, speed;

void loop() 
{
  if (Serial.available()) {
    char params[20] = {0};
    size_t m = Serial.readBytesUntil('\n', params, 20);
    params[m] = '\0';
    pwm = atof(params);
  }
  if ((diff = micros() - last_t) >= Ts*1e6) {
    last_t += diff;
    t += diff*1e-6;
    u = pwm;
    motor(u);

    position = read_motor_encoder();
    speed = (position - last_position)/(diff*1e-6);

    Serial.print("l:-1500,L:1500,");
    Serial.println("Speed(RPM):" + String(speed*MULT*rad_s2RPM));

    last_position = position;
  }
}

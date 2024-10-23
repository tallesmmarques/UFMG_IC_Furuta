#include "lib.hpp"

#define MULT 1000 // escala para aumentar casas decimais dos números

const float Ts = 10e-3; 

float t0, t, t_trig, last_t, diff;

void setup() 
{
  Serial.begin(115200);
  init_lib(0.0);

  Serial.println("t;theta_1;theta_2");

  t0 = micros()/1e6;
  t = t - t0;
}

float theta_1, theta_2;

void loop() 
{
  if ((diff = micros() - last_t) >= Ts*1e6) {
    last_t += diff;
    t += diff*1e-6;

    theta_1 = read_motor_encoder();
    theta_2 = read_pendulum_encoder();

    Serial.print(t*MULT);
    Serial.print(";");
    Serial.print(theta_1*MULT);
    Serial.print(";");
    Serial.println(theta_2*MULT);
  }
}

#include "lib.hpp"

#define MULT 1000 // escala para aumentar casas decimais dos números

float f0 = 0.1; // Hz
float f1 = 5; // Hz
float chirp_pwm = 0.50;
const float T = 60; // s
const float p = T*2; // período da onda triangular
const float simulation_time = T*2;
const float Ts = 1.0/(10*f1); // tempo de amostragem para a frequência de amostragem 10x maior que f1

float t0, t, t_trig, last_t, diff;
float u;

void setup() 
{
  Serial.begin(115200);
  init_lib(0.0);

  while(!Serial.available()) {
    delay(10);
  }
  char params[20] = {0};
  size_t m = Serial.readBytesUntil('\n', params, 20);
  params[m] = '\0';
  f0 = atof(strtok(params, ","));
  f1 = atof(strtok(NULL, ","));
  chirp_pwm = atof(strtok(NULL, ","));

  Serial.println("t;u;theta_1");
  t0 = micros()/1e6;
  t = t - t0;
}

void loop() 
{
  if ((diff = micros() - last_t) >= Ts*1e6) {
    last_t += diff;
    t += diff*1e-6;
    t_trig = 2*abs(t/p - floor(t/p + 0.5)) * T; // onda triangular de período p e amplitude [0, T]

    int u_sign = fmod(t, p) <= T ? 1 : -1; // correção de descontinuidades na chirp 

    u = chirp_pwm * sin(-2*PI*f0*f1*T/(f1-f0)*log(1 - (f1-f0)/(f1*T)*t_trig));
    u *= u_sign;
    motor(u);

    Serial.print(t*MULT);
    Serial.print(";");
    Serial.print(u*MULT);
    Serial.print(";");
    Serial.println(read_motor_encoder()*MULT);

    if (t > simulation_time) {
      motor(0);
      digitalWrite(LED_BUILTIN, HIGH);
      while(true) {
        delay(1000);
      }
    }
  }
}

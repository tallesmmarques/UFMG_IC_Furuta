#include <Encoder.h>

#define ENA           6
#define IN1           5
#define IN2           4
#define M_ENC_A       2
#define M_ENC_B       3

#define MIN_PWM 0.40

// Consts
const float count2round = 1/(4*11*4.4); // contagens de pulsos para rotações
const float f0 = 0.5; // Hz
const float f1 = 10; // Hz
const float T = 60; // s
const float p = T*2; // período da onda triangular
const float Ts = 1.0/(10*f1); // tempo de amostragem para a frequência de amostragem 10x maior que f1
const float chirp_pwm = 0.40;
const float simulation_time = T*4;

// Global Variables
Encoder motor_encoder(M_ENC_A, M_ENC_B);
unsigned long last_t;
float t0, t, t_trig;
float u;

// Auxiliar Functions
float mapfloat(float x, float in_min, float in_max, float out_min, float out_max);
float read_motor_encoder();
float motor(float pwm);

void setup() {
  Serial.begin(115200);

  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  Serial.println("t;u;theta_1");

  motor(0.00);
  delay(3000); // tempo para preparar o furuta
  t0 = micros()/1e6;
}

unsigned long diff;
void loop() {
  if ((diff = micros() - last_t) >= Ts*1e6) {
    last_t = micros();
    t = micros()*1e-6 - t0;
    t_trig = 2*abs(t/p - floor(t/p + 0.5)) * T; // onda triangular de período p e amplitude [0, T]

    int u_sign = fmod(t, p) <= T ? 1 : -1; // correção de descontinuidades 

    u = chirp_pwm * sin(-2*PI*f0*f1*T/(f1-f0)*log(1 - (f1-f0)/(f1*T)*t_trig));
    u *= u_sign;
    motor(u);

    Serial.print(t*1000);
    Serial.print(";");
    Serial.print(u*1000);
    Serial.print(";");
    Serial.println(read_motor_encoder()*1000);

    if (t > simulation_time) {
      motor(0);
      while(true) {
        delay(1000);
      }
    }
  }
}

// Auxiliar Functions - Definition
float mapfloat(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

float read_motor_encoder() {
  return motor_encoder.read()*count2round*2*PI;
}

float motor(float pwm) {
  float new_pwm = constrain(mapfloat(abs(pwm), 0, 1, MIN_PWM, 1), 0, 1);
  if (pwm > 0) {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
  } else if (pwm < 0) {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
  } else {
    new_pwm = 0;
  }
  analogWrite(ENA, (int)(new_pwm * 255));
  return new_pwm;
}

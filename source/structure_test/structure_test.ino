#include "lib.hpp"

#define MULT 1

void setup() 
{
  Serial.begin(115200);
  init_lib(0.0);

  // motor(0.3);
}

void loop() 
{
  Serial.print("motor:");
  Serial.print(read_motor_encoder()*MULT);
  Serial.print(",encoder:");
  Serial.print(read_pendulum_encoder()*MULT);
  Serial.println();

  delay(50);
}

#include "SwitecX25.h"

// 315 degrees of range = 315x3 steps = 945 steps
// declare motor1 with 945 steps on pins 4-7
SwitecX25 motor(315*3, 1,2,3,4);

void setup(void) {
  Serial.begin(9600);
  Serial.println("Go!");

  // run motor against stops to re-zero
  motor.zero();   // this is a slow, blocking operation
  motor.setPosition(427);
}

void loop(void) {
  // update motor frequently to allow them to step
  motor.update();
    // do stuff, call motorX.setPosition(step) to 
  // direct needle to new position.
}

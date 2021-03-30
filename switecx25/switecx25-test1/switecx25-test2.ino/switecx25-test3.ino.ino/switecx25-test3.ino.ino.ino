//----------------------------------------------------------------------
// https://github.com/clearwater/SwitecX25
// 
// This is an example of using the SwitchX25 library.
// It zero's the motor, sets the position to mid-range
// and waits for serial input to indicate new motor positions.
// 
// Open the serial monitor and try entering values 
// between 0 and 944.
// 
// Note that the maximum speed of the motor will be determined
// by how frequently you call update().  If you put a big slow
// serial.println() call in the loop below, the motor will move
// very slowly!
//----------------------------------------------------------------------

#include <SwitecX25.h>

// HALL sensor //
int led = 4;   //LED pin
int sensor = 2; //sensor pin
int val; //numeric variable
// HALL sensor //

// standard X25.168 range 315 degrees at 1/3 degree steps
#define STEPS (315*3)

// For motors connected to digital pins 4,5,6,7
SwitecX25 motor1(STEPS,6,7,8,9);

void setup(void)
{
  // run the motor against the stops
  motor1.setPosition(944);
  motor1.zero();
  motor1.setPosition(944);
  // start moving towards the center of the range
  // motor1.setPosition(STEPS/2);
  
  Serial.begin(9600);
  Serial.print("Enter a step position from 0 through ");
  Serial.print(STEPS-1);
  Serial.println(".");

// HALL sensor //
  pinMode(led, OUTPUT); //set LED pin as output
  pinMode(sensor, INPUT); //set sensor pin as input
// HALL sensor //
}

void loop(void)
{
  val = digitalRead(sensor); //Read the sensor
  if(val == LOW) //when magnetic field is detected, turn led on
  {
    digitalWrite(led, HIGH);
  }
  else
  {
    digitalWrite(led, LOW);
  }
  
  static int nextPos = 0;
  // the motor only moves when you call update
  motor1.update();
  
  if (Serial.available()) {
    char c = Serial.read();
    if (c==10 || c==13) {
      motor1.setPosition(nextPos);
      nextPos = 0;
    } else if (c>='0' && c<='9') {
      nextPos = 10*nextPos + (c-'0');
      Serial.print(nextPos);
    }
  }
}

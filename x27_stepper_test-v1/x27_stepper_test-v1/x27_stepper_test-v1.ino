/*------------------------------------------------------
Gauge_Pot01
Arduining.com 10 July 2011
Direct driving of Miniature Stepper Motor used as an indicator
drive for dashboard instrumentation.
Hardware:
-Arduino UNO.
-Potentiometer.
-Stepper Motor VID29-05P (Can be directly driven by MCU):
    -Low current consumption: <20mA 
    -Rotation Angle:Max 315°
    -0.5°/full step    
    -Coil resistance: 280 +/- 20Ω                           
 -----------------------------------------------------*/
#include <SwitecX25.h>
#include <Stepper.h>
#define  STEPS  720    // steps per revolution (limited to 315°)
#define  COIL1  4
#define  COIL2  5
#define  COIL3  6
#define  COIL4  7

#define PotIn  0

// create an instance of the stepper class:
Stepper stepper(STEPS, COIL1, COIL2, COIL3, COIL4);

void setup(){
  stepper.zero();
  stepper.setSpeed(30);    // set the motor speed to 30 RPM (360 PPS aprox.).
  stepper.step(500);       //Reset Position(630 steps counter-clockwise). 
  Serial.begin(9600);    //for debuging.
}
int pos=0;                 //Position in steps(0-630)= (0°-315°)

void loop(){
  int val = analogRead(PotIn);   //get the potentiometer value (range 0-1023)
  stepper.update();
  val= map(val,0,1023,0,300);    // map pot range in the stepper range.

  if(abs(val - pos)> 2){         //if diference is greater than 2 steps.
      if((val - pos)> 0){
          stepper.step(-1);      // move one step to the left.
          pos++;
          }
      if((val - pos)< 0){
          stepper.step(1);       // move one step to the right.
          pos--;
          }
      }
  Serial.println(pos);      //for debuging...
  Serial.println(val);
  delay(1);
}

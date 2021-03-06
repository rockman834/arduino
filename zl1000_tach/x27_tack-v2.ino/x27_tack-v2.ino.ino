//----------------------------------------------------------------------
// https://github.com/clearwater/SwitecX25
//
// This is an example of using the SwitchX25 library.
//
// This code has been rewriten from Guy Carpenters X25 example to
// function as a Tachometer for a CB450 K0 Motorcycle (Tach
// gearing ratio of 1:7)
//
// Note that the maximum speed of the motor will be determined
// by how frequently you call update(). If you put a big slow
// serial.println() call in the loop below, the motor will move
// very slowly!
//----------------------------------------------------------------------

// Replacing the failing analog tach on the Kawasaki ZL1000 with a digital SwitchX25 stepper designed for instuments
// The original igintion system was replaced years ago with a Dyo2000 digital system when the original failed.
// The Dyno2000 has a Hall effect trigger that could be tapped off of, or I could use one of it's outputs directly from the module.
// Haven't decided wich is best yet.

// Using ardiuno NANO connected to digital pins 6,7,8,9 using HiLetgo DRV8833 driver @ ~9VDC (H-Bridges) 
// NANO to DRV8833 pin6 -> AN1, pin7 -> AN2, pin8 -> BN1, pin9 BN2
// Adafruit DRV8833 would also work (X27 stepper connected to DRV8833 pin 1-> AO1, pin 2-> AO2, pin3 -> BO1, pin4 -> B02)
// Using 2 voltage regulators (Droking OSKJ SKU:2001708006), one set for 12VDC in 5VDC out, and 12VDC in and 9VDC out for stepper motor

// This is tricky code, because the stepper is a "zero" when it's at 944 steps, and 315 degrees at 0 steps

// For the ZL1000
// 1 input from Hall each revolution
// NO DIVISORS
// 245 steps == 13000 rpm (MAX), Bike REDLINES at 10500
// 944 steps == 0 rpm

// ######################### BEGIN CODE #############################################
// Guy Carpenters X25 library
#include <SwitecX25.h>

// standard X25.168 & X27.168 range 315 degrees at 1/3 degree steps
#define STEPS (315*3) //totaling 945 steps to complete 315 degrees of rotation

SwitecX25 motor1(STEPS,4,5,6,7);

// digital pin 2 is the hall effect sensor input pin (AT ARDUINO, ADD A 10K RESISTOR BETWEEN PIN 2 AND VIN TO KEEP PIN FROM 'BOUNCING'!!!!)
int hall_pin = 2;

float rotations = 1.0;                      // pick number of rotations to analyze to determin rpm (higher improves accuracy)  

// ??????
float WhileTime2;                           // Time program has spent in "While" loop per half rotation of tach cable   
// ??????

// each rotation trips hall effect 1 time of bar magnet triping hall effect sensor once
// correct number of trips per rotation.
float hall_thresh = (rotations);
float rpm_val;                              // rpm result
int first_run;                              // to mark the first turn of tach crank. due to no previous recording for position of magnet.
int previous_hall_pin_state;                // used to record previous state of hall effect sensor

// ######################### BEGIN SETUP #############################################
void setup(void)
{
motor1.zero();                              // run the motor against the stops at start up   
motor1.setPosition(944);                    // start moving towards the 0 value (for my wiring and tach gauge 0 value is at positon 945)
                                            // changing the wiring may change the 0 position to postion 0. If wiring is changed code will need to be updated.
first_run = 0;                              // Mark that first turn of tach cable has NOT happened yet
pinMode(hall_pin, INPUT);                   // make the hall pin an input:

// initialize serial communication at 9600 bits per second:
// Serial.begin(9600);
}
// ######################### END SETUP #############################################

// ######################### START LOOP #############################################
void loop(void)
{
static int nextPos = 0;        // nextPos will be the value where the needle will be moved too
            // setting nextPos to 0 at begining of loop does not move the motor, it only clears the value to
            // prepare it for its new value
motor1.update();        // the motor only moves when you call update. It will pull the value from the last motor1.setPosition()

            // preallocate values for tach (clear values at begining of new loop for new data and calculations)
float hall_count = 0.0;       // rotations equal 0
float start = micros();       // start timer
WhileTime2 = 0;         // while loop timer
int on_state = 0;       // Marks current status of hall effect sensor (0=ON, 1=OFF)
            
while(hall_count < hall_thresh){    // run "While" loop as long as number of sample rotaions entered in "roataions" above have not been meet
  motor1.update();        // continue to update motor position even when code is stuck in while loop

              // The amount of time needed for 100rpm reading (used to send needle to 0rpm when readings under 100rpm are received
  float hund_rpm_time = ((30000000/10)*hall_thresh);  // Value is in Micro-Seconds
  WhileTime2 = micros()-start;      // The amount of time program has spent in "While" loop for each loop in the "While" loop
    if (WhileTime2 > (hund_rpm_time/14)){
      motor1.setPosition(944);  //Sends motor to position 944 (0 rpms for this code)
    }

// if magnet is triggering hall effect sensor then...
    if (digitalRead(hall_pin)==0){ // hall effect Sending signal (hall_pin = 0)

    if (first_run = 0){   // to hall effect sensor once engine was turned off. reading is false.  
    previous_hall_pin_state = 0;  // mark hall effect sensor as previously on (previous_hall_pin_state = 0)
    first_run = 1;      // mark first_run = 1 so this section of code only runs at the very first turn of crank only
    hall_count+=1.0;    //incriment tach cable rotation value by 1/2 *** MAY NO BE NEEDED ***
    }
    
// if hall effect sensor is on now and was previoulsy off then...
    if (digitalRead(hall_pin)==0 && previous_hall_pin_state==1){  
      if (on_state==0){
      on_state = 1;
      hall_count+=1.0;
      previous_hall_pin_state = 0;
      }
    }
    
// if hall_pin = 1, magnet is NOT triggering hall effect sensor then...
  } else {
    if (first_run = 0){
    previous_hall_pin_state = 1;
    first_run = 1;
    }

// if hall effect sensor is off now and was previoulsy on then...
    if (digitalRead(hall_pin)==1 && previous_hall_pin_state==0){
    on_state = 0;     //set on_state = 0 (set marker to ON) to prepare for next time hall_pin=0 (ON)
    previous_hall_pin_state = 1;
    }
  }

// if number of data gathering cable rotations have been met then exit "while" loop and begin doing RPM calculations
    if (hall_count>hall_thresh){
    break;
    }
  
//continue to update motor position even when code is stuck in while loop
    motor1.update();
}

float end_time = micros();        // record current time to compare to start time
float time_passed = ((end_time-start)/1000000.0); // calculate how much time has passed in seconds (dividing microseconds by 1 million gives you seconds)

// calculate rpm value (hall sensor is tripped once per rotation), then divide by the amount of time that passed to get rotations per second.
// multiply by 60 to get rotations per minute
rpm_val = ((hall_count)/time_passed)*60.0;

// send value to motor to rotate needle.
// (for values greater than 13000) situation where tachometer has reached its max amount (Motorcycle WILL EXPLODE!)
if(rpm_val > 13000){          //ZL1000 tach max at 13000rpm 
nextPos = 245;            // turn gage about 245 deg (X27 turns 945 steps. 945==0, 245 steps==about 245 degrees)
motor1.setPosition(nextPos);        // call needle to rotate to given potition
nextPos = 0;            // clear nextPos value to zero for next calculation

//send value to motor to rotate needle (for values between 0 and 13000, between 0 and 13000 rpm)
} else if(rpm_val >= 0 && rpm_val <= 13000){

// nextPos represents "motor potition" or steps. Since 944 is our zero we will work backwards by subtracting he the steps from our zero mark (944).
// first we need to covert our RPM's to steps. Since we are only using 245 degrees of the motor for the ZL1000 Tach our max steps is 245.
// these 245 degrees will represent a max of 13000 rpms. 13000 rpms of the crank in these 245 degrees.
// so our conversion factor is (245/13000). we multiply our RPM's (rpm_val) by (245/13000) to get the number of steps to subtract from our zero point.
// ..................its not as confusing as I'm explaining. I'm just doing a horrible job.
nextPos = 944-((rpm_val*245)/13000);
motor1.setPosition(nextPos);        // call needle to rotate to given potition
nextPos = 0;            // clear nextPos value to zero for next calculation
  }
}
// ######################### END LOOP #############################################

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

// Guy Carpenters X25 library
#include <SwitecX25.h>

// standard X25.168 & X27.168 range 315 degrees at 1/3 degree steps
#define STEPS (315*3) //totaling 945 steps to complete 315 degrees of rotation

// For motors connected to digital pins 4,5,6,7 (on X25 & X27 pin 1-> pin4, pin 2-> pin5, pin3 -> pin6, pin4 -> pin7)
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
float rpm_val;                              // tach cables rpm result
int first_run;                              // to mark the first turn of tach cable. due to no previous recording for position of magnet.
int previous_hall_pin_state;                // used to record previous state of hall effect sensor

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


void loop(void)
{

// nextPos will be the value where the needle will be moved too
// setting nextPos to 0 at begining of loop does not move the motor, it only clears the value to
// prepare it for its new value
static int nextPos = 0;
// the motor only moves when you call update. It will pull the value from the last motor1.setPosition()
// value in your code
motor1.update();

// preallocate values for tach (clear values at begining of new loop for new data and calculations)

// rotations equal 0
float hall_count = 0.0;
// start timer
float start = micros();
// while loop timer
WhileTime2 = 0;
// Marks current status of hall effect sensor (0=ON, 1=OFF)
// NOTE: THIS WAS USED ON THE EARLY DESIGN OF THE CODE. THE CODE HAS CHANGED SINCE THEN, AND
// I AM UNSURE IF THIS MARKER IS STILL NEEDED. WILL LOOK INTO LATER
int on_state = 0;

// counting number of times the hall sensor is tripped
// While code is in "While loop" code in main loop is paused. be sure to include "motorl.update()"
// In "While" loop to keep motor running smoothly

// run "While" loop as long as number of sample rotaions entered in "roataions" above have not been meet
while(hall_count < hall_thresh){ //remember hall_count = 2*rotations

// continue to update motor position even when code is stuck in while loop
motor1.update();

// The amount of time needed for 100rpm reading (used to send needle to 0rpm when readings under 100rpm are received
// or no data is received and time threshold for 100rpm has been exceded for Honda CB450)
// Value is in Micro-Seconds
float hund_rpm_time = ((30000000/10)*hall_thresh);

// The amount of time program has spent in "While" loop for each loop in the "While" loop
WhileTime2 = micros()-start;

// If time in 1 pass through a "While" loop exceeds time for 100rpm minimum
// then begin to send needle to 0rpm position
// NOTE: INCREASE THE "14" FOR FASTER NEEDLE RESPONSE. I'LL LOOK INTO THIS PART OF THE CODE FURTHER IN THE
// FUTURE TO SEE IF THIS EFFECTS ACCURACY
if (WhileTime2 > (hund_rpm_time/14)){
//Sends motor to position 944 (0 rpms for this code)
motor1.setPosition(944);
}

// if magnet is triggering hall effect sensor then...
if (digitalRead(hall_pin)==0){ // hall effect Sending signal (hall_pin = 0)

// if first_run = 0 then the Tach cable has not turned yet. Thus, during previous drive Tach magnet came to rest next
// to hall effect sensor once engine was turned off. reading is false.
if (first_run = 0){
// mark hall effect sensor as previously on (previous_hall_pin_state = 0)
previous_hall_pin_state = 0;
// mark first_run = 1 so this section of code only runs at the very first turn of tach cable only
first_run = 1;

// incriment tach cable rotation value by 1/2 (remember that 2 counts are equal to 1 rotation of the cable)
// TECHNICALLY THE CABLE HAS NOT TURNED AT ALL. INCIMENTING MAY NOT BE NEEDED HERE. HOWEVER, IF INCIMENTING
// HERE IS PRODUCING A FALSE VALUE IT IS ONLY FOR THE 1ST VALUE AND THEN IGNORED. ARITHMETIC ERROR IS UN-NOTICABLE
hall_count+=1.0;
}

/// if hall effect sensor is on now and was previoulsy off then...
if (digitalRead(hall_pin)==0 && previous_hall_pin_state==1){

/// if on_state is equal to 0 (ON) then set on_state equal to 1 (OFF) to prepare for next pass,
// increase hall_count, change previous_hall_pin_state=0 (ON)
if (on_state==0){
on_state = 1;
hall_count+=1.0;
previous_hall_pin_state = 0;
}
}

// if hall_pin = 1, magnet is NOT triggering hall effect sensor then...
} else{

// if you reach this point in code and still first_run = 0
// then Tach cable has not turned yet. And you know that the end of the Tach bar magnet
// did NOT come to rest next to hall effect sensor when engine was last turned off
// mark hall effect sensor as previously off (previous_hall_pin_state = 1)
if (first_run = 0){
previous_hall_pin_state = 1;
// mark first_run = 1 so this section of code only runs at the very first turn of tach cable only
first_run = 1;
}

// if hall effect sensor is off now and was previoulsy on then...
if (digitalRead(hall_pin)==1 && previous_hall_pin_state==0){

//set on_state = 0 (set marker to ON) to prepare for next time hall_pin=0 (ON)
on_state = 0;
// change previous_hall_pin_state=1 (OFF)
previous_hall_pin_state = 1;
}
}

// if number of data gathering cable rotations have been met then exit "while" loop
// and begin doing RPM calculations
if (hall_count>hall_thresh){
break;
}

//continue to update motor position even when code is stuck in while loop
motor1.update();
}

// record current time to compare to start time
float end_time = micros();

// calculate how much time has passed in seconds (dividing microseconds by 1 million gives you seconds)
float time_passed = ((end_time-start)/1000000.0);

// calculate rpm value (hall sensor is tripped twice per rotation, so divide hall_count in half to get
// number of rotations. then divide by the amount of time that passed to get rotations per second.
// multiply by 60 to get rotations per minute
rpm_val = ((hall_count/2)/time_passed)*60.0;

// send value to motor to rotate needle.
// NOTE: CB450 TACH GEARING IS 1:7. SO RPM'S WILL BE (1/7)OF THE MOTORS ACTUAL RPM's

// (for values greater than 1571) situation where tachometer has reached its max amount (Motorcycle about to EXPLODE!)
if(rpm_val > 1571){ //CB450 K0 tach max at 11000rpm (tach ratio of 1:7. 11000/7=1571)
nextPos = 472; // turn gage about 180 deg (X27 turns 945 steps. 945/2=472). (ACTUAL ANGLE FOR CB450 IS SLIGHLTY LESS. WILL WORK OUT LATER)
motor1.setPosition(nextPos); // call needle to rotate to given potition
nextPos = 0; // clear nextPos value to zero for next calculation

//send value to motor to rotate needle (for values between 0 and 1571, between 0 and 11000 rpm)
}else if(rpm_val >= 0 && rpm_val <= 1571){

// nextPos represents "motor potition" or steps. Since 944 is our zero we will work backwards by subtracting he the steps from our zero mark (944).
// first we need to covert our RPM's to steps. Since we are only using 180 degrees of the motor for the CB450 K0 Tach our max steps is 472 (half of 945).
// these 180 degrees will represent a max of 11000 rpms. Since our gearing ratio is 1/7th of the motor that is a max of 11000/7= 1571 rpms of the cable in these 180 degrees.
// so our conversion factor is (472/1571). we multiply our RPM's (rpm_val) by (472/1571) to get the number of steps to subtract from our zero point.
// ..................its not as confusing as I'm explaining. I'm just doing a horrible job.
nextPos = 944-((rpm_val*472)/1571);
motor1.setPosition(nextPos); // call needle to rotate to given potition
nextPos = 0; // clear nextPos value to zero for next calculation

}

}

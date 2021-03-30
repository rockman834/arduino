#include <Servo.h> //Use the Servo librarey for generating PWM
Servo ESC; //name the servo object, here ESC

void setup()
{
ESC.attach(9); //Generate PWM in pin 9 of Arduino
}

void loop()
{
int throttle = analogRead(A0); //Read the voltage from POT
throttle = map(throttle, 0, 1023, 0, 180); //Map the values of 0-102 from pot to 0-180 bcs servo works only from 0-180
ESC.write(throttle); //based on the value of throttle generate PWM signal
}

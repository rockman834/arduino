#include<LiquidCrystal.h>
LiquidCrystal lcd(7, 8, 9, 10, 11, 12); // LCD connected to Arduino Nono and Uno pins 7 thru 12 (see the Adafruit LCD wiring link for help with wiring LCD)
float value=0;
float rev=0;
int rpm;
int oldtime=0;
int time;

void isr() //interrupt service routine
{
rev++;
}

void setup()
{
lcd.begin(16,2);                //initialize LCD
attachInterrupt(0,isr,FALLING);  //attaching the interrupt to Digital Pin #2 on Arduino Nano and Uno (int 0 pin)
}

void loop()
{
delay(1000);
detachInterrupt(0);           //detaches the interrupt
time=millis()-oldtime;        //finds the time 
rpm=(rev/time)*30000;         //calculates rpm   30000 for 2 magnets, 60000 for 1 magnet
oldtime=millis();             //saves the current time
rev=0;
lcd.clear();
lcd.setCursor(0,0);
lcd.print("Spindle Tach");
lcd.setCursor(0,1);
lcd.print(     rpm);
lcd.print(" RPM");
lcd.print("   ");
attachInterrupt(0,isr,FALLING);

}

/*
  RPM meter code (tested on device rotating at 10,000 rpm) 
  rev.001
  * using a hall effect sensor
  http://www.adafruit.com/index.php?main_page=product_info&cPath=35&products_id=158
  * using a parallel LCD with
  LiquidCrystal Library
  http://www.ladyada.net/learn/lcd/charlcd.html
  http://www.arduino.cc/en/Tutorial/LiquidCrystal
  with pins 7,8,9,10,11,12
*/

// include the library code:
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(7,8,9,10,11,12);
// read the hall effect sensor on pin 2
const int hallPin=2;
const unsigned long sampleTime=1000;
const int maxRPM = 10200; 

void setup() 
{
  pinMode(hallPin,INPUT);
  Serial.begin(9600);
  // set up the LCD's number of rows and columns: 
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("initializing");
  delay(1000);
  lcd.clear();
}

void loop() 
{
  delay(100);
  int rpm=getRPM();
  lcd.clear();
  displayRPM(rpm);
  displayBar(rpm);
}
 
int getRPM()
{
  // sample for sampleTime in millisecs
  int kount=0;
  boolean kflag=LOW;
  unsigned long currentTime=0;
  unsigned long startTime=millis();
  while (currentTime<=sampleTime)
  {
    if (digitalRead(hallPin)==HIGH)
    {
      kflag=HIGH;
    }
    if (digitalRead(hallPin)==LOW && kflag==HIGH)
    {
      kount++;
      kflag=LOW;
    }
    currentTime=millis()-startTime;
  }
  int kount2rpm = int(60000./float(sampleTime))*kount;
  return kount2rpm;
}
    
void displayRPM(int rpm) 
{
  lcd.clear();
  // set the cursor to column 0, line 1
  lcd.setCursor(0, 0); 
  // print the number of seconds since reset:
  lcd.print(rpm,DEC);
  lcd.setCursor(7,0);
  lcd.print("RPM");
}

void displayBar(int rpm)
{
  int numOfBars=map(rpm,0,maxRPM,0,15);
  lcd.setCursor(0,1);
  if (rpm!=0)
  {
  for (int i=0; i<=numOfBars; i++)
   {
        lcd.setCursor(i,1);
        lcd.write(1023);
      }
  }
} 

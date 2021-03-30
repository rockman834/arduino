#include <LiquidCrystal.h>
LiquidCrystal lcd(13, 12, 5, 4, 3, 2);
int Pwmpin=9;//Pwm Output Pin
int Fchange= A0;//Frequency change through Potentiometer
//int Button=1;//Button to change the frequency
void setup()
{
lcd.begin(16, 2);  // set up the LCD's number of columns and rows: 
pinMode(Pwmpin, OUTPUT);//Pwm pin as Output0
//pinMode(Button, INPUT);//Button as Input
TCCR1A=_BV(COM1A1)|_BV(COM1B1);//Non-inverted Mode
TCCR1B=_BV(WGM13)|_BV(CS11);//Prescalar 8
lcd.setCursor(0, 0);//Lcd Coulomb 0 Row 1
lcd.print("Pwm Period = 50%");
}

void loop(){
float freq=0;
float count=10000,countt=0,Pinput=0;

while(1){
ICR1=count;//variable Frequency
countt=2*8*count;
freq= int(16000000/countt);
OCR1A=int(count/2);
lcd.setCursor(0, 1);//Lcd Coulomb 0 Row 2
lcd.print("Pwm Freq =");
lcd.print(freq);
count=10000;
Pinput=analogRead(A0);//Read input value
Pinput=(Pinput/0.0113);
count=count+Pinput;
if(count>=100000)
  {
    count=10000;
  }
  delay(1000);
}
}

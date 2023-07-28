//define ledPin 8
//define sensorPin A0
#include "HX711.h"
#define DOUT  4
#define CLK  5
#include <LiquidCrystal.h>
const int rs = 12, en = 11, d4 = 2, d5 = 3, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
#include <MQ2.h>
#include <Wire.h> 
int redLed = 13;
int greenLed = 8;
int smokeA0 = A5;
//int co;
int sensorThres = 350;
MQ2 mq2(smokeA0);
long sample=0;
float val=0;
long count=0;
HX711 scale(DOUT, CLK);
float calibration_factor = -250; // this calibration factor must be adjusted according to your load cell
float units;
#include <SoftwareSerial.h>
SoftwareSerial mySerial(9, 10);
void setup (){
 Serial.begin(9600);
 Serial.println("HX711 calibration sketch");
 Serial.println("Remove all weight from scale");
 Serial.println("After readings begin, place known weight on scale");

 mySerial.begin(9600);
 mq2.begin();
 lcd.begin(16, 2);
 lcd.print(" Weight ");
 lcd.setCursor(0,1);
 lcd.print(" Measurement ");
 delay(2000);
 lcd.clear();
 lcd.print("Put some weight");
 delay(1000);
 lcd.clear();
 pinMode(redLed, OUTPUT);
 pinMode(greenLed, OUTPUT);
  scale.set_scale(calibration_factor); //Adjust to this calibration factor
 scale.tare();  //Reset the scale to 0
 long zero_factor = scale.read_average(); //Get a baseline reading

}
void loop(){
 Serial.print("Reading");
 units = scale.get_units(), 5;
 if (units < 0){
  units = 0.00;
 }
 delay(1000);
 lcd.clear();
 lcd.print("Weight:");
 lcd.print(units);
 delay(1000);
 lcd.clear();
 Serial.print("Weight: ");
 Serial.print(units);
 Serial.print(" grams"); 
 Serial.print(" calibration_factor: ");
 Serial.print(calibration_factor);
 Serial.println();
 if(units<950){
    Serial.println("message can be sent now");
    SendMessage();
    Serial.println("sent");
   }
 delay(1000);


 if(Serial.available())
 {
   char temp = Serial.read();
   if(temp == 't' || temp == 'T')
     scale.tare();  //Reset the scale to zero      
 }
 readSensor();
  
} 

 void SendMessage()
{
  mySerial.println("AT+CMGF=1");  
  //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1000 milli seconds or 1 second
  mySerial.println("AT+CMGS=\"8106519629\"\r"); // Replace x with mobile number
  delay(1000);
  mySerial.print("Gas needs to be refilled,ID No :- 23004247,Please confirm with the User");// The SMS text you want to send
  delay(100);
  mySerial.print((char)26);// ASCII code of CTRL+Z
  delay(1000);
}

int readSensor()
{
  unsigned int analogSensor = analogRead(smokeA0); 
  Serial.print("Pin A0: ");
  Serial.println(analogSensor);
  // Checks if it has reached the threshold value
  if (analogSensor > sensorThres)
  {
    digitalWrite(redLed, HIGH);
    digitalWrite(greenLed, LOW);
  }
  else
  {
    digitalWrite(redLed, LOW);
    digitalWrite(greenLed, HIGH);
  }
  delay(100);
}

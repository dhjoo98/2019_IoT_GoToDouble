#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x3f, 16, 2);

int trigPin = 13;
int echoPin = 12;

void setup()
{
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // initialize the LCD
  lcd.begin();
  
 

  // Turn on the blacklight and print a message.
  lcd.setCursor(3, 0);
lcd.print("room1");

lcd.setCursor(9, 0);
lcd.print("room2");


lcd.setCursor(0, 1);
lcd.print("Num");
lcd.setCursor(11, 1);
lcd.print("2");

}

void loop()
{
   long duration, distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 17 / 1000;
  Serial.println(distance);
  delay(100);

  if (distance <= 5)
  {lcd.setCursor(5, 1);
  lcd.print("1");
  }
  else {lcd.setCursor(5, 1);
   lcd.print("0");}
 
  // Do nothing here...
}

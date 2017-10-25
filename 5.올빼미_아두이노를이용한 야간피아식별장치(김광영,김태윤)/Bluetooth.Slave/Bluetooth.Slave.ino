//this is slave

#include <SoftwareSerial.h>

SoftwareSerial BTSerial(2, 3);

int state = 0;
const int led = 8;
const int button = 9;
int buttonstate = 1;

void setup() 
{
  BTSerial.begin(38400);
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);
  pinMode(button, INPUT);
  digitalWrite(button, HIGH);
}
void loop() 
{
 if(BTSerial.available() > 0)
 { 
    // Checks whether data is comming from the serial port
    state = BTSerial.read(); // Reads the data from the serial port
 }
 
 // Reading the button
 buttonstate = digitalRead(button);
 if (buttonstate == LOW) 
 {
   BTSerial.write('1'); // Sends '1' to the master to turn on LED
 }
 else
 {
   BTSerial.write('0');
 }  

  if (state == '1') 
 {
  digitalWrite(led, HIGH); // LED ON
  state = 0;
 }
 else if (state == '0') 
 {
  digitalWrite(led, LOW); // LED ON
  state = 0;
 }
}

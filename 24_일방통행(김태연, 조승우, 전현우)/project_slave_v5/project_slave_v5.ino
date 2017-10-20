#include <SoftwareSerial.h>
#include <MsTimer2.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include<Servo.h>

//DEFINE SOFTWARESERIAL PORT
SoftwareSerial master_port(12,13);

//DEFINE LCD
LiquidCrystal_I2C lcd(0x3F, 16, 2);

//LED
int LED_R = 2;
int LED_G = 3;
int LED_Y = 4;

//BUTTON
int BUTTON_PASSIVE = 8;
int BUTTON_WAIT = 6;
int BUTTON_OUT = 7;
int BUTTON_IN = 5;

int btnstate_wait, btnstate_out, btnstate_in, btnstate_passive;
int prevstate_wait = 1;
int prevstate_out = 1;
int prevstate_in = 1;
int prevstate_passive = 1;

int num_wait = 0;
int num_out = 0;
int num_in = 0;
int num_passive = 0;

char value;
char inData;
char value_temp;
char inChar;
int index = 0;
int carcrossing = 0;


//BUZZER
int BUZZER = 9;
const int beepFrequency = 900;
const int beepDuration = 500;

//PASSIVE_LED_FLASH
void LED_Y_Flash() {
  static boolean output = HIGH;
  digitalWrite(4, output);
  output = !output;
  tone(BUZZER, beepFrequency, beepDuration);    
}

//SERVO
Servo servo;


void setup() {
//SERIAL MONITOR SETUP
  Serial.begin(9600);
  master_port.begin(9600);

//PIN SETUP
  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_Y, OUTPUT);

//TIMER SETUP
  MsTimer2::set(500, LED_Y_Flash);

//STAND BY NEXT VALUE
  digitalWrite(LED_R, HIGH);

//LCD SETUP
  lcd.begin();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.println("On Driving : "); // (13,0)
  lcd.setCursor(0,1);
  lcd.print("Waiting Cars: "); // (11,1)

//SERVO SETUP
  servo.attach(10);
}


void loop() {
  
//RECEIVE VALUE
  if(master_port.available()>0){
    value = (char)master_port.read();
      Serial.print("====================\n들어온 값:\t");
      Serial.println(value);
      switch(value){
        case 'r' :  MsTimer2::stop();    
                    digitalWrite(LED_R, HIGH);
                    digitalWrite(LED_G, LOW);
                    digitalWrite(LED_Y, LOW);
                    value_temp = value;
                    num_passive = 0;
                    servo.write(90);
                    break; 
                   
        case 'g' :  digitalWrite(LED_R, LOW);
                    digitalWrite(LED_G, HIGH);
                    digitalWrite(LED_Y, LOW);
                    value_temp = value;
                    servo.write(0);                    
                    break; 
                   
        case 'y' :  digitalWrite(LED_R, LOW);
                    digitalWrite(LED_G, LOW);
                    MsTimer2::start();
                    value_temp = value;
                    servo.write(0);
                    num_passive = 1;
                      
                    break;
        case 'a' :  if(master_port.available()>0){
                    inData = (char) master_port.read(); // Read a character
                    Serial.print(inData);
                    lcd.clear();
                    lcd.setCursor(0,0);
                    lcd.println("On Driving : "); // (13,0)
                    lcd.setCursor(13,0);
                    lcd.write(carcrossing);
                    lcd.setCursor(0,1);
                    lcd.print("Waiting Cars: "); // (11,1)
                    lcd.setCursor(15,1);
                    lcd.print(inData);

                    }
                    break;
        default  :  carcrossing = value; 
                    break;
     }
  }
    

  /* 0 - 9 */
  
//WAIT BUTTON  
  btnstate_wait = digitalRead(BUTTON_WAIT);

  if(btnstate_wait != prevstate_wait){
    if(btnstate_wait == 0){
      if(num_wait == 0){
        num_wait ++;
      }
      else{
        master_port.print("w");
        Serial.print("w");       
        num_wait = 0;
      }
    }
    prevstate_wait = btnstate_wait;
  }

//OUT BUTTON
  btnstate_out = digitalRead(BUTTON_OUT);

  if(btnstate_out != prevstate_out){
    if(btnstate_out == 0){
      if(num_out == 0){
        num_out ++;
      }
      else{
        master_port.print("o");
          Serial.print("o");
        num_out = 0;
      }
    }
    prevstate_out = btnstate_out;
  }

//IN BUTTON  
  btnstate_in = digitalRead(BUTTON_IN);

  if(btnstate_in != prevstate_in){
    if(btnstate_in == 0){
      if(num_in == 0){
        num_in ++;
      }
      else{
        if(value_temp != 'r'){
          master_port.print("i");
          Serial.print("i");
        }
        num_in = 0;
      }
    }
    prevstate_in = btnstate_in;
  }

//PASSIVE BUTTON
  btnstate_passive = digitalRead(BUTTON_PASSIVE);

  if(btnstate_passive != prevstate_passive){
    if(btnstate_passive == 0){
      if(num_passive == 0){
        master_port.print("p");
        BUTTON_WAIT = 0;
        BUTTON_OUT = 0;
        BUTTON_IN = 0;            
        num_passive ++;
      }
      else{
        master_port.print('a');
        BUTTON_WAIT = 6;
        BUTTON_OUT = 7;
        BUTTON_IN = 5;
        num_wait = 0;
        num_out = 0;
        num_in = 0;         
        num_passive = 0;
      }
    }
    prevstate_passive = btnstate_passive;
  }
  delay(10);
}

#include <Servo.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x3F,16,2);  
Servo servo1;
char sw1 = 3;
char st1;
int count = 0;
int present = 3;
int af[4] = {};

void setup() {
  Serial.begin(9600);
  servo1.attach(9);
  //entrance sw
  pinMode(sw1,INPUT);
  
  //sensor
  pinMode(A0,INPUT);
  pinMode(A1,INPUT);
  pinMode(A2,INPUT);
  pinMode(A3,INPUT);

  //3-LED
  pinMode(12,OUTPUT);
  pinMode(13,OUTPUT);
  
  // lcd 설정
  lcd.init();                     
  lcd.backlight();
}
void loop() {
  int an[4];
    
  an[0] = map(analogRead(A0),30,800,0,100);
  an[1] = map(analogRead(A1),0,280,0,100);
  an[2] = map(analogRead(A2),100,500,0,100);
  an[3] = map(analogRead(A3),100,500,0,100);

  for(int i = 0 ; i < 4 ; i ++){
    if(an[i] < 10)
      af[i]++;
    else
      af[i] = 0;
  }
  if(count > 0)
    count++;

  if( present == 0 ){
    lcd.setCursor(0,0);
    lcd.print("Can't find");
    lcd.setCursor(0,1);
    lcd.print("a parking spot     ");
    if(count > 0)
      count +=10;
  }
  
  else if(count == 0){
    lcd.setCursor(0,0);
    lcd.print("Remaining parkin");
    lcd.setCursor(0,1);
    lcd.print("g spot : ");
    lcd.print(present);
    lcd.print("    ");
  }

  //Entrance
  st1 = digitalRead(sw1);
  if(present > 0 ){
    if(st1 == HIGH){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Entrance");
      if(count == 0){
        present--;
      }
      delay(20);
      count = 1;
    }
  }
  
  //Exit
  if(present < 3 ){ 
    if(af[0] > 10){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Exit");
      delay(20);
      if(count == 0){
        present++;
      }
      count = 1;
    }
  }

  
  //servo open , close
  if(count >= 400)
    count = 0;
  if(count > 0){
    servo1.write(45);
    digitalWrite(13,HIGH);      
    digitalWrite(12,LOW);    
  }    
  else if(count == 0){
    servo1.write(3); 
    if(present == 0 ){
      digitalWrite(13,HIGH);

    }
    else{
      digitalWrite(13,LOW);    
      digitalWrite(12,HIGH);    
    }
  }    
  delay(5);
  for(int i = 1 ; i < 4 ; i ++){
    if(an[i] < 13){
      if(af[i] >= 20){
        digitalWrite(i+4,LOW);
      }
    }
    else
      digitalWrite(i+4,HIGH);
  }
}

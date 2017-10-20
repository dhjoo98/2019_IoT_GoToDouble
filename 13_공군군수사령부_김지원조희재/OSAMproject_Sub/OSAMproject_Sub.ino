#include <SoftwareSerial.h>
#include <Wire.h>

SoftwareSerial mySerial(2, 3); //블루투스의 Tx, Rx핀을 2번 3번핀으로 설정

int LED1 = 8;
int LED2 = 9;
int LED3 = 10;
int LED4 = 11;
int LED5 = 12;
int LED6 = 13;
int LED_state;
int ONOFF[6]={1,1,1,1,1,1};
int input_value=0;
int led_z =0;
int escape_value=0; 

void setup()
{
  Serial.begin(9600);
  mySerial.begin(9600);
  pinMode(8,OUTPUT);
  pinMode(9,OUTPUT);
  pinMode(10,OUTPUT);
  pinMode(11,OUTPUT);
  pinMode(12,OUTPUT);
  pinMode(13,OUTPUT);

  digitalWrite(8,HIGH);
  digitalWrite(9,HIGH);
  digitalWrite(10,HIGH);
  digitalWrite(11,HIGH);
  digitalWrite(12,HIGH);
  digitalWrite(13,HIGH);
}



void loop() 
{ 

  escape_value = 0;
  while(!mySerial.available())
  {
    ;
  }
  led_z=mySerial.read();
  if(led_z==79){

  if(ONOFF[0]==1)  mySerial.write('A');
  else if(ONOFF[0]==0) mySerial.write('a');
  else mySerial.write('A');
  
  if(ONOFF[1]==1)  mySerial.write('B');
  else if(ONOFF[1]==0) mySerial.write('b');
  else mySerial.write('B');

  if(ONOFF[2]==1)  mySerial.write('C');
  else if(ONOFF[2]==0) mySerial.write('c');
  else mySerial.write('C');

  if(ONOFF[3]==1)  mySerial.write('D');
  else if(ONOFF[3]==0) mySerial.write('d');
  else mySerial.write('D');
  
  if(ONOFF[4]==1)  mySerial.write('E');
  else if(ONOFF[4]==0) mySerial.write('e');
  else mySerial.write('E');

  if(ONOFF[5]==1)  mySerial.write('F');
  else if(ONOFF[5]==0) mySerial.write('f');
  else mySerial.write('F');
 
  }

  while(escape_value=1)
  {
  while(!mySerial.available()){
    ;
  }
  input_value = mySerial.read();

  if(input_value == 88)
  {
    escape_value = 1;
  }
  switch(input_value){
   case 79:

      if(ONOFF[0]==1)  mySerial.write('A');
      else if(ONOFF[0]==0) mySerial.write('a');
      else mySerial.write('A');
      
      if(ONOFF[1]==1)  mySerial.write('B');
      else if(ONOFF[1]==0) mySerial.write('b');
      else mySerial.write('B');
    
      if(ONOFF[2]==1)  mySerial.write('C');
      else if(ONOFF[2]==0) mySerial.write('c');
      else mySerial.write('C');
    
      if(ONOFF[3]==1)  mySerial.write('D');
      else if(ONOFF[3]==0) mySerial.write('d');
      else mySerial.write('D');
      
      if(ONOFF[4]==1)  mySerial.write('E');
      else if(ONOFF[4]==0) mySerial.write('e');
      else mySerial.write('E');
    
      if(ONOFF[5]==1)  mySerial.write('F');
      else if(ONOFF[5]==0) mySerial.write('f');
      else mySerial.write('F');
 
       break;
    case 65:
    digitalWrite(8,HIGH);
    ONOFF[0]=1;
    break;
    
    case 66:
    digitalWrite(9,HIGH);
    ONOFF[1]=1;
    break;
    
    case 67:
    digitalWrite(10,HIGH);
    ONOFF[2]=1;
    break;
    
    case 68:
    digitalWrite(11,HIGH);
    ONOFF[3]=1;
    break; 
       
    case 69:
    digitalWrite(12,HIGH);
    ONOFF[4]=1;
    break;
    
    case 70:
    digitalWrite(13,HIGH);
    ONOFF[5]=1;
    break;
    
    case 97:
    digitalWrite(8,LOW);
    ONOFF[0]=0;
    break;
    
    case 98:
    digitalWrite(9,LOW);
    ONOFF[1]=0;
    break;
    
    case 99:
    digitalWrite(10,LOW);
    ONOFF[2]=0;
    break;
    
    case 100:
    digitalWrite(11,LOW);
    ONOFF[3]=0;
    break;
    
    case 101:
    digitalWrite(12,LOW);
    ONOFF[4]=0;
    break;
   
    case 102:
    digitalWrite(13,LOW);
    ONOFF[5]=0;
    break;
  }

  }
}

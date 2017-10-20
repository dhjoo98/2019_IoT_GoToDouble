#include <AddicoreRFID.h>
#include <SPI.h>
#include <DS1302.h>
#include <Servo.h> 
#include <SoftwareSerial.h> 

#define  uchar unsigned char
#define uint  unsigned int
#define MAX_LEN 16



//RFID Pins
const int chipSelectPin = 10;
const int NRSTPD = 9;

//Servo Pins
const int servoPin = 7;
int servoAngle = 90;

//Buzzer pin
const int buzzerPin = 8;

//Bluetooth Pins
const int blueTx=6;   //Tx (보내는핀 설정)at
const int blueRx=5;   //Rx (받는핀 설정)

SoftwareSerial bluetooth(blueTx, blueRx); 
Servo servo;
AddicoreRFID myRFID;

void setup() {                
  Serial.begin(9600);                        // RFID reader SOUT pin connected to Serial RX pin at 9600bps 
 
  // start the SPI library:
  SPI.begin();
  
  pinMode(chipSelectPin,OUTPUT);              // Set digital pin 10 as OUTPUT to connect it to the RFID /ENABLE pin 
  digitalWrite(chipSelectPin, LOW);         // Activate the RFID reader
  pinMode(NRSTPD,OUTPUT);                     // Set digital pin 10 , Not Reset and Power-down
  digitalWrite(NRSTPD, HIGH);

  myRFID.AddicoreRFID_Init();
   
  servo.attach(servoPin);

  bluetooth.begin(9600);
}

void loop()
{
  uchar i, tmp, checksum1;
  uchar status;
  uchar str[MAX_LEN];
  uchar RC_size;
  uchar permission="";
  uchar blockAddr;  //Selection operation block address 0 to 63
  int timeout = 0;
  String mynum = "";
  str[1] = 0x4400;

  status = myRFID.AddicoreRFID_Request(PICC_REQIDL, str);
  
  //Anti-collision, return tag serial number 4 bytes
  status = myRFID.AddicoreRFID_Anticoll(str);
  if (status == MI_OK)
  {
          tone(buzzerPin, 130.8, 50);
          checksum1 = str[0] ^ str[1] ^ str[2] ^ str[3];
          
          bluetooth.write(checksum1);
          bluetooth.write(str[0]);
          bluetooth.write(str[1]);
          bluetooth.write(str[2]);
          bluetooth.write(str[3]);
          
          while( timeout < 100 )
          {
            if( bluetooth.available() != 0 )
              break;
            delay(10);
            ++timeout;
          }
          
          permission = bluetooth.read();
          
          //Open door
          if( permission == 'p' )                   
          {
              uchar tstStr[MAX_LEN];
              Serial.println("Permission acquired");
              tone(buzzerPin, 523.3, 1000);
              for( servoAngle; servoAngle < 180; servoAngle++ ) 
              { 
                servo.write(servoAngle); 
                delay(5); 
              }
              
              delay(5000);

              //Check person keeps his/her card on RFID reader
              while( myRFID.AddicoreRFID_Anticoll(tstStr) == MI_OK )
              {
                delay(1000);
              }
              
              for(servoAngle; servoAngle > 90; servoAngle--) 
              { 
                
                servo.write(servoAngle); 
                delay(5); 
              }
              
          }
          else
          {
            tone(buzzerPin, 246.9, 1000);
            Serial.println("Permission denied");
            delay(2000); 
          }
          
          delay(2000);
          
  }
  permission = "";
  timeout = 0;
  myRFID.AddicoreRFID_Halt();              

}


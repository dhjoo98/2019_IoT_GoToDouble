#include <SPI.h>
#include <AddicoreRFID.h>
#include <IRremote.h>

#define MAX_LEN 16
#define uchar unsigned char
#define uint unsigned int

#define RFID_SDA 10
#define RFID_SCK 13
#define RFID_MOSI 11
#define RFID_MISO 12
#define RFID_RST 9

#define IR_IN 2
#define LED_OUT 4

uchar serNumA[5];
uchar fifobytes;
uchar fifoValue;

AddicoreRFID cardRead;
IRrecv irrecv(IR_IN);
decode_results results;
int readsig = 0;

void setup() {
  Serial.begin(9600);
  SPI.begin();
  while (! Serial);
  
  pinMode(RFID_SDA,OUTPUT);
  pinMode(RFID_RST,OUTPUT);
  pinMode(LED_OUT,OUTPUT);
  digitalWrite(RFID_SDA,LOW);
  digitalWrite(RFID_RST,HIGH);
  Serial.println("Scan card...\r\n");

  Serial.println("Enabling IRin");
  cardRead.AddicoreRFID_Init();
  irrecv.enableIRIn();
  Serial.println("Enabled IRin");
}

void loop() {
  uchar status;
  uchar str[MAX_LEN];
  str[1] = 0x4400;
  
  if (irrecv.decode(&results)) {
    if (results.decode_type == NEC) {
      switch(results.value){
        case 0xFFC23D:
          digitalWrite(LED_OUT,HIGH);

          // timer here
          
          while ( 1 ) {
            status = cardRead.AddicoreRFID_Request(PICC_REQIDL, str);
            status = cardRead.AddicoreRFID_Anticoll(str);
            if ( status == MI_OK ){
              break;
            }
          }
          if ( status == MI_OK ){
            if ( str[0] == 168 && str[1] == 24 && str[2] == 108 && str[3] == 39 ){
              Serial.println("<< OK !!! >>  Registered card...");
              // timer end here
              // Serial.println(timer);
            } else {
              Serial.println("<< ER !!! >>  New card detected...");
            }
            results.value = NULL;
          }
          
          Serial.println("Same");
        default:
          digitalWrite(LED_OUT,LOW);
          break;
      }
    }
  }
  irrecv.resume(); // Receive the next value
  delay(250);
}

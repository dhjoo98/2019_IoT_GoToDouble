#include "DHT.h"                           // 온습도 센서 라이브러리
#include <Servo.h>                         // 모터 라이브러리
#include <SoftwareSerial.h>                // 블루투스 시리얼 통신 라이브러리
#include <MFRC522.h>                       // RFID 라이브러리
#include <SPI.h>                           // RFID 라이브러리
#include <ArduinoJson.h>                   // JSON 라이브러리

#define uchar    unsigned char                                             
#define uint     unsigned int
#define DHTPIN   8                         // 온습도 센서 핀
#define DHTTYPE  DHT11                     // 온습도 센서 종류
#define RST_PIN  9                         // RFID 핀
#define SS_PIN   10                        // RFID 데이터를 주고받는 역할의 핀(SS = Slave Select)
#define BLUE_TX  2                         // 블루투스 보내는 핀
#define BLUE_RX  3                         // 블루투스 받는 핀
#define SERVOPIN 4                         // 모터 핀
#define ID_SIZE  11                        // 군번의 최대 자리수(병사가 10 + 마지막 종료 문자열)
#define REDPIN   5                         // LED 빨간 핀
#define SPEAKERPIN 6                         // LED 초록 핀
#define BLUEPIN  7                         // LED 파란 핀

Servo servo1;                              // 서보 모터 변수
SoftwareSerial mySerial(BLUE_TX, BLUE_RX); // 블루투스 시리얼 통신 변수
DHT dht(DHTPIN, DHTTYPE);                  // 온습도 변수
String id;                                 // RFID로부터 읽어온 ID 변수

void setup()
{
  randomSeed(analogRead(0));

  pinMode(REDPIN, OUTPUT);
  pinMode(BLUEPIN, OUTPUT);
  servo1.attach(SERVOPIN);  

  Serial.begin(9600);
  mySerial.begin(9600);
  SPI.begin();  
}

void loop() {
  // 블루투스에 읽을 값이 있으면 위험상황이므로 알맞은 센서동작을 해준다.
  while(mySerial.available()){
    char danger = mySerial.read();
    if( danger == '!' ) alert();
  }

  // 새로운 RFID 연결을 위해 loop마다 초기화를 진행해준다.
  MFRC522 mfrc522(SS_PIN, RST_PIN);
  MFRC522::MIFARE_Key key;
  mfrc522.PCD_Init();
  for (byte i = 0; i < 6; i++)
    key.keyByte[i] = 0xFF;

  // 새로운 RFID가 연결되는지를 확인한다.  
  if(mfrc522.PICC_IsNewCardPresent() and mfrc522.PICC_ReadCardSerial())
    connectNewRFID(mfrc522,key);
  
  // id를 읽는데 실패하면 센서측정을 실시하지 않는다.
  if( id.length()==0 )
    return;

  // 센서 값을 읽고 서버에 보낸다.
  readSensor();
}

void alert()
{
  uint angle = 0;   // 서보모터 각도 제어 위한 변수 선언 및 초기화
  uint reply = 0;   // 서보모터 반복 제어 위한 변수 선언 및 초기화
  
  Serial.println("ALERT!!!");
  
  digitalWrite(BLUEPIN, LOW);
  
  for (reply = 0; reply < 3; reply++)
  {
      digitalWrite(BLUEPIN, LOW);
  for (int blink=0; blink<3; ++blink)
  {
    digitalWrite(REDPIN, HIGH);
    delay(200);
    digitalWrite(REDPIN, LOW);
    delay(200);
  }
    tone(SPEAKERPIN, 500, 400);
    delay(100); 
    for (angle = 90; angle < 135; angle++)
    {
      servo1.write(angle);
      delay(2);
    }
    
    for (angle = 135; angle > 90; angle--)
    {
      servo1.write(angle);
      delay(2);
    }
  }
}

void connectNewRFID(MFRC522 mfrc522, MFRC522::MIFARE_Key key )
{
  byte sector         = 2;
  byte valueBlockA    = 8;
  byte valueBlockB    = 9;
  byte valueBlockC    = 10;
  byte trailerBlock   = 11;
  byte status;

  // Authenticate using key A.
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK) {
    Serial.print("PCD_Authenticate() failed: ");
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
  
  // Authenticate using key B.
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_B, trailerBlock, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK) {
    Serial.print("PCD_Authenticate() failed: ");
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }

  // 새로운 RFID가 등록된 경우 id를 확인해 해당 id로 id를 바꿔준다.
  // id가 설정된 위치의 블록 값은 10이다.(3번째 섹터의 3번째 블록)     
  byte buffer[100];
  byte size = sizeof(buffer);
  status = mfrc522.MIFARE_Read(valueBlockC, buffer, &size);
  String tmpid="";

  for(int i=0; i<ID_SIZE; ++i) {
    uint tmpInt = buffer[i];
    if( tmpInt==255 ) break; // ID의 마지막 확인을 위한 255세팅값 확인
    tmpInt = tmpInt+48;
    if( tmpInt<48 or tmpInt>57 ) break;
    char c = tmpInt;
    tmpid+=c;
  }

  if( tmpid.length() != 0 ) id=tmpid;
}

void readSensor()
{  
  // JSON 포맷 변수 선언 및 할당
  StaticJsonBuffer<50> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();

  // 온습도 센서의 온도를 심박수로 읽어온다.
  int heartRate  = dht.readTemperature();
  int randomNumber = random(0,220);
  heartRate+=randomNumber;

  // 읽어온 센서값과 ID값을 블루투스에 보내준다.
  char _id[ID_SIZE];
  id.toCharArray(_id,ID_SIZE);
  root["id"]   = _id;
  root["data"] = heartRate;
  root.printTo(mySerial);

  // 전송이 완료되면 전송된 ID를 시리얼 모니터를 통해 확인한다.
  Serial.print("CONNECTED ID : ");
  Serial.println(id);

  // 위험 상황이 아니면 파란불이 들어와 있도록 하며, 센서는 3초마다 값을 읽는다.
  digitalWrite(BLUEPIN, HIGH);
  delay(3000);
}


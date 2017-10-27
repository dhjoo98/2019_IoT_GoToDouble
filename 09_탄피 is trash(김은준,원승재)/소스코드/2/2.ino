#include<SoftwareSerial.h>

SoftwareSerial EJSerial(2,3);

int fire=A0;
long cnt=0;//발사 탄 수


char buffer[20];

char index=0;


   
void setup() {
  Serial.begin(9600);
  EJSerial.begin(9600);
  
  pinMode(fire,INPUT);
}

void loop() {
    long val=0;
  while(true){
    if(EJSerial.available())
     {
          val=EJSerial.read();
          Serial.println(val);
          break;
     }
  }
    while(true)
    {
      if(val>0)
      {
       cnt=analogRead(fire);
       Serial.println(cnt);
       delay(500);
       if(cnt>100)
       {
        
        val--;
        EJSerial.write(val);
        
       }
       if(val==0)
       {
        Serial.println("사격완료");
        EJSerial.write(99);
        break;
       }
      }
    }
}

#include <SoftwareSerial.h>

SoftwareSerial BTSerial(2,3); 
int count=0;
void setup()
{
  Serial.begin(9600);
  BTSerial.begin(9600); 
  Serial.println("POWER ON");
}

void loop() {
  int data;
  int data2;
  char NAME[4][20] = {"Lee Do Yeob", "Lee Pyung Ho", "Byon Young Woo", "Park Myung Ho"};
  char GRADE[4][10] = {"-^1", "-^2", "-^3", "-^4"};
  char NUMBER[4][20] = {"16-70002548", "17-70001347", "17-70012349", "18-70001234"};
  

  if(BTSerial.available() > 0)
  {
    data2=BTSerial.read();
    BTSerial.flush();
    if(data2 == 'a')
    count++;
    Serial.print(" ");
    Serial.println(count);
    if(count == 1)
    {
      while(1)
      {
        if(BTSerial.available() > 0)
        {
          data=BTSerial.read();
          BTSerial.flush();
          Serial.print(count);
          if(data == 'a')
          count++;
          if(count != 1)
          break;  
          Serial.println("  GRADE");  
          Serial.print(data);
          BTSerial.write(GRADE[data-1]);
        
        }
              
    }
   }
   if(count == 2)
    {
      while(1)
      {
        if(BTSerial.available() > 0)
        {
          data=BTSerial.read();
          BTSerial.flush();
          Serial.print(count);
          if(data == 'a')
          count++;
          if(count != 2)
          break;
          Serial.println("  NUMBER");  
          Serial.print(data);
          BTSerial.write(NUMBER[data-1]);
        }       
      }
   }
   if(count == 3)
    {
      while(1)
      {
        if(BTSerial.available() > 0)
        {
          data=BTSerial.read();
          BTSerial.flush();
          Serial.print(count);
          if(data == 'a')
          count=0;
          if(count != 3)
          {
          break;
          }
          Serial.println("  NAME");    
          Serial.print(data);
          BTSerial.write(NAME[data-1]);
        
        }
              
    }
   }
}

}



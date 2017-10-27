/*공개SW IOT 과정 프로젝트
 *IOT 센서를 이용한 매복지 경계 시스템 
 *상병 최수영, 일병 신상훈 (팀명: 아는자와 모르는자)
 *IOT based project to prototype the use of sensors for ambush missions in the army
 */

#include<Servo.h>

//서보모터 핀 (Servo Motor Pin)
int trigPin=2; 
int echoPin=3;

//각도, 시간, 거리 (Angle, Time, Distance)
int val;
long duration;
int distance;

//적 감지 LED  (LED that signals detection of enemey)
int ledPin = 5;

//인체감지 센서 작동 버튼 (Button that activates PIR Motion Sensor) 
int buttonPin = 6;
int buttonState = 0;

//가변저항 핀 (Potentionmeter Pin) 
int potpin = 0;

//상황실과 매복지 수신용 LED와 작동버튼 (LED and Button for communication between ambush site and command center) 
int ledGreen = 7;
int ledYellow = 8;
int buttonGreen = 11;
int buttonYellow = 12;
int buttonStateGreen = 0;
int buttonStateYellow = 0;

//초음파샌서 부착된 서보모터 (Servo Motor with Ultrasonic Sensor attached) 
Servo servo;
//인체감지센서 부착된 서보모터 (Servo Motor with PIR Motion Sensor attached) 
Servo motionServo;
  

void setup(){
  pinMode(trigPin, OUTPUT); 
  pinMode(ledPin,OUTPUT);   
  pinMode(echoPin, INPUT); 
  pinMode(buttonPin, INPUT); 
  pinMode(ledGreen, OUTPUT);
  pinMode(ledYellow, OUTPUT);
  Serial.begin(9600);  
  servo.attach(9);
  motionServo.attach(10);
  digitalWrite(buttonPin, HIGH);
  digitalWrite(buttonGreen, HIGH);
  digitalWrite(buttonYellow, HIGH);
}

void loop(){  
  //자동으로 초음파센서 부착된 서보모터 작동해 주변 감시 (Automatically activates the servo motor with the ultrasonic sensor to watch surrounding) 
  for(int i=0;i<=180;i++)  {  
    val=i;
  
    //버튼 누를시 서보모터를 의심지역 감시하고 인체감지센서 작동 (When button is pressed, ultrasonic sensor stops and the PIR Motion sensor is activated) 
    buttonState = digitalRead(buttonPin);
    if (buttonState == LOW){
      manualControl();
    }

    servo.write(i);    
    delay(100);    
    distance=calculateDistance();       

    if (distance <= 12 && distance >= 0) { //거리범위는 지역에 따라 수정해서 응용 (Change the range of the distance based on the geography)
      //초음파센서로 인근에 의심물체 감지시 시리얼 포트에 각도와 거리 프린트하고 LED신호작동 (If the ultrasonic sensor detects something suspicious in radar, turns on the LED)
      Serial.print("Angle: ");
      Serial.print(i);          
      Serial.print(", ");    
      Serial.print("Distance: ");
      Serial.println(distance);    
      digitalWrite(ledPin, HIGH);
    } else {
            digitalWrite(ledPin, LOW);
    }  
  }

  //반대방향으로 동일하게 감시 (Ultrasonic sensor rotating the other direction)
  for(int i=180;i>0;i--)  { 
    
    val=i;
    buttonState = digitalRead(buttonPin);
    if (buttonState == LOW){
      manualControl();
    }  
    
    servo.write(i); 
    delay(100);   
    distance=calculateDistance();       
   
    if (distance <= 12 && distance >= 0) {
      Serial.print("Angle: ");
      Serial.print(i);          
      Serial.print(", ");    
      Serial.print("Distance: ");
      Serial.println(distance);    
      digitalWrite(ledPin, HIGH);
    } 
    else {
      digitalWrite(ledPin, LOW);
    }
  }
 }

//초음파센서 데이터로 거리측정 하는 함수 (Using the data from the ultrasonic sensor to calculate distance) 
int calculateDistance(){ 
  digitalWrite(trigPin,LOW);  
  delayMicroseconds(2); 
  digitalWrite(trigPin,HIGH);   
  duration=pulseIn(echoPin,HIGH);  
  distance = (duration/2) / 29.1;
  return distance;
}  

//인체감지센서 수동 조정 함수 (Function to manually control PIR Motion Sensor)
void manualControl(){

  //가변저항 값을 각도로 계산 (Mapping the value from the potentiometer to angle)
  int val2 = analogRead(potpin);  
  val2 = map(val2, 0, 1023, 0, 179);
  int temp = val2;

  while (true){
    //매복지에서 이상유무 확인후 이상 없다고 신호를 상활실에 보고 (Ambush site signals the command center that there is no problem) 
    buttonStateGreen = digitalRead(buttonGreen);
    if (buttonStateGreen == LOW){
      digitalWrite(ledGreen, HIGH);
      while(true){
        buttonStateYellow = digitalRead(buttonYellow);
        if(buttonStateYellow == LOW){
          digitalWrite(ledYellow, HIGH);
          delay(5000);
          digitalWrite(ledGreen, LOW);
          digitalWrite(ledYellow, LOW);
          break;
        }
      }
    }  
    //가변저항으로 인체감시 센서 조정 (Using potentionmeter to control PIR Motion Sensor attached servo motor)
    val2 = analogRead(potpin);  
    val2 = map(val2, 0, 1023, 0, 179);
    
    if (val2 >= temp){
      val=val+(val2-temp);
      temp = val2;
    }
    else{
      val=val-(temp-val2);
      temp = val2;
    }
     
    motionServo.write(val);    
    delay(100);
    distance=calculateDistance();    

    if (distance <= 12 && distance >= 0) {
      Serial.print("Angle: ");
      Serial.print(val);          
      Serial.print(", ");    
      Serial.print("Distance: ");
      Serial.println(distance);    
      digitalWrite(ledPin, HIGH);
    } 
    else {
      digitalWrite(ledPin, LOW);
    }  

   //상황종료, 초음파센서 다시 자동 작동 (Emergency situation ended, ultrasonic sensor attached servo motor reactivate)
    buttonState = digitalRead(buttonPin);
    if (buttonState == LOW){
      break;     
    }
  }
}




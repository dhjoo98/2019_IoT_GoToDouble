#include <Keypad.h>
#include <LedControl.h>
#include <SoftReset.h>

// 전역변수 선언
// 4x4 Key
const byte ROWS = 4;
const byte COLS = 4;

char hexaKeys[ROWS][COLS] = {
  { '0', '1', '2', '3' },
  { '4', '5', '6', '7' },
  { '8', '9', 'A', 'B' },
  { 'C', 'D', 'E', 'F' },
};

byte rowPins[ROWS] = { 6, 7, 8, 9 };
byte colPins[COLS] = { 5, 4, 3, 2 };

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

//8x8 LED
LedControl lc = LedControl(12, 10, 11, 1);

const uint64_t CHOOSENUMBER[] = {
  0x7e81818100ff0808,
  0x00ff0808ff007e81,
  0xff007e81817e007e,
  0x817e007e81817e00,
  0x81817e008f8989f9,
  0x8f8989f900ff8989,
  0x00ff898900ff700e,
  0x00ff700eff007f80,
  0xff007f8080807f00,
  0x80807f00ff060c06,
  0xff060c06ff00ff89,
  0xff00ff89897600ff,
  0x897600ff898900ff,
  0x898900ff0919e600
};
const int CHOOSENUMBER_LEN = sizeof(CHOOSENUMBER)/8;

const uint64_t SMILE[] = {
  0x0036666060663600
};

const uint64_t DRAWN[] = {
  0x11cac4c0c0c4ca11
};

const uint64_t NUMBERS[] = {
  0x00ff818181ff0000,
  0x008482ff80800000,
  0x00f98989898f0000,
  0x0089898989ff0000,
  0x00784442ff400000,
  0x008f898989f90000,
  0x00ff919191f10000,
  0x000f010101ff0000,
  0x00ff898989ff0000,
  0x008f898989ff0000
};

int image_iterator = 0;

// 게임 관련 변수
int level = 0;
unsigned long start_time;
unsigned long elapsed_time;
int wrong_count = 0;
int correct_count = 0;


// SETUP
void setup() {
  Serial.begin(9600);

  lc.shutdown(0, false); // 절전모드 끄기
  lc.setIntensity(0, 8); // 밝기 설정
  lc.clearDisplay(0);
}

void loop() {
  char customKey = customKeypad.getKey();
  int row, col;
  int playground;
  int i, j;
  int score[2];
  
  // 8x8 LED 판에 매칭 될 8x8 matrix
  int** matrix;
  matrix = (int**) calloc(8, sizeof(int*));
  for(i = 0; i < 8; i++)
    matrix[i] = (int*) calloc(8, sizeof(int));

  // LED 전부 끄기 - 초기화
  lc.clearDisplay(0);

  // 랜덤하게 n개 LED등 켜기
  randomSeed(analogRead(5));      // 노이즈 값을 이용해 seed 랜덤화
  if(level == 0)
    level = choose_number();
  
  lc.clearDisplay(0);
  for(i = 0; i < level; i++) {
    row = random(0, 8);
    col = random(0, 8);

    if(matrix[col][row] == 1) {
      i--;
      continue;  
    }
    else
      matrix[col][row]++;
    
    lc.setLed(0, col, row, true);
  }

  delay(3000);

  playground = random(0, 4);
  while(!check_playground(playground, matrix, 1))           // 사분면에 불이 들어온게 없으면 사용하지 않는다
    playground = random(0, 4);
  
  make_playground(playground);

  // 게임 스타트
  start_time = millis();
  while(true) {
    elapsed_time = millis();
    i = check_playground(playground, matrix, 1);            // 클리어 혹은 실패 했는지 확인
    j = check_playground(playground, matrix, -1);
    if(i == 0 && j == 0) {
      smile_face(); delay(500); lc.clearDisplay(0); delay(500);
      smile_face(); delay(500); lc.clearDisplay(0); delay(500);
      smile_face(); delay(500); lc.clearDisplay(0); delay(500);
      correct_count++;
      break;
    }
    else if(j == -1 || elapsed_time - start_time > 6000) {              // 10초 경과시 실패로 처리
      drawn_face(); delay(500); lc.clearDisplay(0); delay(500);
      drawn_face(); delay(500); lc.clearDisplay(0); delay(500);
      drawn_face(); delay(500); lc.clearDisplay(0); delay(500);
      
      if(++wrong_count == 3) {                        // 99 점까지만 지원한다
        if(correct_count > 100)
          correct_count = 99;
        
        score[0] = correct_count/10;
        score[1] = correct_count%10;

        if(score[0] == 0) {
          displayImage(NUMBERS[score[1]]);
          delay(3000);
        }
        else {
          displayImage(NUMBERS[score[0]]);
          delay(3000);
          displayImage(NUMBERS[score[1]]);
          delay(3000);
        }
        
        soft_restart();
      }
      break;
    }
    
    // 버튼 입력
    customKey = customKeypad.getKey();
    if(customKey)
      btn_click(playground, customKey, matrix);
    
  }

  delay(1000);

  for(i = 0; i < 8; i++)
    free(matrix[i]);
  free(matrix);
}

// 지정된 필드의 불을 전부 끄고, 나머지 필드는 전부 킴
void make_playground(int playground) {
  if(playground == 0) {
    light_0(0);
    light_1(1);
    light_2(1);
    light_3(1);
  }
  else if(playground == 1) {
    light_0(1);
    light_1(0);
    light_2(1);
    light_3(1);
  }
  else if(playground == 2) {
    light_0(1);
    light_1(1);
    light_2(0);
    light_3(1);
  }
  else {
    light_0(1);
    light_1(1);
    light_2(1);
    light_3(0);
  }
}

// light0~3은 각각 4분면, n이 1이면 키고 n이 0이면 끈다
void light_0(int n) {
  if(n == 1)
    for(int col = 0; col < 4; col++)
      for(int row = 0; row < 4; row++)
        lc.setLed(0, col, row, true);
  else
    for(int col = 0; col < 4; col++)
      for(int row = 0; row < 4; row++)
        lc.setLed(0, col, row, false);
}

void light_1(int n) {
  if(n == 1)
    for(int col = 0; col < 4; col++)
      for(int row = 4; row < 8; row++)
        lc.setLed(0, col, row, true);
  else
    for(int col = 0; col < 4; col++)
      for(int row = 4; row < 8; row++)
        lc.setLed(0, col, row, false);
}

void light_2(int n) {
  if(n == 1)
    for(int col = 4; col < 8; col++)
      for(int row = 0; row < 4; row++)
        lc.setLed(0, col, row, true);
  else
    for(int col = 4; col < 8; col++)
      for(int row = 0; row < 4; row++)
        lc.setLed(0, col, row, false);
}

void light_3(int n) {
  if(n == 1)
    for(int col = 4; col < 8; col++)
      for(int row = 4; row < 8; row++)
        lc.setLed(0, col, row, true);
  else
    for(int col = 4; col < 8; col++)
      for(int row = 4; row < 8; row++)
        lc.setLed(0, col, row, false);
}


// n사분면에 finding 이 1또는 -1인 경우를 입력받아 찾는다. 없으면 0을 리턴
int check_playground(int n, int** matrix, int finding) {
  if(finding == 1) {
    if(n == 0)
      for(int col = 0; col < 4; col++)
        for(int row = 0; row < 4; row++)
          if(matrix[col][row] == 1)
            return 1;
   if(n == 1)
     for(int col = 0; col < 4; col++)
       for(int row = 4; row < 8; row++)
         if(matrix[col][row] == 1)
           return 1;
   if(n == 2)
     for(int col = 4; col < 8; col++)
       for(int row = 0; row < 4; row++)
         if(matrix[col][row] == 1)
           return 1;
   if(n == 3)
     for(int col = 4; col < 8; col++)
       for(int row = 4; row < 8; row++)
         if(matrix[col][row] == 1)
           return 1;
  }
  else {
    if(n == 0)
      for(int col = 0; col < 4; col++)
        for(int row = 0; row < 4; row++)
          if(matrix[col][row] == -1)
            return -1;
    if(n == 1)
      for(int col = 0; col < 4; col++)
        for(int row = 4; row < 8; row++)
          if(matrix[col][row] == -1)
            return -1;
    if(n == 2)
      for(int col = 4; col < 8; col++)
        for(int row = 0; row < 4; row++)
          if(matrix[col][row] == -1)
            return -1;
    if(n == 3)
      for(int col = 4; col < 8; col++)
        for(int row = 4; row < 8; row++)
          if(matrix[col][row] == -1)
            return -1;
  }
  
  return 0;
}

// btn클릭 받으면 현재 playground의 해당 위치에 매핑되어 1을 감산한다. matrix의 해당값이 1이면 -2로 강제 초기화시킨다(맞는 버튼 2번 눌렀을 때 실패로 기록되지 않기 위함).
void btn_click(int playground, char customKey, int** matrix) {
  int i = 0;
  int tmp;
  
  if(65 <= customKey && customKey <= 70)
    tmp = (int) customKey - 55;
  else 
    tmp = (int) customKey - 48;
    
  if(playground == 0) {
    for(int col = 0; col < 4; col++) {
      for(int row = 0; row < 4; row++) {
        if(i == tmp) {
          if(matrix[col][row] == 1)
            matrix[col][row] = -2;
          else
            matrix[col][row]--;
          lc.setLed(0, col, row, true);
        }
      i++;
      }
    }
  }
  else if(playground == 1) {
    for(int col = 0; col < 4; col++) {
      for(int row = 4; row < 8; row++) {
        if(i == tmp) {
          if(matrix[col][row] == 1)
            matrix[col][row] = -2;
          else
            matrix[col][row]--;
          lc.setLed(0, col, row, true);
        }
      i++;
      }
    }
  }
  else if(playground == 2) {
    for(int col = 4; col < 8; col++) {
      for(int row = 0; row < 4; row++) {
        if(i == tmp) {
          if(matrix[col][row] == 1)
            matrix[col][row] = -2;
          else
            matrix[col][row]--;
          lc.setLed(0, col, row, true);
        }
      i++;
      }
    }
  }
  else if(playground == 3) {
    for(int col = 4; col < 8; col++) {
      for(int row = 4; row < 8; row++) {
        if(i == tmp) {
          if(matrix[col][row] == 1)
            matrix[col][row] = -2;
          else
            matrix[col][row]--;
          lc.setLed(0, col, row, true);
        }
      i++;
      }
    }
  }
}

// hex배열 이미지 코드 찍어내는 함수
void displayImage(uint64_t image) {
  for(int i = 0; i < 8; i++) {
    byte row = (image >> i * 8) & 0xFF;
    for(int j = 0; j < 8; j++) {
      lc.setLed(0,i, j, bitRead(row, j));
    }
  }
}

void smile_face() {
    lc.clearDisplay(0);
    displayImage(SMILE[0]);
}

void drawn_face() {
  lc.clearDisplay(0);
    displayImage(DRAWN[0]);
}

// CHOOSENUMVER 왼쪽으로 지나가면서 출력, 버튼 입력받으면 끝내고 리턴
int choose_number() {
  char Key;

  while(1) {
    lc.clearDisplay(0);

    displayImage(CHOOSENUMBER[image_iterator]);
    if(++image_iterator >= CHOOSENUMBER_LEN) image_iterator = 0;

    Key = customKeypad.getKey();
    if(Key) {
      if(65 <= Key && Key <= 70)
        return (int) Key - 55 + 1;
      else
        return (int) Key - 48 + 1;
    }
      
    delay(500);
  }
}


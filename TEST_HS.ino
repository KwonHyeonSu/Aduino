#include "RgbLcd.h"
#include "RgbLed.h"
#include "Buzzer.h"
#include "Switch.h"
#include "Humidity.h"
#include "Sound.h"
#include "Piezo.h"
#include "Light.h"
#include "Wire.h"
#include "FND.h"
#include "Temperature.h" 
#include "ThreeAsixGyro.h"

bool isInputed = false;

//1
int pirPin = 39;

//2
int startPushPin=22; 
int endPushPin=37; 

//3
int usPin=75; 

//4
#define RS_PIN 62
#define RW_PIN 63
#define E_PIN 64
#define R_PIN 65
#define G_PIN 66
#define B_PIN 67
#define D_PIN4 45
#define D_PIN5 44
#define D_PIN6 43
#define D_PIN7 42
RgbLcd lcd;

//5
#define RGB_DPIN  81
#define RGB_CPIN  82
RgbLed rgbLed;
byte r=0, g=0, b=0;


//6
#define DC_PIN1 7
#define DC_PIN2 8

//7
#define DIR_PIN 74 
#define STEP_PIN 3
#define MS1_PIN 41
#define MS2_PIN 40 
#define SLP_PIN 38 

//8
#define BUZ_PIN  11
Buzzer buzzer;
Switch pushSW;
int scales[]={256, 288, 320, 341, 384, 427, 480};


//9
#define HUM_PIN 54
Humidity humidity;

//10
#define SOUND_PIN 56
Sound sound;

//a
#define GAS_PIN 55

//b
#define PIEZO_PIN 58
Piezo piezo;

//c
#define LIGHT_PIN 57
Light light;

//d
#define LED_ADDR 36 
#define LED_IOA_REG 6 
#define LED_PORTA_REG 2 

//e
#define FND_ADDR1 38 // 1,2번 FND 주소 값 정의
#define FND_ADDR2 32 // 3,4번 FND 주소 값 정의
FND fnd; 

//f
#define TEMP_ADDR 72 // 온도센서의 주소 값 정의
Temperature temper; 

//g
#define GYRO_ADDR 105 // 자이로 센서의 주소
ThreeAsixGyro gyro;

void setup( ) {
  //1
  pinMode(pirPin, INPUT); // INPUT Mode 설정

  //2
  for(int i=startPushPin; i<=endPushPin; i++) {
    pinMode(i, INPUT);
  }

  //3

  //4
  lcd.begin(RS_PIN, RW_PIN, E_PIN, // Lcd 초기화
  R_PIN, G_PIN, B_PIN, D_PIN4, D_PIN5, D_PIN6, D_PIN7);
  lcd.onBacklightGreen( ); // Green 백라이트 On
  lcd.print("hello, Ardu-Ez!"); // 문자열 출력

  //5
  rgbLed.begin(RGB_DPIN, RGB_CPIN);
  r = 255;

  //6
  pinMode(DC_PIN1, OUTPUT);
  pinMode(DC_PIN2, OUTPUT);

  //7
  pinMode(DIR_PIN, OUTPUT);
  pinMode(STEP_PIN, OUTPUT);
  pinMode(MS1_PIN, OUTPUT);
  pinMode(MS2_PIN, OUTPUT);
  pinMode(SLP_PIN, OUTPUT);
  
  digitalWrite(MS1_PIN, HIGH);
  digitalWrite(MS2_PIN, LOW);

  //8
  buzzer.begin(BUZ_PIN);

  //9
  humidity.begin(HUM_PIN);
  
  //10
  sound.begin(SOUND_PIN);
  
  //a
  pinMode(GAS_PIN, INPUT);
  
  //b
  piezo.begin(PIEZO_PIN);

  //c
  light.begin(LIGHT_PIN);

  //d
  Wire.begin( ); 
 Wire.beginTransmission(LED_ADDR); 
 Wire.write(LED_IOA_REG); 
 Wire.write(byte(0)); 
 Wire.endTransmission( ); 

  //e
  fnd.begin(FND_ADDR1, FND_ADDR2); 

  //f
  temper.begin(TEMP_ADDR); 

  //g
  gyro.begin(GYRO_ADDR);

  
  Serial.begin(9600); // 9600 bps로 Serial 초기화
}



void loop( ) {
  String str = "";
  char ch;

  
  if(!isInputed){  
    Serial.println("\nq : Exit");
    Serial.println("1:pir 센서");
    Serial.println("2:switch button");
    Serial.println("3:초음파 센서");
    Serial.println("4:Text Lcd");
    Serial.println("5:RGB LED");
    Serial.println("6:DC 모터 (검증불가)") ;
    Serial.println("7:Stepping 모터");
    Serial.println("8:BUZZER WITH SWITCH(마지막에)");
    Serial.println("9:온습도센서");
    Serial.println("10:Sound센서");
    Serial.println("a:가스센서");
    Serial.println("b:PIEZO센서");
    Serial.println("c:조도센서");
    Serial.println("d:8개 led센서");
    Serial.println("e:7-segment");
    Serial.println("f:Temperature");
    Serial.println("g:Gyro");
    isInputed = true;
  }
  
  if(Serial.available() > 0){
    str = readSerial();
    if(str == ""){
      Serial.println("아무 것도 입력되지 않음!");
    }

//1pir센서 테스트---------------------------
    else if(str == "1"){
      Serial.println("PIR 센서 테스트 시작");
      while(true){
        int readVal=0; // 입력 값을 저장할 변수 선언 및 초기화
        readVal = digitalRead(pirPin); // 디지털 입력 값을 읽어옴
        Serial.println(readVal); // 읽어 온 값을 Serial 통신으로 송신
        delay(500); // 0.5초(500ms) 동안 대기

        str = readSerial();
        if(str == "q"){
          Serial.println("테스트 종료");
          break;
        }
      }
    }
//2푸시 버튼 스위치 테스트---------------------------------------------
    else if(str == "2"){
      Serial.println("푸시 버튼 스위치 테스트 시작");
      while(true){
        String tmp = "Pushed Number : ";
        for(int i = startPushPin ; i<=endPushPin ; i++){
          if(digitalRead(i) == LOW){
            Serial.println(tmp+i);
          }
        }
        str = readSerial();
        if(str == "q"){
          Serial.println("테스트 종료");
          break;
        }
        
      }
    }
//3초음파센서 테스트--------------------------------------------------------------
    else if(str == "3"){
      Serial.println("초음파 센서 테스트 시작");
      while(true){
         unsigned long duration=0; // Pulse 시간 값을 저장할 변수
         int disMm=0; // 거리를 계산하여 저장할 변수
         pinMode(usPin, OUTPUT); // pulse 보내기 위해 모드 설정
         digitalWrite(usPin, LOW); // LOW로 pulse 초기화
         delayMicroseconds(2); 
         digitalWrite(usPin, HIGH); // HIGH로 pulse 시작
         delayMicroseconds(5); // 최소 5 microseconds 유지
         digitalWrite(usPin, LOW); // LOW로 pulse 마침
         pinMode(usPin, INPUT); // 신호를 받기 위해 입력 모드 설정
         
         // HIGH 값을 줘서 SIG 신호의 Pulse 시간을 읽어옴
         duration = pulseIn(usPin, HIGH); 
         
         // duration 값으로 mm 단위의 거리를 계산
         disMm = (331.5 + 0.6*25) * ((float)duration/1000000/2) * 1000;
         
         Serial.print(disMm); // 거리 값을 Serial로 출력
         Serial.println(" mm");
         delay(1000); // 1초간 대기
         str = readSerial();
          if(str == "q"){
            Serial.println("테스트 종료");
            break;
        }
      }

    }
    
//4Text Lcd 테스트-------------------------------------------
    else if(str == "4"){
      Serial.println("Text Lcd 테스트 시작");
      while(true){
        lcd.setCursor(0,1);
        str = readSerial();
          if(str == "q"){
            Serial.println("테스트 종료");
            break;
        }
      }
    }

//5rgb LED 테스트-------------------------------------------
  else if(str == "5"){
    Serial.println("rgb LED 테스트 시작");
    while(true){
       for(int i=0; i<255; i++)
       {
       rgbLed.OnRgb(r--, g++, b); //- Red 감소/ Green 증가
       }
       for(int i=0; i<255; i++)
       {
       rgbLed.OnRgb(r, g--, b++); //- Green 감소/ Blue 증가
       }
       for(int i=0; i<255; i++)
       {
       rgbLed.OnRgb(r++, g, b--); //- Red 증가/ Blue 증가
       }
      str = readSerial();
      if(str == "q"){
        Serial.println("테스트 종료");
        break;
      }
    }

  }

//6DC 모터 테스트-------------------------------------------
  else if(str == "6"){
    Serial.println("DC 모터 테스트");
    while(true){
      analogWrite(DC_PIN1, 200);
      analogWrite(DC_PIN2, 0);
      delay(3000);
      
      digitalWrite(DC_PIN1, HIGH);
      digitalWrite(DC_PIN2, HIGH);
      delay(3000); 
      
      str = readSerial();
      if(str == "q"){
        Serial.println("테스트 종료");
        break;
      }
    }
  }

//7Stepping 모터 테스트-------------------------------------------
  else if(str == "7"){
    Serial.println("Stepping 모터 테스트");
    while(true){
      int speed = 200; 
      digitalWrite(SLP_PIN, HIGH); 
      digitalWrite(DIR_PIN, HIGH); 
      analogWrite(STEP_PIN, speed); 
      delay(1000); 
      analogWrite(STEP_PIN, 0); // 모터 회전 속도값 0 : 정지
      delay(1000); 
      digitalWrite(DIR_PIN, LOW); // 모터 회전 방향 역방향 설정
      analogWrite(STEP_PIN, speed); // 지정된 속도값으로 회전 시작
      delay(1000);
      analogWrite(STEP_PIN, 0); // 모터 회전 정지
      delay(1000);
      
      str = readSerial();
      if(str == "q"){
        Serial.println("테스트 종료");
        break;
      }
    }
  }

//8BUZZER 테스트-------------------------------------------
  else if(str == "8"){
    Serial.println("BUZZER and Switch 테스트");
    while(true){
      play(pushSW.waitGetPushKey());
    }
  }

//9.온습도 테스트-------------------------------------------
  else if(str == "9"){
    Serial.println("온습도 테스트");
    while(true){
      Serial.print(humidity.read( ));
       Serial.println(" %");
       delay(1000);
      str = readSerial();
      if(str == "q"){
        Serial.println("테스트 종료");
        break;
      }
    }
  }

//10.Sound 테스트-------------------------------------------
  else if(str == "10"){
    Serial.println("Sound 테스트");
    while(true){
      Serial.println(sound.read( ));
       delay(1000);
      str = readSerial();
      if(str == "q"){
        Serial.println("테스트 종료");
        break;
      }
    }
  }
  
  //a.Gas 테스트-------------------------------------------
  else if(str == "a"){
    Serial.println("GAS 테스트");
    while(true){
      int val = analogRead(GAS_PIN);
     Serial.println(val);
     delay(1000);
      str = readSerial();
      if(str == "q"){
        Serial.println("테스트 종료");
        break;
      }
    }
  }
  
  //B.PIEZO 테스트-------------------------------------------
  else if(str == "b"){
    Serial.println("PIEZO 테스트");
    while(true){
      int val = 0;
       piezo.setMode(INPUT); 
       val = piezo.read( ); 
       Serial.println(val); 
       
       if(val > 100) { 
       piezo.setMode(OUTPUT); 
       delay(100); 
       }
      str = readSerial();
      if(str == "q"){
        Serial.println("테스트 종료");
        break;
      }
    }
  }

 //C.조도 테스트-------------------------------------------
  else if(str == "c"){
    Serial.println("조도 테스트");
    while(true){
      String tmp = "";
       tmp += light.read();
       tmp += "Lux";
       Serial.println(tmp);
       delay(1000);
      str = readSerial();
      if(str == "q"){
        Serial.println("테스트 종료");
        break;
      }
    }
  }

  //d.led 센서테스트-------------------------------------------
  else if(str == "d"){
    Serial.println("LED 센서 테스트");
    while(true){
      for(int i=0; i<8; i++) { 
       Wire.beginTransmission(LED_ADDR); 
       Wire.write(LED_PORTA_REG); 
       Wire.write(~(1<<i)); 
       Wire.endTransmission( ); 
       delay(500);
       }
      str = readSerial();
      if(str == "q"){
        Serial.println("테스트 종료");
        break;
      }
    }
  }

  //e.7-seg 센서테스트-------------------------------------------
  else if(str == "e"){
    Serial.println("7-seg fnd 테스트");
    while(true){
      uint8_t i;
 
     fnd.setAllNum(0,0,0,0); // 4개 FND에 0 표시
     
     for(i=0; i<7; i++) {
     fnd.setNum(1, i); // 1번 FND에 숫자 표시
     fnd.setNum(2, i+1); // 2번 FND에 숫자 표시
     fnd.setNum(3, i+2); // 3번 FND에 숫자 표시
     fnd.setNum(4, i+3); // 4번 FND에 숫자 표시
     delay(500);
     }
      str = readSerial();
      if(str == "q"){
        Serial.println("테스트 종료");
        break;
      }
    }
  }

 
  //f.온도감지 센서테스트-------------------------------------------
  else if(str == "f"){
    Serial.println("온도감지센서 테스트");
    while(true){
      Serial.print("C = ");
     Serial.print(temper.getTemperatureC( )); // 섭씨온도 출력
     Serial.print("\t\tF = "); 
     Serial.println(temper.getTemperatureF( )); // 화씨온도 출력
     
     delay(1000); 
      str = readSerial();
      if(str == "q"){
        Serial.println("테스트 종료");
        break;
      }
    }
  }


 //g.자이로 센서테스트-------------------------------------------
  else if(str == "g"){
    Serial.println("자이로센서 테스트");
    while(true){
      Serial.print("x : ");
       Serial.print(gyro.readXValue( )); // X축 값
       Serial.print("\t\ty : ");
       Serial.print(gyro.readYValue( )); // Y축 값
       Serial.print("\t\tz : ");
       Serial.println(gyro.readZValue( )); // Z축 값
      
      delay(1000);
      str = readSerial();
      if(str == "q"){
        Serial.println("테스트 종료");
        break;
      }
    }
  }

   //g.자이로 센서테스트-------------------------------------------
  else if(str == "g"){
    Serial.println("자이로센서 테스트");
    while(true){

      str = readSerial();
      if(str == "q"){
        Serial.println("테스트 종료");
        break;
      }
    }
  }

   //g.자이로 센서테스트-------------------------------------------
  else if(str == "g"){
    Serial.println("자이로센서 테스트");
    while(true){

      str = readSerial();
      if(str == "q"){
        Serial.println("테스트 종료");
        break;
      }
    }
  }

    
    isInputed = false;
  }
  
}

String readSerial()   
{   
  String str = "";   
  char ch;   
  ch = Serial.read();   
  str.concat(ch);   
  delay(10);  
  return str;     
}   

void play(int nKey)
{
 int nLevel = (nKey/9 == 0)? 2 : 4;
 buzzer.setFreq(scales[nKey-1]*nLevel);
 buzzer.On();
 delay(100);
 buzzer.Off();

}

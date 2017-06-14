/****************************************
Example Sound Level Sketch for the 
Adafruit Microphone Amplifier
****************************************/


//#include <ffft.h>
//#include <fix_fft.h>

#include <SoftwareSerial.h>

#define NOISE 5
#define DC_OFFSET 0

#define SECOND 1000

const int sampleWindow = 50; // Sample window width in mS (50 mS = 20Hz)
unsigned int sample;


/* INIT */

char im[128], data[128];
char x = 0, ylim=60;
int i =0, val;
unsigned long  currentTime;

int literSum = 0;
bool waterFlag = false;
float inputMagnitude =0;

/* BLUETOOTH INIT */
SoftwareSerial BTSerial(2, 3); 
byte recvbuffer[1024]; // 데이터를 수신 받을 버퍼
int recvbufferPosition; // 버퍼에 데이타를 저장할 때 기록할 위치; 
byte sendbuffer[1024]; // 데이터를 수신 받을 버퍼
int sendbufferPosition; // 버퍼에 데이타를 저장할 때 기록할 위치





void setup() 
{
  /* 
   *  BLUETOOTH SETUP
   */
  BTSerial.begin(9600);
  recvbufferPosition = 0;  // 버퍼 위치 초기화
  sendbufferPosition = 0;

  /*
   * SOUND BUFFER SETUP
   */
  for( i = 0 ; i < 128; i ++){
    data[i] = 0;
  }

  
  currentTime = millis();
  Serial.begin(9600);
}


void loop() 
{

/*
 * BLUETOOTH FIND
 * receive data information
 * determine the user
 */

  if (BTSerial.available()) 
  { 
    // 수신 받은 데이터 저장
    byte data = BTSerial.read(); 
    
    // 수신된 데이터 시리얼 모니터로 출력
    Serial.write(data); 
    
    // 수신 받은 데이터를 버퍼에 저장
    recvbuffer[recvbufferPosition++] = data; 
    
    // 문자열 종료 표시
    if (data == '\n') { 
      recvbuffer[recvbufferPosition] = '\0';
      
      // 스마트폰으로 문자열 전송
      BTSerial.write(recvbuffer, recvbufferPosition);
      recvbufferPosition = 0;
    }  
  }
  else
  {
    return;
  }
  
  int min = 1024, max = 0;

  /*
   * Sampling sound data window of 128
   */


  /*
   * Sliding Window
   */
  val = analogRead(A0);
  val   = (val - 512 - DC_OFFSET);            // Center on zero
  val   = (val <= NOISE) ? 0 : (val - NOISE);      // Remove noise/hum
  for( i = 0 ; i < 127; i ++){
    data[i] = data[i+1];
    if(val<min) min = data[i];
    if(val>max) max = data[i];
  }
  data[127] = val; 
  if(val<min) min = val;
  if(val>max) max = val;





//




  /*
   * compare the sampled data and the pre-registered data
   * 1.  determine if it is noise or water
   * 2. determine the power of water by extracting the maximum peak value
   */
   
  
  /*
   *  Determine Noise vs Water
   */ 


  int slopeCount = 0;
  for( i = 0 ; i < 126; i++){
    int nextSlope = data[i+2] - data[i+1];
    int prevSlope = data[i+1] - data[i];
    if (prevSlope > 0 && nextSlope <0 && data[i+1] )
      slopeCount ++;
  }
  Serial.println(slopeCount);
  if(slopeCount > 128/4){
    waterFlag = true;
    inputMagnitude = max;
  }
  

  /*
   * Calc Water Flow 
   */
  float maxCalibValue, minCalibValue;
  float maxLiterPerSec, minLiterPerSec;
  float outputLiterPerSec = 0 ;
  float slope, constant;


  slope = (maxLiterPerSec - minLiterPerSec)/(maxCalibValue - minCalibValue);
  constant = maxLiterPerSec - slope * maxCalibValue;

  outputLiterPerSec = inputMagnitude * slope + constant;

  




  /*
   * Timer for one second
   */
   if (waterFlag){
    int count = 0 ;
    if(millis() - currentTime >= SECOND){
      currentTime = millis();
      literSum += outputLiterPerSec;
    }
   }
   




/*
 * BLUETOOTH SEND DATA
 */
  Serial.println("this is a test");
  BTSerial.println("this is a test");
  delay(500); //freq
}


//slopeCount = 0;
//for( i = 0 ; i < 126; i++){
//  nextSlope = data[i+2] - data[i+1];
//  prevSlope = data[i+1] - data[i];
//  if (prevSlope > 0 && nextSlope <0)
//    slopeCount ++;
//}


/****************************************
Example Sound Level Sketch for the 
Adafruit Microphone Amplifier
****************************************/


#include <ffft.h>
#include <fix_fft.h>

//const int sampleWindow = 50; // Sample window width in mS (50 mS = 20Hz)
//unsigned int sample;


/*
  
*/
char im[128], data[128];
char x = 0, ylim=60;
int i =0, val;



void setup() 
{
   Serial.begin(9600);
//   display.begin(SSD1306_SWITCHCAPVCC,0x3C);
//   display.setTextSize(1);
//   display.setTextColor(WHITE);
//   display.clearDisplay();
//   analogReference(DEFAULT);
}


void loop() 
{
  int min = 1024, max = 0;
  for (i =0; i<128; i++){

    val = analogRead(A0);
    data[i] = val ;
    im[i] = 0;
    if(val>max) max=val;
    if(val<min) min =val;
    Serial.println( float(data[i]));     
//    Serial.print("\t");
//
//    Serial.print(i);
    
  }

  fix_fft(data,im,7,0);
  
  for (i = 1 ; i <64;i++){
    int dat = sqrt(data[i]*data[i] +im[i]*im[i]);
    
  }

  






 /*
//   unsigned long startMillis= millis();  // Start of sample window
//   unsigned int peakToPeak = 0;   // peak-to-peak level
//
//   unsigned int signalMax = 0;
//   unsigned int signalMin = 1024;
//
//   // collect data for 50 mS
//   while (millis() - startMillis < sampleWindow)
//   {
//      sample = analogRead(0);
//      if (sample < 1024)  // toss out spurious readings
//      {
//         if (sample > signalMax)
//         {
//            signalMax = sample;  // save just the max levels
//         }
//         else if (sample < signalMin)
//         {
//            signalMin = sample;  // save just the min levels
//         }
//      }
//   }
//   peakToPeak = signalMax - signalMin;  // max - min = peak-peak amplitude
////   double volts = (peakToPeak * 5.0) / 1024;  // convert to volts
//  double volts = peakToPeak;
//   Serial.println(volts);
*/
}

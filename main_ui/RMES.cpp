
#include "RMES.h"
#include <Arduino.h>


#define chB0 22
#define chB1 23
#define chB2 24
#define chB3 25
#define chB4 26
#define chB5 27
#define RESA A0
#define RESB A1
#define RMESPowerEnable 28
#define MuxToggle 29
#define chDelay 30
#define ANAVREF 3.304
#define Rds 0.073
#define Rsource 0.48
#define Vs 3.224

/*
 * float RMESa[20] = {0.020004, 0.020004, 0.020185, 0.019621, 0.02, 0.0199579, 0.0196985, 0.0197669, 0.0196985, 0.0197669, 0.0196985, 0.0204962, 0.0195137, 0.0196985, 0.0199579, 0.0196985, 0.0197669, 0.0196985, 0.0196985, 0.0199579};
float RMESb[20] = { -0.0408516, -0.0360515, -0.0412954, -0.0355793, -0.0409320, -0.0361421, -0.0400148, -0.0357717, -0.0400148, -0.0357717, -0.0398178, -0.0373127, -0.0392256, -0.038045, -0.0401337, -0.036863, -0.0379461, -0.0371, -0.037848, -0.0387366};

 * 
 */


//i need to set these manually based on the channel. This needs to be an array of values for each channel
float RMESa[20] = { 0.0176  , 0.0175, 0.0226, 0.0202, 0.02, 0.0199579, 0.0202, 0.0202, 0.0202, 0.0197669, 0.0202, 0.0204962, 0.0192, 0.0202, 0.0192, 0.0202, 0.0202, 0.0196985, 0.0385, 0.0427};
float RMESb[20] = { -0.0339 , -0.0325, -0.0471, -0.0356, -0.0409320, -0.0361421, -0.0403, -0.0358, -0.0403, -0.0357717, -0.0401, -0.0373127, -0.0375, -0.0407, -0.0373, -0.0397, -0.0383, -0.0391, -0.0669, -0.0911};
float RMESrs = 200.0;
int numCh = 20;




void RMESini(int numOfChannels) {
  pinMode(chB0, OUTPUT);
  pinMode(chB1, OUTPUT);
  pinMode(chB2, OUTPUT);
  pinMode(chB3, OUTPUT);
  pinMode(chB4, OUTPUT);
  pinMode(chB5, OUTPUT);
  pinMode(RMESPowerEnable, OUTPUT);
  pinMode(MuxToggle, OUTPUT);
  analogReadResolution(12);
}
void resetRMES() {
  digitalWrite(RMESPowerEnable, HIGH);
  digitalWrite(MuxToggle, HIGH);
  digitalWrite(chB0, LOW);
  digitalWrite(chB1, LOW);
  digitalWrite(chB2, LOW);
  digitalWrite(chB3, LOW);
  digitalWrite(chB4, LOW);
  digitalWrite(chB5, LOW);
}

int readCh(int chVal) {
  int ADCres = 0;
  int avRes[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  int ch = chVal + 1;
  int mux = 0;
  switch (ch) {
    case 1:
      digitalWrite(MuxToggle, HIGH);
      digitalWrite(chB0, LOW);
      digitalWrite(chB1, LOW);
      digitalWrite(chB2, LOW);
      digitalWrite(chB3, LOW);
      digitalWrite(chB4, LOW);
      digitalWrite(chB5, LOW);
      mux = 1;
      break;
    case 3:
      digitalWrite(MuxToggle, HIGH);
      digitalWrite(chB0, HIGH);
      digitalWrite(chB1, LOW);
      digitalWrite(chB2, LOW);
      digitalWrite(chB3, LOW);
      digitalWrite(chB4, LOW);
      digitalWrite(chB5, LOW);
      mux = 1;
      break;
    case 5:
      digitalWrite(MuxToggle, HIGH);
      digitalWrite(chB0, LOW);
      digitalWrite(chB1, HIGH);
      digitalWrite(chB2, LOW);
      digitalWrite(chB3, LOW);
      digitalWrite(chB4, LOW);
      digitalWrite(chB5, LOW);
      mux = 1;
      break;
    case 7:
      digitalWrite(MuxToggle, HIGH);
      digitalWrite(chB0, HIGH);
      digitalWrite(chB1, HIGH);
      digitalWrite(chB2, LOW);
      digitalWrite(chB3, LOW);
      digitalWrite(chB4, LOW);
      digitalWrite(chB5, LOW);
      mux = 1;
      break;
    case 9:
      digitalWrite(MuxToggle, HIGH);
      digitalWrite(chB0, LOW);
      digitalWrite(chB1, LOW);
      digitalWrite(chB2, HIGH);
      digitalWrite(chB3, LOW);
      digitalWrite(chB4, LOW);
      digitalWrite(chB5, LOW);
      mux = 1;
      break;
    case 11:
      digitalWrite(MuxToggle, HIGH);
      digitalWrite(chB0, HIGH);
      digitalWrite(chB1, LOW);
      digitalWrite(chB2, HIGH);
      digitalWrite(chB3, LOW);
      digitalWrite(chB4, LOW);
      digitalWrite(chB5, LOW);
      mux = 1;
      break;
    case 13:
      digitalWrite(MuxToggle, HIGH);
      digitalWrite(chB0, LOW);
      digitalWrite(chB1, HIGH);
      digitalWrite(chB2, HIGH);
      digitalWrite(chB3, LOW);
      digitalWrite(chB4, LOW);
      digitalWrite(chB5, LOW);
      mux = 1;
      break;
    case 15:
      digitalWrite(MuxToggle, HIGH);
      digitalWrite(chB0, HIGH);
      digitalWrite(chB1, HIGH);
      digitalWrite(chB2, HIGH);
      digitalWrite(chB3, LOW);
      digitalWrite(chB4, LOW);
      digitalWrite(chB5, LOW);
      mux = 1;
      break;
    case 17:
      digitalWrite(MuxToggle, HIGH);
      digitalWrite(chB0, LOW);
      digitalWrite(chB1, LOW);
      digitalWrite(chB2, LOW);
      digitalWrite(chB3, HIGH);
      digitalWrite(chB4, LOW);
      digitalWrite(chB5, LOW);
      mux = 1;
      break;
    case 19:
      digitalWrite(MuxToggle, HIGH);
      digitalWrite(chB0, HIGH);
      digitalWrite(chB1, LOW);
      digitalWrite(chB2, LOW);
      digitalWrite(chB3, HIGH);
      digitalWrite(chB4, LOW);
      digitalWrite(chB5, LOW);
      mux = 1;
      break;
    case 2:
      digitalWrite(MuxToggle, HIGH);
      digitalWrite(chB0, LOW);
      digitalWrite(chB1, HIGH);
      digitalWrite(chB2, LOW);
      digitalWrite(chB3, HIGH);
      digitalWrite(chB4, LOW);
      digitalWrite(chB5, LOW);
      mux = 1;
      break;
    case 4:
      digitalWrite(MuxToggle, HIGH);
      digitalWrite(chB0, HIGH);
      digitalWrite(chB1, HIGH);
      digitalWrite(chB2, LOW);
      digitalWrite(chB3, HIGH);
      digitalWrite(chB4, LOW);
      digitalWrite(chB5, LOW);
      mux = 1;
      break;
    case 6:
      digitalWrite(MuxToggle, HIGH);
      digitalWrite(chB0, LOW);
      digitalWrite(chB1, LOW);
      digitalWrite(chB2, HIGH);
      digitalWrite(chB3, HIGH);
      digitalWrite(chB4, LOW);
      digitalWrite(chB5, LOW);
      mux = 1;
      break;
    case 8:
      digitalWrite(MuxToggle, HIGH);
      digitalWrite(chB0, HIGH);
      digitalWrite(chB1, LOW);
      digitalWrite(chB2, HIGH);
      digitalWrite(chB3, HIGH);
      digitalWrite(chB4, LOW);
      digitalWrite(chB5, LOW);
      mux = 1;
      break;
    case 10:
      digitalWrite(MuxToggle, HIGH);
      digitalWrite(chB0, LOW);
      digitalWrite(chB1, HIGH);
      digitalWrite(chB2, HIGH);
      digitalWrite(chB3, HIGH);
      digitalWrite(chB4, LOW);
      digitalWrite(chB5, LOW);
      mux = 1;
      break;
    case 12:
      digitalWrite(MuxToggle, HIGH);
      digitalWrite(chB0, HIGH);
      digitalWrite(chB1, HIGH);
      digitalWrite(chB2, HIGH);
      digitalWrite(chB3, HIGH);
      digitalWrite(chB4, LOW);
      digitalWrite(chB5, LOW);
      mux = 1;
      break;
    case 14:
      digitalWrite(MuxToggle, LOW);
      digitalWrite(chB0, LOW);
      digitalWrite(chB1, LOW);
      digitalWrite(chB2, LOW);
      digitalWrite(chB3, LOW);
      digitalWrite(chB4, LOW);
      digitalWrite(chB5, LOW);
      mux = 0;
      break;
    case 16:
      digitalWrite(MuxToggle, LOW);
      digitalWrite(chB0, LOW);
      digitalWrite(chB1, LOW);
      digitalWrite(chB2, LOW);
      digitalWrite(chB3, LOW);
      digitalWrite(chB4, HIGH);
      digitalWrite(chB5, LOW);
      mux = 0;
      break;
    case 18:
      digitalWrite(MuxToggle, LOW);
      digitalWrite(chB0, LOW);
      digitalWrite(chB1, LOW);
      digitalWrite(chB2, LOW);
      digitalWrite(chB3, LOW);
      digitalWrite(chB4, LOW);
      digitalWrite(chB5, HIGH);
      mux = 0;
      break;
    case 20:
      digitalWrite(MuxToggle, LOW);
      digitalWrite(chB0, LOW);
      digitalWrite(chB1, LOW);
      digitalWrite(chB2, LOW);
      digitalWrite(chB3, LOW);
      digitalWrite(chB4, HIGH);
      digitalWrite(chB5, HIGH);
      mux = 0;
      break;
  }
  delay(chDelay);
  if (mux == 0) {
    for (int i = 0; i < 20; i++) {
      ADCres = ADCres + analogRead(RESB);
      delay(1);
    }
  }
  else {
    for (int i = 0; i < 20; i++) {
      ADCres = ADCres + analogRead(RESA);
      delay(1);
    }
  }
  ADCres = ADCres / 20;
  return ADCres;
}

void measureRMES(float* resArray, int numCh) {
  int RESdigital[20];
  for (int i = 0; i < numCh; i++) {
    RESdigital[i] = 0;
  }
  resetRMES();

  for (int i = 0; i < numCh; i++) {
    RESdigital[i] = readCh(i);
  }
  for (int i = 0; i < numCh; i++) {
    resArray[i] = VtoR((ANAVREF * ((float)RESdigital[i] / 4095.0)), i); //do i need to typecast RESdigital to preserve the decimals?
  }
  digitalWrite(RMESPowerEnable, LOW);
  delay(10);
}

float VtoR(float voltage, int i) {
  float res = 0;
  res = (RMESrs * (RMESa[i] * voltage + RMESb[i])) / (Vs - RMESa[i] * voltage - RMESb[i]);
  //Serial.print(i + 1);
  //Serial.print(": ");
  //Serial.println(voltage);
  return res;
}

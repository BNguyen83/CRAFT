//to run RMES you need to first run RMESini and pass it the number of channels being used, then when you want to use it, you create an array of floats, you pass this as an argument along with the number of channels to measureRMES and it will populate your array.

#ifndef RMES_HEADER_H
#define RMES_HEADER_H

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
#define chDelay 60
#define ANAVREF 3.304
#define Rds 0.073
#define Rsource 0.48
#define Vs 3.224


//i need to set these manually based on the channel. This needs to be an array of values for each channel
float RMESa[20] = {0.020004,0.020004,0.020185,0.019621,0.02,0.0199579,0.0196985,0.0197669,0.0196985,0.0197669,0.0196985,0.0204962,0.0195137,0.0196985,0.0199579,0.0196985,0.0197669,0.0196985,0.0196985,0.0199579};
float RMESb[20] = {-0.0408516,-0.0360515,-0.0412954,-0.0355793,-0.0409320,-0.0361421,-0.0400148,-0.0357717,-0.0400148,-0.0357717,-0.0398178,-0.0373127,-0.0392256,-0.038045,-0.0401337,-0.036863,-0.0379461,-0.0371,-0.037848,-0.0387366};
float RMESrs = 200.0;
int numCh = 20;
float VtoR(float voltage);
void RMESini(int numOfChannels);
void resetRMES();
int readCh(int chVal);
void measureRMES(float * resArray, int numCh);

#endif

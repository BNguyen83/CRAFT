//to run RMES you need to first run RMESini and pass it the number of channels being used, then when you want to use it, you create an array of floats, you pass this as an argument along with the number of channels to measureRMES and it will populate your array.

#ifndef RMES_H
#define RMES_H


#include <Arduino.h>


float VtoR(float voltage, int i);
void RMESini(int numOfChannels);
void resetRMES();
int readCh(int chVal);
void measureRMES(float * resArray, int numCh);
#endif

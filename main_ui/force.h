#ifndef FORCE_h
#define FORCE_h

#include <Arduino.h>
#include "HX711_ADC.h"
#include "config.h"

void forceSetup();
void insertionForce(int*);
void forceDataReadyISR();
void removalForce();

#endif

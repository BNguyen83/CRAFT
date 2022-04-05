#ifndef CRAFT_MC_H
#define CRAFT_MC_H

#include <Arduino.h>

void setupMC();                         // use this to setup the library code
void setPosition(double input);      // use this to set a new position
void setOrigin();                       // use this to set the origin
void disableMotor(bool state);           // use this to stop the motor
double getPosition();
double getSetPos();
bool isRun();

// plz dont use these
void motorControl(double input);
void posCheck(double input);
void motorControlEncoder();
void motorControlTimerSetup();
void TC8_Handler();
void printer();


#endif

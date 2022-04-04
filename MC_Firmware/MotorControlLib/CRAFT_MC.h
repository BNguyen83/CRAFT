#ifndef CRAFT_MC_H
#define CRAFT_MC_H

#include <Arduino.h>

void setupMC();                         // use this to setup the library code
void setPosition(double newPos);      // use this to set a new position
void setOrigin();                       // use this to set the origin
void disableMotor(int state);           // use this to stop the motor
double getPosition();

#endif

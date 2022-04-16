#include <Arduino.h>
#include "CRAFT_MC.h"
#include "MC.h"

bool motorFlag = false;

void runMotor(uint32_t newPosition){
	setPosition(newPosition);		// set new position
	disableMotor(false);			// Enable motor
	while(isRun() && motorFlag){
		// do nothing else while motor runs
		// if stop motor is false or isRun() returns false then this loop breaks
	}
	disableMotor(true);				// Disable motor
}

void stopMotor(bool mFlag){
	motorFlag = mFlag;
}
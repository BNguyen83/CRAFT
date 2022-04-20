#include <Arduino.h>
#include "CRAFT_MC.h"
#include "MC.h"

bool motorFlag = false;


void runMotor(double newPosition){
	setPosition(newPosition);		// set new position
	disableMotor(false);			// Enable motor
	while(isRun() && motorFlag){
		// do nothing else while motor runs
		// if stop motor is false or isRun() returns false then this loop breaks
	}
	disableMotor(true);				// Disable motor
}

double jogMotor(double offsetPos){
	double currentPosition = getPosition() + offsetPos; // store the current position and add offset
	runMotor(currentPosition); // move to the offset position
	return getPosition();
}

void stopMotor(bool mFlag){
	motorFlag = mFlag;
}

int changeTopSpeed(double speedRatio){
	int errorCode = 0;
	
	if(speedRatio < 0){
		errorCode = 1;
	}
	else if (speedRatio > 1){
		errorCode = 1;
	}
	else{
		changeSpeed(speedRatio);
		errorCode = 0;
	}
	return errorCode;
}
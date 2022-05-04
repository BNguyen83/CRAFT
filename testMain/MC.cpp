#include<Arduino.h>
#include "CRAFT_MC.h"
#include "MC.h"

bool motorFlag = true;


void runMotor(double newPosition){
	setPosition(newPosition);		// set new position
 /*
 //Serial.println(motorFlag);
	disableMotor(false);			// Enable motor
	while(isRun() && motorFlag){
		// do nothing else while motor runs
		// if stop motor is false or isRun() returns false then this loop breaks
    //Serial.println(isRun());
    //Serial.println(digitalRead(34));
    //Serial.println(getPosition());
    //if(digitalRead(34)) motorFlag = false;
    //if(digitalRead(35)) motorFlag = false;
	}
	disableMotor(true);				// Disable motor
  //Serial.println("stopped");
  */
}

double jogMotor(double offsetPos){
	double currentPosition = getPosition() + offsetPos; // store the current position and add offset
	setPosition(currentPosition); // move to the offset position
  disableMotor(false);
  while(isRun());
  disableMotor(true);
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

void menuMotor(double pos){
  setPosition(pos); // move to the offset position
  disableMotor(false);
  while(isRun());
  disableMotor(true);
}

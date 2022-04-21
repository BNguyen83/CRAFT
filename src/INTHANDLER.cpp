#include <Arduino.h>
#include "MC.h"
#include "CRAFT_MC.h"

#define ESTOP 34		// this is for the EStop
#define FAILSAFE 35		// this is the failsafe

int interruptFlag = 0;

void setupInterrupts(){
	attachInterrupt(digitalPinToInterrupt(ESTOP), estop, RISING);
	attachInterrupt(digitalPinToInterrupt(FAILSAFE), failsafe, RISING);
}

void eStop(){
	int flag = 0;
	stopMotor(false);		// this should stop the motor
	if (isRun() == false){
		flag = 1;			// code for estop trigger
	}
}

void failsafe(){
	int flag = 0;
	stopMotor(false);
	if (isRun() == false){
		flag = 2;			// code for failsafe trigger
	}
}

int interruptHandler(){
	return interruptFlag;
}
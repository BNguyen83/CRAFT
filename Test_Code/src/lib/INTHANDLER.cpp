#include <Arduino.h>
#include "MC.h"
#include "CRAFT_MC.h"

#define ESTOP 34		// this is for the EStop
#define FAILSAFE 35		// this is the failsafe

int interruptFlag = 0;


void setupInterrupts(){
	pinMode(ESTOP, INPUT);
	pinMode(FAILSAFE, INPUT);
	attachInterrupt(digitalPinToInterrupt(ESTOP), eStop1, CHANGE);
	attachInterrupt(digitalPinToInterrupt(FAILSAFE), failsafe, RISING);
}

void eStop1(){
	int flag = 0;
	if(digitalRead(ESTOP) == 1){
		stopMotor(false);		// this should stop the motor
		if (isRun() == false){
			flag = 1;			// code for estop trigger
		}
	}
	else if(digitalRead(ESTOP) != 1){
		flag = 0;
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
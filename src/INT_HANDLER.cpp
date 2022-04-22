#include <Arduino.h>
#include "MC.h"
#include "CRAFT_MC.h"

#define ESTOP_ 34		// this is for the EStop
#define FAILSAFE_ 35		// this is the failsafe

int interruptFlag = 0;


void eStop(){
	Serial.println("Tripped!");
  interruptFlag = 1;
  Serial.println(interruptFlag);
 /*
	if(digitalRead(ESTOP_) == 1){
		stopMotor(false);		// this should stop the motor
		if (isRun() == false){
			interruptFlag = 1;			// code for estop trigger
		}
	}
	else if(digitalRead(ESTOP_) != 1){
		interruptFlag = 0;
	}

*/
}

void failsafe(){
	
	//stopMotor(false);
	
	interruptFlag = 2;			// code for failsafe trigger
	
}

int interruptHandler(){
	return interruptFlag;
}

void setupInterrupts(){
  pinMode(ESTOP_, INPUT_PULLUP);
  pinMode(FAILSAFE_, INPUT);
  attachInterrupt(digitalPinToInterrupt(ESTOP_), eStop, RISING);
  attachInterrupt(digitalPinToInterrupt(FAILSAFE_), failsafe, RISING);
  delay(1000);
}

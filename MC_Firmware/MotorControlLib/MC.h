#ifndef MC_H
#define MC_H

#include <Arduino.h>
#include "CRAFT_MC.h"

/*	use run motor to set a new position and begin the motor move
	the function will be stuck in a while loop until the drive train reaches the 
	target position
	*/
void runMotor(double newPosition);

/*
	use stop motor in any situation when you want to stop the drive train. the previous
	function will continue to try to reenable and move but will be immediatly stopped as
	long as the motor flag remains false
*/
void stopMotor(bool mFlag);

/*
	use this to jog the motor a certain distance
	this is incremental movement rather than absolute
*/
double jogMotor(double offsetPos);
/*
	This function changes the top speed of the movment bettween 
	the max speed and the min speed must be between 0 and 1
	return error code 1 = number out of range
	return error code 0 = no error
*/ 
int changeTopSpeed(double speedRatio);
#endif
#ifndef MC_H
#define MC_H

#include <Arduino.h>
#include "CRAFT_MC.h"

/*	use run motor to set a new position and begin the motor move
	the function will be stuck in a while loop until the drive train reaches the 
	target position
	*/
void runMotor(uint32_t newPosition);

/*
	use stop motor in any situation when you want to stop the drive train. the previous
	function will continue to try to reenable and move but will be immediatly stopped as
	long as the motor flag remains false
*/
void stopMotor(bool mFlag);

#endif
#include <FastAccelStepper.h>
#include "AVRStepperPins.h"

#define DISABLE 4
#define STEP 9
#define DIR 3

#define FULL_STEP_SIZE 200      // The number of full steps per revolution of the motor
#define MICRO_STEPS 1           // micro step division setting
#define ACCEL 15000             // set acceleration in steps/s

#define DEFAULT_SPEED 20000

FastAccelStepperEngine engine = FastAccelStepperEngine();
FastAccelStepper *stepper = NULL;

void setup() {
  // put your setup code here, to run once:
  engine.init();
  stepper = engine.stepperConnectToPin(STEP);
  if (stepper) {
    stepper->setDirectionPin(DIR);
    stepper->setEnablePin(DISABLE, false);
    stepper->setAutoEnable(true);     // auto enables and disables the motor driver( to save power on standstill)
    stepper->setAcceleration(ACCEL);
    stepper->setSpeedInHz(DEFAULT_SPEED);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  stepper->moveTo(40000);
  while (stepper->isRunning()) {
    //runEncoder();
  }
  //delay(50);
  stepper->moveTo(0);
  while (stepper->isRunning()) {
    //runEncoder();
  }
  //delay(50);
}

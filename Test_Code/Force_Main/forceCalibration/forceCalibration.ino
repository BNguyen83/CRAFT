#include "HX711ADC.h"
#include "forceCalibration.h"
#include <Arduino.h>
//pins:


void setup() {
  forceCalibrationSetup();
  }

void loop() {
  forceCalibration();
}

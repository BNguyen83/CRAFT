#include <Arduino.h>
#include "HX711ADC.h"

const int LOADCELL_DOUT_PIN = 30;
const int LOADCELL_SCK_PIN = 31;

double forceOffset = 871;
bool firstPeak = 0;
double insertionForce = 0;
double removalForce = 0;

HX711 scale;

void resetForceFlag() {
  firstPeak = 1;
}

void forceCalibration() {
  scale.set_scale(forceOffset);
  scale.tare();
}

void forceSetup() {
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_gain(128); // Set gain
  forceCalibration();
}

double measureInsertion() {
  double i = scale.get_units(4);
  firstPeak = 1;
  if (firstPeak){
  if (i >= insertionForce ) {
    insertionForce = i;
  }
  else if (insertionForce - i > 100) {
    insertionForce = insertionForce;
    firstPeak = 0;
  }
  }
  return (insertionForce);
}

double measureRemoval() {
  double i = scale.get_units(2);
  //if (scale.wait_ready_retry(10)) {
    // INSERT SCREEN PRINT HERE???????-------------------------
  //}
  if (i >= removalForce ) {
    removalForce = i;
  }
  return (removalForce);
}

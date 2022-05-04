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
  static boolean newDataReady = 0;
  if (scale.read()) newDataReady = true;
  if (firstPeak && newDataReady) {
    double i = scale.get_units(4);
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
  static boolean newDataReady = 0;
  if (scale.read()) newDataReady = true;
  if (firstPeak && newDataReady) {
    double i = scale.get_units(2);
    if (i >= removalForce ) {
      removalForce = i;
    }
    return (removalForce);
  }
}

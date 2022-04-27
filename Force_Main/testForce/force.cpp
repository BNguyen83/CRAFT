#include <Arduino.h>
#include "HX711ADC.h"
#include "force.h"
#include "config.h"

const int HX711_dout = 2; //mcu > HX711 dout pin
const int HX711_sck = 3; //mcu > HX711 sck pin
float calibrationValue = 88.47; // set the calibration value in the sketch

HX711_ADC LoadCell(HX711_dout, HX711_sck);

unsigned long t = 0;
// volatile boolean newDataReady; ---- uncomment this when using interrupt
int overload = 10000;

void forceSetup() {
  Serial.begin(57600);
  delay(10);

  LoadCell.begin();
  unsigned long stabilizingtime = 2000; // preciscion right after power-up can be improved by adding a few seconds of stabilizing time
  boolean _tare = true; //set this to false if you don't want tare to be performed in the next step
  LoadCell.start(stabilizingtime, _tare); //if start fails, check wiring and pin destination
  LoadCell.setCalFactor(calibrationValue); // set calibration value (float)
  Serial.println("Setup is Ready");
}

//void forceDataReadyISR() { //interrupt routine
//  if (LoadCell.update()) {
//    newDataReady = 1;
//  }
//}

void measureInsertion(int* maxForce) {
  static boolean newDataReady = 0;
  const int settlingTime = 0;
  if (LoadCell.update()) newDataReady = 1;
  // get smoothed value from the dataset:
  if (newDataReady) {
    if (millis() > t + settlingTime) {
      float i = LoadCell.getData();
      if (i >= *maxForce) {
        *maxForce = i;
        newDataReady = 0;
        t = millis();
      }
      else if (i < *maxForce || *maxForce > overload) {
        LoadCell.powerDown();
      }
    }
  }
}

void measureRemoval(int* minForce) {
  static boolean newDataReady = 0;
  const int settlingTime = 0;
  if (LoadCell.update()) newDataReady = 1;
  // get smoothed value from the dataset:
  if (newDataReady) {
    if (millis() > t + settlingTime) {
      float i = LoadCell.getData();
      if (i > *minForce ) {
              *minForce = i;
        newDataReady = 0;
        t = millis();
      }
    }
  }
}

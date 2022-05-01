#include <Arduino.h>
#include "HX711ADC.h"
#include "force.h"
#include "config.h"
#include "MC.h"

// volatile boolean newDataReady; ---- uncomment this when using interrupt
float calibrationValue = 88.47; // set the calibration value in the sketch
const int HX711_dout = 30; //mcu > HX711 dout pin
const int HX711_sck = 31; //mcu > HX711 sck pin

HX711_ADC LoadCell(HX711_dout, HX711_sck);

int overload = 20000;
boolean firstPeak = 0;
unsigned long t = 0;

void forceSetup() {
  Serial.begin(9600);
  delay(10);

  LoadCell.begin();
  unsigned long stabilizingtime = 2000; // preciscion right after power-up can be improved by adding a few seconds of stabilizing time
  boolean _tare = true; //set this to false if you don't want tare to be performed in the next step
  LoadCell.start(stabilizingtime, _tare); //if start fails, check wiring and pin destination
  LoadCell.setCalFactor(calibrationValue); // set calibration value (float)
  Serial.println("Setup is Ready");
}

void resetForceFlag() {
  firstPeak = 0;
}

void measureInsertion(int* maxForce) {
  static boolean newDataReady = 0;
  const int settlingTime = 0;
  if (LoadCell.update()) newDataReady = true;
  // get smoothed value from the dataset:
  if (newDataReady) {
    float n = LoadCell.getData();
    if (n >= *maxForce) {
      *maxForce = n;
      newDataReady = 0;

    }
  }
}

void measureRemoval(int* minForce) {
  static boolean newDataReady = 0;
  const int settlingTime = 300;
  if (LoadCell.update()) newDataReady = 1;
  // get smoothed value from the dataset:
  if (newDataReady) {
    if (millis() > t > settlingTime) {
      float n = LoadCell.getData();
      if (n > *minForce ) {
        *minForce = n;
        newDataReady = 0;
      }
    }
  }
}

void forceCalibrationSetup() {
  Serial.begin(9600); delay(10);
  Serial.println();
  Serial.println("Starting...");

  LoadCell.begin();
  //LoadCell.setReverseOutput(); //uncomment to turn a negative output value to positive
  unsigned long stabilizingtime = 2000; // preciscion right after power-up can be improved by adding a few seconds of stabilizing time
  boolean _tare = true; //set this to false if you don't want tare to be performed in the next step
  LoadCell.start(stabilizingtime, _tare);
  if (LoadCell.getTareTimeoutFlag() || LoadCell.getSignalTimeoutFlag()) {
    Serial.println("Timeout, check MCU>HX711 wiring and pin designations");
    while (1);
  }
  else {
    LoadCell.setCalFactor(1.0); // user set calibration value (float), initial value 1.0 may be used for this sketch
    Serial.println("Startup is complete");
  }
  while (!LoadCell.update());
  forceCalibration(); //start calibration procedure
}

void forceCalibration() {
  Serial.println("***");
  Serial.println("Start calibration:");
  Serial.println("Place the load cell an a level stable surface.");
  Serial.println("Remove any load applied to the load cell.");
  Serial.println("Send 't' from serial monitor to set the tare offset.");

  boolean _resume = false;
  while (_resume == false) {
    LoadCell.update();
    if (Serial.available() > 0) {
      if (Serial.available() > 0) {
        char inByte = Serial.read();
        if (inByte == 't') LoadCell.tareNoDelay();
      }
    }
    if (LoadCell.getTareStatus() == true) {
      Serial.println("Tare complete");
      _resume = true;
    }
  }

  Serial.println("Now, place your known mass on the loadcell.");
  Serial.println("Then send the weight of this mass (i.e. 100.0) from serial monitor.");

  float known_mass = 0;
  _resume = false;
  while (_resume == false) {
    LoadCell.update();
    if (Serial.available() > 0) {
      known_mass = Serial.parseFloat();
      if (known_mass != 0) {
        Serial.print("Known mass is: ");
        Serial.println(known_mass);
        _resume = true;
      }
    }
  }

  LoadCell.refreshDataSet(); //refresh the dataset to be sure that the known mass is measured correct
  float newCalibrationValue = LoadCell.getNewCalibration(known_mass); //get the new calibration value

  Serial.print("New calibration value has been set to: ");
  Serial.print(newCalibrationValue);
  Serial.println(", use this as calibration value (calFactor) in your project sketch.");

  _resume = true;

  Serial.println("End calibration");
  Serial.println("***");
  Serial.println("To re-calibrate, send 'r' from serial monitor.");
  Serial.println("For manual edit of the calibration value, send 'c' from serial monitor.");
  Serial.println("***");
  Serial.println("End change calibration value");
  Serial.println("***");
  // receive command from serial terminal
  if (Serial.available() > 0) {
    char inByte = Serial.read();
    if (inByte == 't') {
      Serial.println("exit");
      Serial.println("exit");
    }
  }
}

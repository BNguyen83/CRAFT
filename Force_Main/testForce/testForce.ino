#include <Arduino.h>
#include "force.h"
#include "config.h"
#include "HX711ADC.h"

void setup() {
  forceSetup();
}

int maxForce = 0;
int ignoredForce = 0;
int minForce = 0;

void loop() {
  measureInsertion(&maxForce, &ignoredForce);
  Serial.print("Insertion Force (kg): ");
  Serial.println(maxForce);

  measureRemoval(&minForce);
}
 

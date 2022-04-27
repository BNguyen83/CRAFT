#include <Arduino.h>
#include "force.h"
#include "config.h"
#include "HX711ADC.h"

void setup() {
  forceSetup();
}

int maxForce = 0;
int minForce = 0;

void loop() {
  measureInsertion(&maxForce);
  Serial.print("Insertion Force (g): ");
  Serial.println(maxForce);
  measureRemoval(&minForce);
  Serial.print("Removal Force (g): ");
  Serial.println(minForce);
}
 

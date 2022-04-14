#include "RMES.h"
#include "CRAFT_MC.h"

 int numCh1 = 20;


void setup() {
  // put your setup code here, to run once:
 
  RMESini(numCh1);
  setupMC(8000, 4000, 0.8, 0.02);
  changeGain(100, 0);
  delay(250);
  //pinMode(35, INPUT);
  //attachInterrupt(digitalPinToInterrupt(35), failsafe, RISING);
  setPosition(0);
  Serial.begin(230400);
}

void loop() {
  // put your main code here, to run repeatedly:
  
  
  runMotorTest();
  //Serial.println(isRun());
  disableMotor(true);
  Serial.println("Move Done!");


  setPosition(0);
  Serial.println("New position Set");
  Serial.println(getSetPos());
  Serial.println(getPosition());

  
  delay(10);
  disableMotor(false);
  Serial.println(isRun());

  runMotorTest();
  //Serial.println(isRun());
  disableMotor(true);
  Serial.println("Move Done!");
  runRes();
  delay(2000);

  setPosition(-10);
  Serial.println("New position Set");
  Serial.println(getSetPos());
  Serial.println(getPosition());

  
  disableMotor(false);
  Serial.println(isRun());
 
  delay(10);

  disableMotor(false);
  Serial.println(isRun());
}

void runRes(){
  float res[20];

  measureRMES(res, numCh1);

  for (int i = 0; i < numCh1; i++) {
    Serial.print("Resistance in Channel ");
    Serial.print(i + 1);
    Serial.print("\t");
    Serial.println(res[i], 3);
  }
}

void runMotorTest() {
  while (isRun()) {

    /*
      if(Serial.available() > 0) {

      disableMotor(Serial.find('k'));

      }
    */

    Serial.println(getPosition());
  }
  Serial.println("done move");
}

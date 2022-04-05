#include <CRAFT_MC.h>


void setup() {
  // put your setup code here, to run once:
  setupMC();
  setPosition(0);
  SerialUSB.begin(230400);
}

void loop() {
  // put your main code here, to run repeatedly:
  
  
  runMotorTest();
  SerialUSB.println(isRun());
  disableMotor(true);
  SerialUSB.println("Move Done!");
  
  setPosition(0);
  SerialUSB.println("New position Set");
  SerialUSB.println(getSetPos());
  SerialUSB.println(getPosition());
  
  delay(2000);
  disableMotor(false);
  SerialUSB.println(isRun());
  
  runMotorTest();
  SerialUSB.println(isRun());
  disableMotor(true);
  SerialUSB.println("Move Done!");
  
  setPosition(20);
  SerialUSB.println("New position Set");
  SerialUSB.println(getSetPos());
  SerialUSB.println(getPosition());
  
  delay(2000);
  disableMotor(false);
  SerialUSB.println(isRun());
}
void runMotorTest(){
  while(isRun()){
    
    /*
    if(Serial.available() > 0) {
    
    disableMotor(Serial.find('k'));
    
    }
    */
    
   //SerialUSB.println(getPosition());
  }
}

#include <CRAFT_MC.h>

int obstFlag = 1; // obstruction spike flag. reset before next move
void setup() {
  // put your setup code here, to run once:
  setupMC(8000, 4000, 0.8, 0.02);
  changeGain(100, 0);
  delay(250);
  pinMode(35, INPUT);
  attachInterrupt(digitalPinToInterrupt(35), failsafe, RISING);
  setPosition(0);
  Serial.begin(230400);
}

void loop() {
  // put your main code here, to run repeatedly:
  
  
  motorLoop();
}
void motorLoop(){
  while(obstFlag){
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
  
  
  setPosition(-10);
  Serial.println("New position Set");
  Serial.println(getSetPos());
  Serial.println(getPosition());
  
  delay(1000);
  disableMotor(false);
  Serial.println(isRun());
  }
}
void runMotorTest(){
  while(isRun() && obstFlag){
    
    /*
    if(Serial.available() > 0) {
    
    disableMotor(Serial.find('k'));
    
    }
    */
    
   Serial.println(getPosition());
  }
  Serial.println("test");
}

void failsafe(){
  obstFlag = 0;
  //disableMotor(true);
  Serial.println("Motor Disabled");
  //delay(5000);
  //disableMotor(false);
  //Serial.println("Motor Enabled");
  //Serial.println("actual obstruction");
  
 
}

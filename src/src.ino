// this is a test file
#include "MC.h"
#include "CRAFT_MC.h"
#include "INT_HANDLER.h"
int flag = 0;
void setup() {
  // put your setup code here, to run once:
  setupMC(8000, 4000, 0.8, 0.02);
  setupInterrupts();
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  
  //Serial.println(" move");
  //runMotor(0);
  //runMotor(-10);
  
  //flag = 0;
  checkFlag();
  delay(1000);
}

void checkFlag(){
  flag = interruptHandler();
  //Serial.print("flag: ");
  //Serial.println(flag);
  switch (flag){
    case 0: break;
    case 1: Serial.println("Emergency Stop Active"); break;
    case 2: Serial.println("Obstruction Detected"); break;
  }
}

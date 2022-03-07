/*
 * Test code for motor control
 * this code mimics the input and output of the motor driver and linear encoder
 * this code is setup for use with an Arduino MEGA 2560 
 * but it should work as is on a due or any other arduino really
 */


#define STEP 2
#define DIR 3
#define ENC_A 11
#define ENC_B 12

int state = 0;
int count0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(STEP, INPUT_PULLUP);
  pinMode(DIR, INPUT_PULLUP);
  pinMode(ENC_A, OUTPUT);
  pinMode(ENC_B, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  counter();
}

void encoder(){
    switch (state){
      case 0: digitalWrite(ENC_A, LOW); digitalWrite(ENC_B, LOW); break;
      case 1: digitalWrite(ENC_A, HIGH); digitalWrite(ENC_B, LOW); break;
      case 2: digitalWrite(ENC_A, HIGH); digitalWrite(ENC_B, HIGH); break;
      case 3: digitalWrite(ENC_A, LOW); digitalWrite(ENC_B, HIGH); break;
    }
  }

void changeState(){
  if (digitalRead(STEP) == 0){
    if (digitalRead(DIR) == 1){
      state++;
      state %= 4;
    }
    else {
      state--;
      if (state < 0) state = 3;
      state %= 4;
    }
  }
  encoder();
  //Serial.println(count0);
  delayMicroseconds(10);
}

void counter(){
  if(count0 > 787){
    changeState();
    count0 = 0;
  }
  else count0++;
}




  

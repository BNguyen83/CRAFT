#include <Arduino.h>
#include "CRAFT_MC.h"


#define MCK 84000000


#define DIR 32                          // Direction pin
#define EN 33                           // enable pin
//#define STEP 2                          // step pin
#define FSAFE 34                        // failsafe flag pin
#define ESTOP 35                        // estop flag pin
#define ENC_A 36                        // Encoder pin A
#define ENC_B 37                        // Encoder pin B
#define ENC_RI 38                       // Encoder pin RI


double MAXACCEL = 0.2;

double MAXSPEED = 100;                 // arbitrary max speed value 50 is about as fast as it can go in no load
double MINSPEED = 500;                 
double Ts = 0.00005;
double tolerance = 0.1;
double speedRatio = 0;
double currentPos = 0;                  // current position in mm
double lastPos = 0;
double newPos = 0;
double currentSpeed = 0;
double integ1[] = {0, 0};                // current value and previous value

bool isRunning = true;
bool killMotor = false;

int BIGNUM = 42000000;
int SMALLNUM = 41998600;

int encoderState[] = {0, 0};             // current state, previous state
int count1 = 0;                          // internal counter for encoder decoder

// control loop variables
double e1 = 0;
double e2 = 0;
double y = 0;

// Gain MATRIX 0: error1 |1: error2 |2: speed feedback
double gainM[] = {45, 1, 1, 1, 1};

void setupMC(double MAXS, double MINS, double ACCEL, double tol) {
  // setup ISR register
  pinMode(12, OUTPUT);
  //pinMode(STEP, OUTPUT);

  pinMode(DIR, OUTPUT);
  pinMode(EN, OUTPUT);
  pinMode(ENC_A, INPUT);
  pinMode(ENC_B, INPUT);
  pinMode(ENC_RI, INPUT);

  attachInterrupt(digitalPinToInterrupt(ENC_A), motorControlEncoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENC_B), motorControlEncoder, CHANGE);
  digitalWrite(EN, LOW);
  //Serial.begin(230400);

	MAXSPEED = 500000 / MAXS;
	MINSPEED = 500000 / MINS;
	MAXACCEL = ACCEL;
	tolerance = tol;
  motorControlTimerSetup();

  newPos = 0;

}

void setPosition(double input) {
  // use this to set a new position
  newPos = input;
}

double getPosition(){
  return currentPos;
}

double getSetPos() {
    return newPos;
}

void setOrigin() {
  currentPos = 0;                       // use this to set the origin
}

void disableMotor(bool state){
  // use this to stop the motor
    killMotor = state;
    if (state) {
        isRunning = false;
        TC2->TC_CHANNEL[2].TC_CCR = TC_CCR_CLKDIS;
    }
    else {
        isRunning = true;
        TC2->TC_CHANNEL[2].TC_CCR = TC_CCR_SWTRG | TC_CCR_CLKEN;
    }
  
}

bool isRun() {
    return isRunning;
}

void changeGain(double input, int index){
	gainM[index] = input;
}

void changeSpeed(double speedratio){
	double currentTopSpeed = 50000 / MAXSPEED;	// convert speed to Hz
	double currentBotSpeed = 50000 / MINSPEED;
	
	double newMaxSpeed = speedratio * (currentTopSpeed - currentBotSpeed) + currentBotSpeed;
	MAXSPEED = 50000 / newMaxSpeed; // convert back to ArDuiNO sPEak
}
void motorControl (double input) {
  // do entire control loop in here
  posCheck(input);
  // input in mm minus current pos in mm
  e1 = gainM[0] * (input - currentPos);
  e2 = e1 - (gainM[1] * currentSpeed);

  // set direction
  if (e1 < 0) digitalWrite(DIR, HIGH);      // throw it reverse
  else if (e1 > 0) digitalWrite(DIR, LOW);  // go forward
  // set acceleration
  if (e2 > MAXACCEL) e2 = MAXACCEL;
  else if (e2 < -MAXACCEL) e2 = -MAXACCEL;

  integ1[0] = e2;
  // descrete time integrator 1
  y = integ1[0] + integ1[1];


  // set speed
  if (y > MAXSPEED) y = MAXSPEED;
  else if (y < -MAXSPEED) y = -MAXSPEED;
  integ1[1] = y;
  //speedRatio = 1 - abs(0.99 * (MAXSPEED / y));
  speedRatio = y / MAXSPEED;

  // set new speed
  REG_PWM_CPRDUPD1 = MINSPEED - (MINSPEED-MAXSPEED)*abs(speedRatio);
  REG_PWM_CDTYUPD1 = (MINSPEED -(MINSPEED-MAXSPEED)*abs(speedRatio))/50;
}
/*
void motorControlStep (uint32_t spd, uint32_t acel, uint8_t dire) {
  // calculate speed and set PWM
  // set driver outputs
}
*/
void posCheck(double input){
   
          
    if (abs(input - currentPos) < tolerance) {

            digitalWrite(EN, LOW);    // disable output
            isRunning = false;
            
    }
    else {
        if (killMotor) {
            digitalWrite(EN, LOW);    // disable output
            isRunning = false;
            
        }
        else {
            digitalWrite(EN, HIGH);
            isRunning = true;
            
        }
    }
   
}

void motorControlEncoder () {
  int A = 0;  // Encoder A input
  int B = 0;  // Encoder B input
  int dirFactor = 0;  // number used in incrementing position counter

  // do encoder data capture
  A = digitalRead(ENC_A);
  B = digitalRead(ENC_B);
  //Serial.print(A); Serial.println(B);
  // deturmine current state
  if (A == 0) {
    switch (B) {
      case (0): encoderState[0] = 0; break;
      case (1): encoderState[0] = 1; break;
    }
  }
  else if (A > 0) {
    switch (B) {
      case (1): encoderState[0] = 2; break;
      case (0): encoderState[0] = 3; break;
    }
  }


  // test is input is good
  if (encoderState[0] == 0) {
    switch (encoderState[1]) {
      case (0): dirFactor = 0; encoderState[1] = encoderState[0]; break;  // no change in position
      case (1): dirFactor = 1; encoderState[1] = encoderState[0]; count1++; break;  // forward movoment
      case (2): dirFactor = 0; encoderState[1] = encoderState[1]; break;  // input error
      case (3): dirFactor = -1; encoderState[1] = encoderState[0]; count1--; break;  // reverse movment
    }
  }
  else if (encoderState[0] == 1) {
    switch (encoderState[1]) {
      case (1): dirFactor = 0; encoderState[1] = encoderState[0]; break;  // no change in position
      case (2): dirFactor = 1; encoderState[1] = encoderState[0]; count1++; break;  // forward movoment
      case (3): dirFactor = 0; encoderState[1] = encoderState[1]; break;  // input error
      case (0): dirFactor = -1; encoderState[1] = encoderState[0]; count1--; break;  // reverse movment
    }
  }
  else if (encoderState[0] == 2) {
    switch (encoderState[1]) {
      case (2): dirFactor = 0; encoderState[1] = encoderState[0]; break;  // no change in position
      case (3): dirFactor = 1; encoderState[1] = encoderState[0]; count1++; break;  // forward movoment
      case (0): dirFactor = 0; encoderState[1] = encoderState[1]; break;  // input error
      case (1): dirFactor = -1; encoderState[1] = encoderState[0]; count1--; break;  // reverse movment
    }
  }
  else if (encoderState[0] == 3) {
    switch (encoderState[1]) {
      case (3): dirFactor = 0; encoderState[1] = encoderState[0]; break;  // no change in position
      case (0): dirFactor = 1; encoderState[1] = encoderState[0]; count1++; break;  // forward movoment
      case (1): dirFactor = 0; encoderState[1] = encoderState[1]; break;  // input error
      case (2): dirFactor = -1; encoderState[1] = encoderState[0]; count1--; break;  // reverse movment
    }
  }

  if (count1 <= -1) {
    currentPos = currentPos + dirFactor * 0.02; // subtract 20um
    count1 = 3;
    //printer();
  }
  else if (count1 >= 4) {
    currentPos = currentPos + dirFactor * 0.02; // add 20um
    count1 = 0;
    //printer();
  }

  //currentPos = currentPos + dirFactor * 0.005; // add or sub 5um
  // return state variable in mm and mm/s
  // also maintain acuracy of measurment here using datum marks
  //return currentState;
}

void motorControlTimerSetup () {
  // TC8 is the timer for sensor sampling
  //-----------------------------------------------------------------
  // turn on timer/counter 2 channel 2
  PMC->PMC_PCER1 |= PMC_PCER1_PID35;
  // set the base clock to MCK/8
  TC2->TC_CHANNEL[2].TC_CMR = TC_CMR_TCCLKS_TIMER_CLOCK2 | TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC;

  TC2->TC_CHANNEL[2].TC_RC = 10500; //525;   // set timer to 20kHz or 1kHz
  TC2->TC_CHANNEL[2].TC_IER = TC_IER_CPCS; // setup interrupt
  NVIC_EnableIRQ(TC8_IRQn);         // software trigger
  TC2->TC_CHANNEL[2].TC_CCR = TC_CCR_SWTRG | TC_CCR_CLKEN; // Enable timer
  //------------------------------------------------------------------

  //PB17 or pin A9
  PIOB->PIO_PDR |= 1<<17;    // disable pio and enable periferals
  PIOB->PIO_ABSR |= 1<<17;   // set pin B17 to perf B which is PWML1
  
  PMC->PMC_PCER1 |= PMC_PCER1_PID36;    // turn on PWM controller

  REG_PWM_CLK = PWM_CLK_PREA(0) | PWM_CLK_DIVA(84);   // set PWM clock to 1MHz
  REG_PWM_CMR1 = PWM_CMR_CALG | PWM_CMR_CPRE_CLKA;      // set controller to use clock A
  REG_PWM_CPRD1 = MAXSPEED;     // set speed to 20kHz
  REG_PWM_CDTY1 = MAXSPEED/2;   // set 50% dutycycle
  REG_PWM_ENA = PWM_ENA_CHID1;       // enable PWM Channel 1

}

// Interupts

void TC8_Handler() {
  TC2->TC_CHANNEL[2].TC_SR;
  //Serial.println('loop');
  // get current speed
  motorControl(newPos);
  currentSpeed = gainM[2] * (MAXSPEED * speedRatio);
  
}


/* test functions
void printer() {
  if (currentPos != lastPos) {
    Serial.println(currentPos);
    //Serial.print("  |  ");
    Serial.print(newPos-currentPos);
    Serial.print("  |  ");
    //Serial.print(e2);
    //Serial.print("  ::  ");
    //Serial.println(100 * speedRatio);
  }
  lastPos = currentPos;
}
*/


#define MAXACCEL 100

#define DIR 32                          // Direction pin
#define EN 34                           // enable pin
#define STEP 2                          // step pin
#define FSAFE 34                        // failsafe flag pin
#define ESTOP 35                        // estop flag pin
#define ENC_A 36                        // Encoder pin A
#define ENC_B 37                        // Encoder pin B
#define ENC_RI 38                       // Encoder pin RI
double MAXSPEED = 2400;                 // arbitrary max speed value
double Ts = 0.00005;
double speedRatio = 0;
double currentPos = 0;                  // current position in mm
double lastPos = 0;
double newPos = 0;
double currentSpeed = 0;
double integ1[] = {0, 0};                // current value and previous value

int encoderState[] = {0, 0};             // current state, previous state
int count1 = 0;                          // internal counter for encoder decoder

// control loop variables
double e1 = 0;
double e2 = 0;
double y = 0;

// Gain MATRIX 0: error1 |1: error2 |2: speed feedback
double gainM[] = {1, 1, 1, 1, 1};

void setup() {
  // put your setup code here, to run once:
  // setup ISR register
  pinMode(12, OUTPUT);
  pinMode(STEP, OUTPUT);

  pinMode(DIR, OUTPUT);
  pinMode(EN, OUTPUT);
  pinMode(ENC_A, INPUT);
  pinMode(ENC_B, INPUT);
  pinMode(ENC_RI, INPUT);

  Serial.begin(230400);

  motorControlTimerSetup();

  newPos = -20;

}

void loop() {
  printer();
  // setup timer for 100k sample rate: 10us. use Timer Counter(TC)

  // encoder code

  // unit convertions

  // MAX Acceleration and Velocity for motor

  // gain values for feedback loop in a matrix
  // PWM to drive motor
}

void motorControl (double input) {
  // do entire control loop in here

  // input in mm minus current pos in mm
  e1 = gainM[0] * (input - currentPos);
  e2 = e1 - (gainM[1] * currentSpeed);

  // set direction
  if (e1 < 0) digitalWrite(DIR, LOW);      // throw it reverse
  else if (e1 > 0) digitalWrite(DIR, HIGH);  // go forward
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
  speedRatio = 1 - abs(0.99 * (MAXSPEED / y));
}

void motorControlStep (uint32_t spd, uint32_t acel, uint8_t dire) {
  // calculate speed and set PWM
  // set driver outputs
}

int32_t motorControlEncoder () {
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

  TC2->TC_CHANNEL[2].TC_RC = 525;   // set timer to 20kHz
  TC2->TC_CHANNEL[2].TC_IER = TC_IER_CPCS; // setup interrupt
  NVIC_EnableIRQ(TC8_IRQn);         // software trigger
  TC2->TC_CHANNEL[2].TC_CCR = TC_CCR_SWTRG | TC_CCR_CLKEN; // Enable timer
  //------------------------------------------------------------------

  // TC7
  //------------------------------------------------------------------
  // turn on TC7
  PMC->PMC_PCER1 |= PMC_PCER1_PID34;

  TC2->TC_CHANNEL[1].TC_CMR = TC_CMR_TCCLKS_TIMER_CLOCK1 | TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC;
  TC2->TC_CHANNEL[1].TC_RC = MAXSPEED / speedRatio;   // set timer to 20kHz
  TC2->TC_CHANNEL[1].TC_IER = TC_IER_CPCS; // setup interrupt
  NVIC_EnableIRQ(TC7_IRQn);         // software trigger
  TC2->TC_CHANNEL[1].TC_CCR = TC_CCR_SWTRG | TC_CCR_CLKEN; // Enable timer

}

// Interupts
void TC7_Handler() {
  TC2->TC_CHANNEL[1].TC_SR;

  digitalWrite(STEP, HIGH);
  digitalWrite(STEP, LOW);
}
void TC8_Handler() {
  TC2->TC_CHANNEL[2].TC_SR;
  //Serial.println('loop');
  // get current speed
  currentSpeed = gainM[2] * (MAXSPEED * speedRatio);
  // functions to run
  motorControl(newPos);
  motorControlEncoder();

  TC2->TC_CHANNEL[1].TC_RC = MAXSPEED * speedRatio; // adjust speed
  digitalWrite(12, HIGH);
  digitalWrite(12, LOW);
}


// test functions
void printer() {
  if (currentPos != lastPos) {
    Serial.print(currentPos);
    Serial.print("  |  ");
    Serial.print(e1);
    Serial.print("  |  ");
    Serial.print(e2);
    Serial.print("  ::  ");
    Serial.println(100 * speedRatio);
  }
  lastPos = currentPos;
  //Serial.print(encoderState[0]); Serial.println(encoderState[1]);
}

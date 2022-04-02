#include <math.h>
#define MCK 84000000
double MAXCOUNT = 5000;


int wave = 0;
double Ts = 0.000002;
double Fm = 5000;
int count = 0;
double pi = 3.14159;
double theta = 0;
double scalar = 0.5;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(230400);
  pinMode(DAC0, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  timerSetup0();
  timerSetup1();
}

void loop() {
  // put your main code here, to run repeatedly:
  //theta = 2*pi*Fm*Ts*count;
  //wave = (1 + sin(theta)) * 127;
  //wave = (1 + sin(2*pi*Fm*Ts*count)) * 127;
  //analogWrite(DAC1,wave);
  //Serial.print(scalar);Serial.print('|');Serial.print(0.7-(scalar*0.3));Serial.print('|');Serial.println(count);
  
}

void timerSetup0(){
  // TC8 is the timer for sensor sampling
  //-----------------------------------------------------------------
  PIOB->PIO_PDR = 1<<25;    // disable pio and enables perifierals
  PIOB->PIO_ABSR = 1<<25;   // set pin B.25 to periferal B which is TIOA0
  
  // turn on timer/counter 2 channel 2
  PMC->PMC_PCER0 |= PMC_PCER0_PID27;
  // set the base clock to MCK/8
  TC0->TC_CHANNEL[0].TC_CMR = TC_CMR_TCCLKS_TIMER_CLOCK2 | TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC |
  TC_CMR_ACPC_SET | TC_CMR_ACPA_TOGGLE;

  TC0->TC_CHANNEL[0].TC_RC = MCK/8/(Fm/16);   // set timer to 100kHz
  TC0->TC_CHANNEL[0].TC_RA = MCK/8/(Fm/16)/2; 
  TC0->TC_CHANNEL[0].TC_IER = TC_IER_CPCS; // setup interrupt
  TC0->TC_CHANNEL[0].TC_IER = TC_IER_CPAS;  // TCRA
  NVIC_EnableIRQ(TC0_IRQn);         // software trigger
  TC0->TC_CHANNEL[0].TC_CCR = TC_CCR_CLKEN | TC_CCR_SWTRG; // Enable timer
}

void timerSetup1(){
  // TC7 is the timer for sensor sampling
  //-----------------------------------------------------------------
  PIOC->PIO_PDR = 1<<28;    // disable pio and enables perifierals
  PIOC->PIO_ABSR = 1<<28;   // set pin B.25 to periferal B which is TIOA0
  
  // turn on timer/counter 2 channel 2
  PMC->PMC_PCER1 |= PMC_PCER1_PID34;
  // set the base clock to MCK/8
  TC2->TC_CHANNEL[1].TC_CMR = TC_CMR_TCCLKS_TIMER_CLOCK2 | TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC |
  TC_CMR_ACPC_SET | TC_CMR_ACPA_TOGGLE;

  TC2->TC_CHANNEL[1].TC_RC = MCK/8/Fm*(1-scalar*0.99);   // set timer to 100kHz
  TC2->TC_CHANNEL[1].TC_RA = MCK/8/Fm/2*(1-scalar*0.99); 
  TC2->TC_CHANNEL[1].TC_IER = TC_IER_CPCS; // setup interrupt
  TC2->TC_CHANNEL[1].TC_IER = TC_IER_CPAS;  // TCRA
  NVIC_EnableIRQ(TC7_IRQn);         // software trigger
  TC2->TC_CHANNEL[1].TC_CCR = TC_CCR_CLKEN | TC_CCR_SWTRG; // Enable timer
}

void TC0_Handler() {

  
  TC0->TC_CHANNEL[0].TC_SR;
  //long dummy = REG_TC0_SR0;
  count++;
  //scalar = count/MAXCOUNT;
  
 
  scalar = (1 + sin(2*pi*1000*Ts*count))/2;

  TC2->TC_CHANNEL[1].TC_RC = MCK/8/Fm/(1-(scalar*0.1));   // set timer to 100kHz
  TC2->TC_CHANNEL[1].TC_RA = MCK/8/Fm/2/(1-(scalar*0.1)); 
  
  //if (count >= MAXCOUNT - 100) count = 1;
  
}
void TC7_Handler(){
  TC2->TC_CHANNEL[1].TC_SR;
}

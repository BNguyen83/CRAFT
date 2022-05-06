#define chB0 22 //check pin names, i need to make them match my mapping in readCh() as well
#define chB1 23
#define chB2 24
#define chB3 25
#define chB4 26
#define chB5 27
#define RESA A0
#define RESB A1
#define RMESPowerEnable 28
#define MuxToggle 29
#define chDelay 20
#define ANAVREF 3.2722

//i need to set these manually based on the channel. This needs to be an array of values for each channel
float RMESa[20] = {0.0333,0.0333,0.0333,0.0333,0.03330,0.0333,0.0333,0.0333,0.0333,0.0333,0.0333,0.0333,0.0333,0.0333,0.0333,0.03330,0.03330,0.03330,0.0330,0.03330};
float RMESb[20] = {-0.0502,-0.0502,-0.0502,-0.0502,-0.0502,-0.0502,-0.0502,-0.0502,-0.0502,-0.0502,-0.0502,-0.0502,-0.0502,-0.0502,-0.0502,-0.0502,-0.0502,-0.0502,-0.0502,-0.0502};
float RMESrs[20] = {218.0,218.0,218.0,218.0,218.0,218.0,218.0,218.0,218.0,218.0,218.0,218.0,218.0,218.0,218.0,218.0,218.0,218.0,218.0,218.0};
//I need to set this based on the number of channels in use. This will get set once when the user decides the number of pins.
float RMESVCal[20] = {0.023,0.023,0.023,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1};
float RMESV = 0; //this will need to be initialized to the correct value in the main function when the user specifies the number of channels they need.
int numCh = 20;
float VtoR(float voltage);
void RMESini(int numOfChannels);
void resetRMES();
int readCh(int chVal);
void measureRMES(float * resArray, int numCh);

void setup() {
  numCh = 2;
  RMESini(numCh);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  float res[20] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  measureRMES(res, numCh);
  for(int i = 0; i < numCh; i++){
    Serial.print("Resistance in Channel ");
    Serial.print(i+1);
    Serial.print("\t");
    Serial.println(res[i],3);}
  delay(5000);
}

void RMESini(int numOfChannels){
  pinMode(chB0, OUTPUT);
  pinMode(chB1, OUTPUT);
  pinMode(chB2, OUTPUT);
  pinMode(chB3, OUTPUT);
  pinMode(chB4, OUTPUT);
  pinMode(chB5, OUTPUT);
  pinMode(RMESPowerEnable, OUTPUT);
  pinMode(MuxToggle, OUTPUT);
  analogReadResolution(12);
  RMESV = RMESVCal[numOfChannels-1];
}
void resetRMES(){
    digitalWrite(RMESPowerEnable, HIGH);
    digitalWrite(MuxToggle, HIGH);
    digitalWrite(chB0, LOW);
    digitalWrite(chB1, LOW);
    digitalWrite(chB2, LOW);
    digitalWrite(chB3, LOW);
    digitalWrite(chB4, LOW);
    digitalWrite(chB5, LOW);
}

int readCh(int chVal){
    int ADCres = 0;
    int avRes[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    switch (chVal){
      case 0:
      break; //this is making certain assumptions that the reset function has ran and that everything is sequential, only changing the pins that NEED to be changed at a given time, and that chB0 actually corresponds to the first bit, etc.
      case 1: //it would have been nice to be able to do this in a more clean and short manner.
        digitalWrite(chB0, HIGH);
      break;        
      case 2:
        digitalWrite(chB0, LOW);
        digitalWrite(chB1, HIGH);
      break;         
      case 3:
        digitalWrite(chB0, HIGH);
      break;        
      case 4:
        digitalWrite(chB0, LOW);
        digitalWrite(chB1, LOW);
        digitalWrite(chB2, HIGH);
      break;        
      case 5:
        digitalWrite(chB0, HIGH);
      break;        
      case 6:
        digitalWrite(chB0, LOW);
        digitalWrite(chB1, HIGH);
      break;        
      case 7:
        digitalWrite(chB0, HIGH);
      break;  
      case 8:
        digitalWrite(chB0, LOW);
        digitalWrite(chB1, LOW);
        digitalWrite(chB2, LOW);
        digitalWrite(chB3, HIGH);
      case 9:
        digitalWrite(chB0, HIGH);
      break;
      case 10:
        digitalWrite(chB0, LOW);
        digitalWrite(chB1, HIGH);
        break;
      case 11:
        digitalWrite(chB0, HIGH);
      break;
      case 12:
        digitalWrite(chB0, LOW);
        digitalWrite(chB1, LOW);
        digitalWrite(chB2, HIGH);
      break;
      case 13:
        digitalWrite(chB0, HIGH);
      break;
      case 14:
        digitalWrite(chB0, LOW);
        digitalWrite(chB1, HIGH);
      break;
      case 15:
        digitalWrite(chB0, HIGH);
      break;
      case 16:
        digitalWrite(MuxToggle, LOW);
      break;
      case 17:
        digitalWrite(chB4, HIGH);
      case 18:
        digitalWrite(chB4, LOW);
        digitalWrite(chB5, HIGH);
      break;
      case 19:
        digitalWrite(chB4, HIGH);
      break;      
     }
    delay(chDelay);
    if (chVal < 15){
      for (int i = 0; i < 10; i++){
        ADCres = ADCres + analogRead(RESA);
        delay(10);
        }
     }
    else {ADCres = analogRead(RESB);}
    if(chVal < 15){
    ADCres = ADCres / 10;}
    return ADCres;
}

void measureRMES(float* resArray, int numCh){ //when everything is working this could be consolidated, we need to check to make sure you can do this kind of thing, passing the address of the float array, and then putting values into the array that was instantiated by another function.
    int RESdigital[20];
    for (int i = 0; i < numCh; i++){
        RESdigital[i] = 0;
    }
    resetRMES();
    for(int i = 0; i < numCh; i++){
        RESdigital[i] = readCh(i);
    }
    for (int i = 0; i < numCh; i++){
        resArray[i] = VtoR((ANAVREF *((float)RESdigital[i] / 4095.0)), i); //do i need to typecast RESdigital to preserve the decimals?
    }
    digitalWrite(RMESPowerEnable, LOW);
    delay(10);
}

float VtoR(float voltage, int i){
  float res = 0;
  float vs = ANAVREF - RMESV;
  res = (RMESrs[i]*(RMESa[i] * voltage + RMESb[i]))/(vs - RMESa[i] * voltage - RMESb[i]);
  return res;
}

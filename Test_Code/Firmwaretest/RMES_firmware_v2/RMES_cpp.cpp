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
    int ch = chVal + 1;
    int mux = 0;
    switch (ch){
      case 1:
        digitalWrite(MuxToggle, HIGH);
        digitalWrite(chB0, LOW);
        digitalWrite(chB1, LOW);
        digitalWrite(chB2, LOW);
        digitalWrite(chB3, LOW);
        digitalWrite(chB4, LOW);
        digitalWrite(chB5, LOW);
        mux = 1;
      break; 
      case 3: 
        digitalWrite(MuxToggle, HIGH);
        digitalWrite(chB0, HIGH);
        digitalWrite(chB1, LOW);
        digitalWrite(chB2, LOW);
        digitalWrite(chB3, LOW);
        digitalWrite(chB4, LOW);
        digitalWrite(chB5, LOW);
        mux = 1;
      break;        
      case 5:
        digitalWrite(MuxToggle, HIGH);
        digitalWrite(chB0, LOW);
        digitalWrite(chB1, HIGH);
        digitalWrite(chB2, LOW);
        digitalWrite(chB3, LOW);
        digitalWrite(chB4, LOW);
        digitalWrite(chB5, LOW);
                mux = 1;
      break;         
      case 7:
              digitalWrite(MuxToggle, HIGH);
        digitalWrite(chB0, HIGH);
        digitalWrite(chB1, HIGH);
        digitalWrite(chB2, LOW);
        digitalWrite(chB3, LOW);
        digitalWrite(chB4, LOW);
        digitalWrite(chB5, LOW);
                mux = 1;
      break;        
      case 9:
              digitalWrite(MuxToggle, HIGH);
        digitalWrite(chB0, LOW);
        digitalWrite(chB1, LOW);
        digitalWrite(chB2, HIGH);
        digitalWrite(chB3, LOW);
        digitalWrite(chB4, LOW);
        digitalWrite(chB5, LOW);
                mux = 1;
      break;        
      case 11:
              digitalWrite(MuxToggle, HIGH);
        digitalWrite(chB0, HIGH);
        digitalWrite(chB1, LOW);
        digitalWrite(chB2, HIGH);
        digitalWrite(chB3, LOW);
        digitalWrite(chB4, LOW);
        digitalWrite(chB5, LOW);
                mux = 1;
      break;        
      case 13:
              digitalWrite(MuxToggle, HIGH);
        digitalWrite(chB0, LOW);
        digitalWrite(chB1, HIGH);
        digitalWrite(chB2, HIGH);
        digitalWrite(chB3, LOW);
        digitalWrite(chB4, LOW);
        digitalWrite(chB5, LOW);
                mux = 1;
      break;        
      case 15:
              digitalWrite(MuxToggle, HIGH);
        digitalWrite(chB0, HIGH);
        digitalWrite(chB1, HIGH);
        digitalWrite(chB2, HIGH);
        digitalWrite(chB3, LOW);
        digitalWrite(chB4, LOW);
        digitalWrite(chB5, LOW);
                mux = 1;
      break;  
      case 17:
              digitalWrite(MuxToggle, HIGH);
        digitalWrite(chB0, LOW);
        digitalWrite(chB1, LOW);
        digitalWrite(chB2, LOW);
        digitalWrite(chB3, HIGH);
        digitalWrite(chB4, LOW);
        digitalWrite(chB5, LOW);
                mux = 1;
        break;
      case 19:
              digitalWrite(MuxToggle, HIGH);
        digitalWrite(chB0, HIGH);
        digitalWrite(chB1, LOW);
        digitalWrite(chB2, LOW);
        digitalWrite(chB3, HIGH);
        digitalWrite(chB4, LOW);
        digitalWrite(chB5, LOW);
                mux = 1;
      break;
      case 2:
              digitalWrite(MuxToggle, HIGH);
        digitalWrite(chB0, LOW);
        digitalWrite(chB1, HIGH);
        digitalWrite(chB2, LOW);
        digitalWrite(chB3, HIGH);
        digitalWrite(chB4, LOW);
        digitalWrite(chB5, LOW);
                mux = 1;
        break;
      case 4:
              digitalWrite(MuxToggle, HIGH);
        digitalWrite(chB0, HIGH);
        digitalWrite(chB1, HIGH);
        digitalWrite(chB2, LOW);
        digitalWrite(chB3, HIGH);
        digitalWrite(chB4, LOW);
        digitalWrite(chB5, LOW);
                mux = 1;
      break;
      case 6:
              digitalWrite(MuxToggle, HIGH);
        digitalWrite(chB0, LOW);
        digitalWrite(chB1, LOW);
        digitalWrite(chB2, HIGH);
        digitalWrite(chB3, HIGH);
        digitalWrite(chB4, LOW);
        digitalWrite(chB5, LOW);
                mux = 1;
      break;
      case 8:
              digitalWrite(MuxToggle, HIGH);
        digitalWrite(chB0, HIGH);
        digitalWrite(chB1, LOW);
        digitalWrite(chB2, HIGH);
        digitalWrite(chB3, HIGH);
        digitalWrite(chB4, LOW);
        digitalWrite(chB5, LOW);
                mux = 1;
      break;
      case 10:
              digitalWrite(MuxToggle, HIGH);
        digitalWrite(chB0, LOW);
        digitalWrite(chB1, HIGH);
        digitalWrite(chB2, HIGH);
        digitalWrite(chB3, HIGH);
        digitalWrite(chB4, LOW);
        digitalWrite(chB5, LOW);
                mux = 1;
      break;
      case 12:
              digitalWrite(MuxToggle, HIGH);      
        digitalWrite(chB0, HIGH);
        digitalWrite(chB1, HIGH);
        digitalWrite(chB2, HIGH);
        digitalWrite(chB3, HIGH);
        digitalWrite(chB4, LOW);
        digitalWrite(chB5, LOW);
                mux = 1;
      break;
      case 14:
        digitalWrite(MuxToggle, LOW);
        digitalWrite(chB0, LOW);
        digitalWrite(chB1, LOW);
        digitalWrite(chB2, LOW);
        digitalWrite(chB3, LOW);
        digitalWrite(chB4, LOW);
        digitalWrite(chB5, LOW);
                mux = 0;
      break;
      case 16:
        digitalWrite(MuxToggle, LOW);
        digitalWrite(chB0, LOW);
        digitalWrite(chB1, LOW);
        digitalWrite(chB2, LOW);
        digitalWrite(chB3, LOW);
        digitalWrite(chB4, HIGH);
        digitalWrite(chB5, LOW);
                        mux = 0;
        break;
      case 18:
        digitalWrite(MuxToggle, LOW);
        digitalWrite(chB0, LOW);
        digitalWrite(chB1, LOW);
        digitalWrite(chB2, LOW);
        digitalWrite(chB3, LOW);
        digitalWrite(chB4, LOW);
        digitalWrite(chB5, HIGH);
                        mux = 0;
      break;
      case 20:
        digitalWrite(MuxToggle, LOW);
        digitalWrite(chB0, LOW);
        digitalWrite(chB1, LOW);
        digitalWrite(chB2, LOW);
        digitalWrite(chB3, LOW);
        digitalWrite(chB4, HIGH);
        digitalWrite(chB5, HIGH);
                        mux = 0;
      break;      
     }
    delay(chDelay);
    if (mux == 0){
      for (int i = 0; i < 20; i++){
        ADCres = ADCres + analogRead(RESB);
        delay(5);
        }
     }
    else {
      for (int i = 0; i < 20; i++){
        ADCres = ADCres + analogRead(RESA);
        delay(5);
      }
    }
    ADCres = ADCres / 20;
    return ADCres;
}

void measureRMES(float* resArray, int numCh){ 
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
  res = (RMESrs*(RMESa[i] * voltage + RMESb[i]))/(Vs - RMESa[i] * voltage - RMESb[i]);
  //Serial.println(voltage);
  return res;
}

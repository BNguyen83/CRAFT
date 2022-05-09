#include <LiquidCrystal.h>
#include "Keypad.h"
#include <SPI.h> // SD card headers
#include <SD.h>
#include "RMES.h" // Mark's header
#include "CRAFT_MC.h" // Andrew's header
#include "MC.h"
#include "INT_HANDLER.h"
#include "force.h" // Belinda's header
#include "HX711ADC.h"



File myFile;
File conf;
/***** System Defaults *****/


uint8_t REED_PIN = 2; // Pin connected to reed switch

/***** LCD *****/
// LCD Pins
uint8_t DB7 = 39; // Pins for 4-bit operation
uint8_t DB6 = 40;
uint8_t DB5 = 41;
uint8_t DB4 = 42;
uint8_t RS  = 43; // Register select
uint8_t RW  = 44; // Read/Write
uint8_t E1  = 45; // top enable, first 2 rows
uint8_t E2  = 46; // bot enable, bottom 2 rows

// Top half LCD initialization
LiquidCrystal top(RS, RW, E1, DB4, DB5, DB6, DB7);

// Bottom half LCD initialization
LiquidCrystal bot(RS, RW, E2, DB4, DB5, DB6, DB7);


/***** Keypad *****/
// Rows
uint8_t K1 = 5;  // A
uint8_t K2 = 6;  // B
uint8_t K7 = 11; // D
uint8_t K8 = 12; // C
// Columns
uint8_t K3 = 7;  // 1
uint8_t K4 = 8;  // 2
uint8_t K5 = 9;  // 3
uint8_t K6 = 10; // 4

const byte ROWS = 4; // four rows
const byte COLS = 4; // four columns

// Key definitions
char hexaKeys[ROWS][COLS] = { {'1', '2', '3', 'A'}, {'4', '5', '6', 'B'}, {'7', '8', '9', 'C'}, {'*', '0', '#', 'D'} };

// Arduino digital pin connection
byte rowPins[ROWS] = {K1, K2, K8, K7};
byte colPins[COLS] = {K3, K4, K5, K6};

Keypad myKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

/*** Test Parameters ***/
int   cycleCount;
float trvSpeed;    // Travel speed
float trvDistance; // Travel Distance
float maxRes;      // Max resistance
float maxInForce_LIMIT;  // Insertion force
float minRemForce_LIMIT; // Removal force
float maxInSpeed;  // Insertion speed
float dwellTime;
int radd[] = {0, 0, 0};
static float radd2 = 0.18;

float res[20] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 , 0 , 0 , 0 , 0, 0, 0, 0};
int   cycleCounter = 0;

float maxForce = 0;
float minForce = 0;

float loadCellCalibration = 871;

/*** State machine flags ***/
int mainState  = 0; // Main
int menuState  = 0; // Menu
int testState  = 0; // Test
int printFlag  = 0; // Keypad
int motorState = 0; // motor
int pauseState = 0; // pause
int endFlag    = 0; // end condition
int doorFlag   = 0;  // Door
int timerFlag  = 0; // motor timer
unsigned long sysTime = 0;


char arr[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int i = 0;
int intFlag = 0;
int holdMainState = 0;

/****** SETUP ******/
void setup()
{
  forceSetup();
  delay(2000);
  // Initialize top and bottom LCD
  top.begin(40, 2);
  bot.begin(40, 2);

  RMESini(20);
  setupMC(0.1, 0.02);
  setupInterrupts();
  //attachInterrupt(digitalPinToInterrupt(A9), doorSwitch, FALLING);

  //Serial.begin(115200);

  /* Other function initializations go here */
}

void loop()
{
  char key = myKeypad.getKey();

  // "interupts" Handler
  interruptHand();
  //motorTimer();
  if (endFlag != 0) {
    mainState = 9;
  }
  switch (mainState)
  {
    /********* Main menu *********/
    case 0:
      disableMotor(true);
      switch (menuState) // Sub-state
      {
        case 0: // check SD card
          if (printFlag == 0) {
            top.clear();
            bot.clear();
            bot.setCursor(31, 1);
            bot.print("<#> Next");
            top.setCursor(1, 0);
            top.print("Welcome to C.R.A.F.T.");
            bot.setCursor(1, 0);
            bot.print("Insert SD card now");
          }
          printFlag = 1;
          cycleCounter = 1;
          if (key == '#') {
            printFlag = 0;
            if (!SD.begin(4)) {
              top.clear();
              bot.clear();
              bot.setCursor(31, 1);
              bot.print("<#> Next");
              top.setCursor(1, 0);
              top.print("**ERROR**");
              bot.setCursor(1, 0);
              bot.print("SD card not detected");
              menuState = 0;
              printFlag = 0;
              delay(2000);
            } else {
              top.clear();
              bot.clear();
              bot.setCursor(31, 1);
              bot.print("<#> Nice");
              top.setCursor(1, 0);
              top.print("Welcome to C.R.A.F.T.");
              bot.setCursor(1, 0);
              bot.print("SD card detected!");

              myFile = SD.open("test.txt", FILE_WRITE);
              //myFile.println("testing 1, 2, 3.");
            }
            delay(1000);
            menuState++;
          } break;
        case 1: // Main menu prompt
          if (printFlag == 0) {
            top.clear();
            bot.clear();
            top.setCursor(1, 0);
            top.print("Welcome to C.R.A.F.T.!");
            top.setCursor(1, 1);
            top.print("Connection Resistance And Force Tester ");

            bot.setCursor(1, 0);
            bot.print("<R> Begin test  <Y> Set Origin ");
            //bot.setCursor(1, 1);
            //bot.print("<B> Set Origin  <G> System defaults");

          }
          printFlag = 1;
          //<<< <<< < Updated upstream
          //iniConfig();
          //== == == =
          //iniConfig();
          //>>> >>> > Stashed changes
          //Serial.println(loadCellCalibration);
          if (key == 'A') {
            menuState++;
            printFlag = 0;
          } else if (key == 'B') {
            mainState = 3;
            printFlag = 0;
          } else if (key == 'C') {
            //mainState = 3;
            printFlag = 0;
          } else if (key == 'D') {
            //mainState = 4;
            printFlag = 0;
          }
          break;
        case 2: // Cycle count
          if (printFlag == 0) {
            top.clear();
            bot.clear();
            bot.setCursor(21, 1);
            bot.print("<*> Back  <#> Next");
            top.setCursor(1, 0);
            top.print("Set test parameters...");

            bot.setCursor(1, 0);
            bot.print("Cycle count: ");
          }
          printFlag = 1;

          if (key > 47 && key < 58 && i < 9) {
            bot.setCursor(14 + i, 0);
            arr[i] = key;
            bot.print(key);
            i++;
          }

          if (key == '#') {
            String ms = String(arr);
            ms.remove(i);
            cycleCount = ms.toInt();
            menuState++;
            i = 0;
            printFlag = 0;
          } else if (key == '*') {
            String ms = String(arr);
            ms.remove(i);
            i = 0;
            menuState--;
            printFlag = 0;
          } break;
        case 3: // Travel speed
          if (printFlag == 0) {
            top.clear();
            bot.clear();
            bot.setCursor(21, 1);
            bot.print("<*> Back  <#> Next");
            top.setCursor(1, 0);
            top.print("Set test parameters...");

            bot.setCursor(1, 0);
            bot.print("Travel speed (mm/s): ");
          } // in/s
          printFlag = 1;

          if (key > 45 && key < 58 && i < 9) {
            bot.setCursor(22 + i, 0);
            arr[i] = key;
            bot.print(key);
            i++;


          }

          if (key == '#') {
            String ms = String(arr);
            ms.remove(i);
            trvSpeed = ms.toFloat();
            convertSpeed(trvSpeed);
            //Serial.println(trvSpeed);
            menuState++;
            i = 0;
            printFlag = 0;
          } else if (key == '*') {
            String ms = String(arr);
            ms.remove(i);
            i = 0;
            menuState--;
            printFlag = 0;
          } break;
        case 4: // Travel distance
          if (printFlag == 0) {
            top.clear();
            bot.clear();
            bot.setCursor(21, 1);
            bot.print("<*> Back  <#> Next");
            top.setCursor(1, 0);
            top.print("Set test parameters...");

            bot.setCursor(1, 0);
            bot.print("Travel distance (mm): "); //
            printFlag = 1;
          }
          if (key > 45 && key < 58 && i < 9) {
            bot.setCursor(23 + i, 0);
            arr[i] = key;
            bot.print(key);
            i++;
          }
          if (key == '#') {
            String ms = String(arr);
            ms.remove(i);
            trvDistance = ms.toFloat();
            if (trvDistance > 30) {
              trvDistance = 30;
            }
            menuMotor(trvDistance * -1); // move to new open position
            menuState++;
            i = 0;
            printFlag = 0;
          }
          else if (key == '*') {
            String ms = String(arr);
            ms.remove(i);
            i = 0;
            menuState--;
            printFlag = 0;
          } break;
        case 5: // Max resistance
          if (printFlag == 0) {
            top.clear();
            bot.clear();
            bot.setCursor(21, 1);
            bot.print("<*> Back  <#> Next");
            top.setCursor(1, 0);
            top.print("Set test parameters...");

            bot.setCursor(1, 0);
            bot.print("Resistance (mOhms): ");
          }
          printFlag = 1;

          if (key > 47 && key < 58 && i < 9) {
            bot.setCursor(21 + i, 0);
            arr[i] = key;
            bot.print(key);
            i++;
          }
          if (key == '#') {
            String ms = String(arr);
            ms.remove(i);
            maxRes = (float)ms.toInt() / 1000.0;
            //Serial.println(maxRes);
            menuState++;
            i = 0;
            printFlag = 0;
          } else if (key == '*') {
            String ms = String(arr);
            ms.remove(i);
            i = 0;
            menuState--;
            printFlag = 0;
          } break;
        case 6: // Max insertion force
          if (printFlag == 0) {
            top.clear();
            bot.clear();
            bot.setCursor(21, 1);
            bot.print("<*> Back  <#> Next");
            top.setCursor(1, 0);
            top.print("Set test parameters...");

            bot.setCursor(1, 0);
            bot.print("Insertion force (g): ");
          }
          printFlag = 1;

          if (key > 45 && key < 58 && i < 9) {
            bot.setCursor(22 + i, 0);
            arr[i] = key;
            bot.print(key);
            i++;
          }

          if (key == '#') {
            String ms = String(arr);
            ms.remove(i);
            maxInForce_LIMIT = ms.toFloat();

            menuState++;
            i = 0;
            printFlag = 0;
          } else if (key == '*') {
            String ms = String(arr);
            ms.remove(i);
            i = 0;
            menuState--;
            printFlag = 0;
          } break;
        case 7: // Minimum removal force
          if (printFlag == 0) {
            top.clear();
            bot.clear();
            bot.setCursor(21, 1);
            bot.print("<*> Back  <#> Next");
            top.setCursor(1, 0);
            top.print("Set test parameters...");

            bot.setCursor(1, 0);
            bot.print("Removal force (g): ");
          }
          printFlag = 1;

          if (key > 45 && key < 58 && i < 9) {
            bot.setCursor(20 + i, 0);
            arr[i] = key;
            bot.print(key);
            i++;
          }
          if (key == '#') {
            String ms = String(arr);
            ms.remove(i);
            minRemForce_LIMIT = ms.toFloat();
            menuState++;
            i = 0;
            printFlag = 0;
          } else if (key == '*') {
            String ms = String(arr);
            ms.remove(i);
            i = 0;
            menuState--;
            printFlag = 0;
          } break;

        case 8: // Dwell time
          if (printFlag == 0) {
            top.clear();
            bot.clear();
            bot.setCursor(21, 1);
            bot.print("<*> Back  <#> Next");
            top.setCursor(1, 0);
            top.print("Set test parameters...");

            bot.setCursor(1, 0);
            bot.print("Dwell time (s): ");
          }
          printFlag = 1;

          if (key > 45 && key < 58 && i < 9) {
            bot.setCursor(17 + i, 0);
            arr[i] = key;
            bot.print(key);
            i++;
          }

          if (key == '#') {
            String ms = String(arr);
            ms.remove(i);
            dwellTime = ms.toFloat();
            menuState++;
            i = 0;
            printFlag = 0;
          } else if (key == '*') {
            String ms = String(arr);
            ms.remove(i);
            i = 0;
            menuState--;
            printFlag = 0;
          } break;
        case 9: // Confirm test
          if (printFlag == 0) {
            top.clear();
            bot.clear();
            bot.setCursor(24, 1);
            bot.print("<*> No  <#> Yes");
            top.setCursor(1, 0);
            top.print("Begin test?");
          }
          printFlag = 1;

          if (key == '#') {
            mainState = 1;
            printFlag = 0;
            cycleCounter = 1;
          } else if (key == '*') {
            menuState--;
            printFlag = 0;
          } break;
      } break; // This } is for 'switch(menuState)'

    /********* Testing *********/
    case 1:
      doorFlag = 1;
      switch (testState) // Sub-state
      {
        case 0: // Drivetrain running
          if (printFlag == 0) {
            top.clear();
            bot.clear();
            // Top half LCD
            top.setCursor(1, 0);
            top.print("Total cycles:  ");
            top.setCursor(16, 0);
            top.print(cycleCount);
            top.setCursor(1, 1);
            top.print("Current cycle: ");
            top.setCursor(16, 1);
            top.print(cycleCounter);

            // Bot half LCD
            bot.setCursor(1, 0);
            bot.print("Ins. Force (g): ");
            bot.setCursor(17, 0);
            bot.print(maxForce);
            bot.setCursor(1, 1);
            bot.print("Rem. Force (g): ");
            bot.setCursor(17, 1);
            bot.print(minForce);
            bot.setCursor(30, 1);
            bot.print("<G> Pause");

            top.setCursor(24, 1);
            top.print("Res. (O): ");
            top.setCursor(35, 1);
            top.print(res[resSort()]);
          }
          testState = 1;
          if (key == 'D') {
            mainState = 6;
            pauseState = 0;
            printFlag = 0;
            disableMotor(true);
          }
          minForce = 0;
          maxForce = 0;
          break;

        //---------------------------------------------------------------------------
        // cycle procedure
        case 1: // insertion
          resetForceFlag();
          setPosition(0);    // move to closed position
          timerFlag = 1;     // start timer
          mainState = 5;
          if (key == 'D') {
            mainState = 6;
            pauseState = 0;
            printFlag = 0;
            disableMotor(true);
          }
          break;
        case 2:
          //Serial.println("End of insertion");
          delay(dwellTime * 1000);
          measureRMES(res, 20); // run resistance measurment
          for (int n = 0; n < 20; n++) {
            res[n] = res[n] - radd2;
          }
          //delay(dwellTime * 1000);
          resetForceFlag();
          setPosition(trvDistance * -1);
          timerFlag = 1;      // start timer
          mainState = 5;
          if (key == 'D') {
            mainState = 6;
            pauseState = 0;
            printFlag = 0;
            disableMotor(true);
          }
          break;
        case 3:
          timerFlag = 0;
          printToSD();        // print stuff to SD card
          //Serial.println(minForce);
          //Serial.println(maxForce);
          cycleCounter++;
          testEnd();
          //endFlag = 4;
         
          //------------------------------------------------------------------------
          if (cycleCounter > cycleCount) {
            mainState = 9;
            menuState = 0;
            printFlag = 0;
            myFile.close();
            for (int w = 0; w < 20; w++) {
              res[w] = 0;
            }
            maxForce = 0;
            minForce = 0;
          }
          testState = 0;
          if (key == 'D') {
            mainState = 6;
            pauseState = 0;
            printFlag = 0;
            disableMotor(true);
          }

          break;
          /*
            case 1: // User/System pause
            break;

            case 2: // Emergency pause
            break;
          */
      } break; // This } is for 'switch(testState)'

    /******** Test Cycle ********/
    //case 2:
    //break;

    /********* Set origin *********/
    case 3:

      if (printFlag == 0) {
        changeTopSpeed(0.5);
        top.clear();
        bot.clear();
        bot.setCursor(31, 1);
        bot.print("<#> Next");
        top.setCursor(1, 0);
        top.print("Set origin (mm)... ");
        top.setCursor(1, 1);
        top.print("<1>  5.0  <2>  1.0  <3>  0.5");
        bot.setCursor(1, 0);
        bot.print("<4> -0.5  <5> -1.0  <6> -5.0");
        bot.setCursor(1, 1);
        bot.print("<7>  0.1  <8> -0.1");
        printFlag = 1;
      }
      if (key == '1') {
        jogMotor(5.0);
      } else if (key == '2') {
        jogMotor(1.0);
      } else if (key == '3') {
        jogMotor(0.5);
      } else if (key == '4') {
        jogMotor(-0.5);
      } else if (key == '5') {
        jogMotor(-1.0);
      } else if (key == '6') {
        jogMotor(-5.0);
      } else if (key == '7') {
        jogMotor(0.1);
      } else if (key == '8') {
        jogMotor(-0.1);
      } else if (key == '#') {
        mainState = 0;
        printFlag = 0;
        changeTopSpeed(.9);
        setOrigin();
      }

      break;

    /******** More Options ********/
    //case 4:
    //break;

    /******* Motor Loop ******/
    case 5:
      switch (motorState) {
        case 0: disableMotor(false); motorState = 1;
          if (key == 'D') {
            mainState = 6;
            pauseState = 0;
            printFlag = 0;
            disableMotor(true);
          }
        case 1: // put the stuff you want to do here
          switch (testState) {
            case 1: maxForce = measureInsertion(); break;
            case 2: minForce = measureRemoval(); break;
          }
          if (isRun() != 1) motorState = 2;

          if (key == 'D') {
            mainState = 6;
            pauseState = 0;
            printFlag = 0;
            disableMotor(true);
          }
          break;
        case 2: disableMotor(true); testState++; motorState = 0; mainState = 1;
          if (key == 'D') {
            mainState = 6;
            pauseState = 0;
            printFlag = 0;
            disableMotor(true);
          }
          break;
      }
      break;
    /******* pause ******/
    case 6:
    timerFlag = 0;
      switch (pauseState) {
        case 0: // Pause menu
          if (printFlag == 0) {
            top.clear();
            bot.clear();
            top.setCursor(1, 0);
            top.print("Test is paused...");
            bot.setCursor(31, 1);
            bot.print("<*> Back");
            top.setCursor(1, 1);
            top.print("<R> Stop test");
            bot.setCursor(1, 0);
            bot.print("<Y> Jog Motor");
            printFlag = 1;
          }

          if (key == '*') {
            mainState = 1;
            printFlag = 0;
          } else if (key == 'A') { // go to end screen
            mainState = 0;
            menuState = 0;
            printFlag = 0;
            myFile.close();
            for (int w = 0; w < 20; w++) {
              res[w] = 0;
            }
            maxForce = 0;
            minForce = 0;
          } else if (key == 'B') { // go to jog motor
            pauseState = 1;
            printFlag = 0;
          }
          break;
        case 1: // Jog motor in pause state
          //changeTopSpeed(0.5);
          if (printFlag == 0) {
            top.clear();
            bot.clear();
            bot.setCursor(31, 1);
            bot.print("<#> Back");
            top.setCursor(1, 0);
            top.print("Jog motor (mm)... ");
            top.setCursor(1, 1);
            top.print("<1>  5.0  <2>  1.0  <3>  0.5");
            bot.setCursor(1, 0);
            bot.print("<4> -0.5  <5> -1.0  <6> -5.0");
            bot.setCursor(1, 1);
            bot.print("<7>  0.1  <8> -0.1");
            printFlag = 1;
          }
          if (key == '1') {
            jogMotor(5.0);
          } else if (key == '2') {
            jogMotor(1.0);
          } else if (key == '3') {
            jogMotor(0.5);
          } else if (key == '4') {
            jogMotor(-0.5);
          } else if (key == '5') {
            jogMotor(-1.0);
          } else if (key == '6') {
            jogMotor(-5.0);
          } else if (key == '7') {
            jogMotor(0.1);
          } else if (key == '8') {
            jogMotor(-0.1);
          } else if (key == '#') {
            pauseState = 0;
            printFlag = 0;
          }
          changeTopSpeed(1);
          break;
      } // This } is for 'pauseState'


    /******* Test end *******/
    case 7:


      break;
    default:
      setup();
      break;

    /******* ERROR SCREEN *******/
    case 8:
      switch (intFlag) {
        case 0:
          mainState = holdMainState;
          printFlag = 0;
          disableMotor(false);
          //delay(2000);

          break;
        case 1:
          disableMotor(true);
          if (printFlag == 0) {
            if (printFlag == 0) {
              top.clear();
              bot.clear();
              top.setCursor(2, 1);
              top.print("****** EMERGENCY STOP ENGAGED ******");
              printFlag = 1;
            }
          }
          printFlag = 1;
          break;
        case 2:
          disableMotor(true);
          if (printFlag == 0) {
            if (printFlag == 0) {
              top.clear();
              bot.clear();
              top.setCursor(2, 1);
              top.print("****** OBSTRUCTION DETECTED ******");
              printFlag = 1;
            }
          }
          break;
        case 3:
          disableMotor(true);
          if (printFlag == 0) {
            top.clear();
            bot.clear();
            top.setCursor(2, 1);
            top.print("****** DOOR OPEN ******");
            printFlag = 1;
          }
          intFlag = 10;
          delay(2000);
          printFlag = 0;
          mainState = 6;
          printFlag = 0;

          break;

        case 5:
          disableMotor(true);
          if (printFlag == 0) {
            if (printFlag == 0) {
              top.clear();
              bot.clear();
              top.setCursor(2, 1);
              top.print(" MOTOR STALLED!!!   RESTART TEST");
              printFlag = 1;
            }
          }
          break;
      }
      break;
    case 9: //end of test
      doorFlag = 0;
      if (printFlag == 0) {
        switch (endFlag) {
          case 1:
            top.clear();
            bot.clear();
            bot.setCursor(31, 1);
            bot.print("<#> Next");
            top.setCursor(1, 0);
            top.print("Max insertion force reached, test ended.");
            printFlag = 1;
            break;
          case 2:
            top.clear();
            bot.clear();
            bot.setCursor(31, 1);
            bot.print("<#> Next");
            top.setCursor(1, 0);
            top.print("Min removal force reached, test ended.");
            printFlag = 1;
            break;
          case 3:
            top.clear();
            bot.clear();
            bot.setCursor(31, 1);
            bot.print("<#> Next");
            top.setCursor(1, 0);
            top.print("Max resistance reached, test ended.");
            printFlag = 1;
            break;
          case 4:
            top.clear();
            bot.clear();
            bot.setCursor(31, 1);
            bot.print("<#> Next");
            top.setCursor(1, 0);
            top.print("Target cycle count reached, test ended.");
            printFlag = 1;
            break;
        }
        myFile.close();
        for (int w = 0; w < 20; w++) {
          res[w] = 0;
        }
        maxForce = 0;
        minForce = 0;
      }
      if (key == '#') {
        mainState = 0;
        menuState = 0;
        printFlag = 0;
        endFlag = 0;
      }
      break;

    case 10: // end screen
      if (printFlag == 0) {
        top.clear();
        bot.clear();
        top.setCursor(1, 0);
        top.print("Test is now complete!");
        top.setCursor(1, 1);
        top.print("Cycles completed: ");
        top.setCursor(19, 1);
        top.print(cycleCounter);
        bot.setCursor(1, 0);
        bot.print("End condition: ");
      }
      break;

      //  default: break;

      //break;

  }// This } is for 'switch(mainState)'
  resetIntFlag();
}

// this function will not work
/*
  void enclosureReading()
  {
  pinMode(REED_PIN, INPUT_PULLUP); //pull-up the reed switch pin internally.
  int proximity = digitalRead(REED_PIN); // Read the state of the switch
  if (proximity == HIGH) // If the pin reads low, the switch is closed.
  {
    Serial.println ("The test paused because the lid is open");
    Serial.println ("Please close it");
    delay(1000);
  }


  }
*/
void printToSD() {
  myFile.print(cycleCounter);
  myFile.print("|");
  for (int g = 0; g < 20; g++) {
    // print resistance values
    myFile.print(res[g]);
    myFile.print(" ");
  }
  myFile.print("\t");
  myFile.print(maxForce);
  myFile.print(" ");
  myFile.print(minForce);
  myFile.println();

}

void convertSpeed(float input) {
  changeTopSpeed(map(input, 5.08, 25.4, 0, 10000) / 10000.0);
  //Serial.println(map(input, 5.08, 25.4, 0, 10000)/10000.0);
}

void interruptHand() {
  // E-stop
  if (intFlag == 5) {
    while (1);
  }
  if (digitalRead(34)) {
    // send to error screen
    intFlag = 1;
    if (mainState != 8) {
      holdMainState = mainState;  // store current state
      printFlag = 0;
    }
    mainState = 8;              // move to error screen


  }
  // Obstruction Detection
  /*
    if (digitalRead(35)) {
      // send to error screen
      intFlag = 2;
      if (mainState != 8) {
        holdMainState = mainState;  // store current state
        printFlag = 0;
      }
      mainState = 8;              // move to error screen


    }*/
  // door switch

  if (digitalRead(A9) != 0 && doorFlag) {
    // send to error screen
    if (intFlag != 10) {
      intFlag = 3;
      if (mainState != 8) {
        holdMainState = mainState;  // store current state
        printFlag = 0;
        mainState = 8;              // move to error screen
      }
    }
    //else intFlag = 10;




  }/*
  Serial.print(mainState);
  Serial.print(" ");
  Serial.print(holdMainState);
  Serial.print(" ");
  Serial.println(intFlag);
*/
  //delay(10);

}



void doorSwitch() {

  disableMotor(true);

  if (printFlag == 0) {
    top.clear();
    bot.clear();
    top.setCursor(2, 1);
    top.print("****** Door open ******");
    printFlag = 1;
  }/*
  Serial.print(mainState);
  Serial.print(" ");
  Serial.print(digitalRead(A9));
  Serial.print(" ");
  Serial.println(intFlag);
*/
  delay(2000);


}

void resetIntFlag() {
  if (digitalRead(34) != 1) {
    switch (intFlag) {
      case (1): intFlag = 0; break;
      case (2): break;
      //case (3): intFlag = 10; break;
      default: break;
    }
    if (digitalRead(A9) == 0) intFlag = 0;
  }
}

void testEnd() {
  if (maxForce > maxInForce_LIMIT) {
    radd[0]++;
    if (radd[0] > 3) {
      endFlag = 2;
    }

  }
  else {
    radd[0] = 0;
  }
  if (minForce < minRemForce_LIMIT) {
    radd[1]++;
    if (radd[1] > 3) {
      endFlag = 1;
    }

  }
  else {
    radd[1] = 0;
  }
  for (int rescount = 0; rescount < 20; rescount++) {
    if (res[rescount] > maxRes) {
      radd[2]++;
      //Serial.println(radd[2]);
      break;
    }
    else {
      radd[2] = 0;
    }
  }
  if (radd[2] > 3) {
    endFlag = 3;
  }



  if (cycleCounter > cycleCount) {
    endFlag = 4;
  }
}

void iniConfig() {
  if (SD.exists("config.txt")) {
    conRead();
  }
  else {
    conFormat();
  }
  conf.close();

}

void conFormat() {
  conf = SD.open("config.txt", FILE_WRITE);
  conf.print(loadCellCalibration);
}

void conRead() {
  conf = SD.open("config.txt", FILE_READ);
  while (conf.available()) {

  }

  loadCellCalibration = conf.read();
  //Serial.write(loadCellCalibration);
}

int resSort() {
  int index = 0;
  for (int w = 0; w < 20; w++) {
    if (res[w + 1] > res[index]) index = w + 1;
  }
  return index;
}

void motorTimer() {
  switch (timerFlag) {
    case 0: // no movemet

      break;
    case 1: // start timer
      sysTime = millis();
      timerFlag = 2;
      break;
    case 2: // check time
      unsigned long checkTime = millis();
      if (checkTime - sysTime > 3000) {
        mainState = 8;
        intFlag = 5;
        disableMotor(true);
        myFile.close();
      }
      break;
  }
}

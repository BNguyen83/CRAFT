#include <LiquidCrystal.h>
#include "Keypad.h"
#include <SPI.h> // SD card headers
#include <SD.h>
#include "RMES.h" // Mark's header
#include "CRAFT_MC.h" // Andrew's header
#include "MC.h"
//#include "INT_HANDLER.h"
#include "force.h" // Belinda's header
#include "HX711ADC.h"

File myFile;

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
char hexaKeys[ROWS][COLS] = { {'1', '2', '3', 'A'}, {'4', '5', '6', 'B'}, {'7', '8', '9', 'C'}, {'*', '0', '#', '.'} };

// Arduino digital pin connection
byte rowPins[ROWS] = {K1, K2, K8, K7};
byte colPins[COLS] = {K3, K4, K5, K6};

Keypad myKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);


/*** Test Parameters ***/
int   cycleCount;
float trvSpeed;    // Travel speed
float trvDistance; // Travel Distance
int   maxRes;      // Max resistance
float maxInForce;  // Insertion force
float minRemForce; // Removal force
float maxInSpeed;  // Insertion speed
float dwellTime;

float res[20] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 , 0 , 0 , 0 , 0, 0, 0, 0};
int   cycleCounter = 0;

int maxforce = 0;

/*** State machine flags ***/
int mainState = 0; //Main
int menuState = 0; // Menu
int testState = 0; // Test
int printFlag = 0; // Keypad
int motorState = 0; // motor


char arr[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int i = 0;

void setup()
{
  // Initialize top and bottom LCD
  top.begin(40, 2);
  bot.begin(40, 2);

  RMESini(20);
  setupMC(12000, 4000, 0.5, 0.02);
  forceSetup();
  Serial.begin(9600);

  /* Other function initializations go here */
}

void loop()
{
  char key = myKeypad.getKey();
  /*
    insertionForce(&maxforce);
    Serial.print("Insertion Force (kg): ");
    Serial.println(maxforce);
  */
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

              if (key == '#') {
                menuState = 0;
                printFlag = 0;
              }

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
            bot.print("<A> Begin test  <B> Test Cycle ");
            bot.setCursor(1, 1);
            bot.print("<C> Set Origin  <D> Set Force");
          }
          printFlag = 1;
          if (key == 'A') {
            menuState++;
            printFlag = 0;
          } else if (key == 'B') {
            //mainState = 2;
            printFlag = 0;
          } else if (key == 'C') {
            mainState = 3;
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
            Serial.println(cycleCount);
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
            Serial.println(trvSpeed);
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
            Serial.println(trvDistance);
            jogMotor(trvDistance * -1); // move to new open position
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
            maxRes = ms.toInt();
            Serial.println(maxRes);
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
            bot.print("Insertion force (kg): ");
          }
          printFlag = 1;

          if (key > 45 && key < 58 && i < 9) {
            bot.setCursor(23 + i, 0);
            arr[i] = key;
            bot.print(key);
            i++;
          }

          if (key == '#') {
            String ms = String(arr);
            ms.remove(i);
            maxInForce = ms.toFloat();
            Serial.println(maxInForce);
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
            bot.print("Removal force (kg): ");
          }
          printFlag = 1;

          if (key > 45 && key < 58 && i < 9) {
            bot.setCursor(21 + i, 0);
            arr[i] = key;
            bot.print(key);
            i++;
          }
          if (key == '#') {
            String ms = String(arr);
            ms.remove(i);
            minRemForce = ms.toFloat();
            Serial.println(minRemForce);
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
        case 8: // Insertion speed
          if (printFlag == 0) {
            top.clear();
            bot.clear();
            bot.setCursor(21, 1);
            bot.print("<*> Back  <#> Next");
            top.setCursor(1, 0);
            top.print("Set test parameters...");

            bot.setCursor(1, 0);
            bot.print("Insertion speed: ");
          }
          printFlag = 1;

          if (key > 45 && key < 58 && i < 9) {
            bot.setCursor(18 + i, 0);
            arr[i] = key;
            bot.print(key);
            i++;
          }
          if (key == '#') {
            String ms = String(arr);
            ms.remove(i);
            maxInSpeed = ms.toFloat();
            Serial.println(maxInSpeed);
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
        case 9: // Dwell time
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
            Serial.println(dwellTime);
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
        case 10: // Confirm test
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
      switch (testState) // Sub-state
      {
        case 0: // Drivetrain running
          if (printFlag == 0) {
            top.clear();
            bot.clear();
            top.setCursor(1, 0);
            top.print("Total cycles: ");
            top.setCursor(15, 0);
            top.print(cycleCount);
            bot.setCursor(1, 0);
            bot.print("Current cycle: ");
            bot.setCursor(16, 0);
            bot.print(cycleCounter);
          }
          testState = 1;
          break;

        //---------------------------------------------------------------------------
        // cycle procedure
        case 1: // insertion
          setPosition(0);    // move to closed position
          mainState = 5;
          break;
        case 2:
          measureRMES(res, 20); // run resistance measurment
          delay(dwellTime * 1000);
          setPosition(trvDistance * -1);
          mainState = 5;
          break;
        case 3:
          cycleCounter++;
          printToSD();        // print stuff to SD card

          //------------------------------------------------------------------------
          if (cycleCounter >= cycleCount + 1) {
            mainState = 0;
            menuState = 0;
            printFlag = 0;
            myFile.close();
          }
          testState = 0;
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
        setOrigin();
      }
      break;

    /******** Set Force ********/
    //case 4:
    //break;
    default:
      setup();
      break;

    case 5:
      switch (motorState) {
        case 0: disableMotor(false); motorState = 1; break;   // enableMotor
        case 1: // put the stuff you want to do here
          if (isRun() != 1) motorState = 2;

          break;
        case 2: disableMotor(true); testState++; motorState = 0; mainState = 1; break;
      }





  } // This } is for 'switch(mainState)'
}

// this function will not work
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

void printToSD() {
  for (int g = 0; g < 20; g++) {
    // print resistance values
    myFile.print(res[g]);
    myFile.print(" ");
  }
  myFile.println();
}

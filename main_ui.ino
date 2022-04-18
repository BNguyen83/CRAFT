#include <LiquidCrystal.h>
#include <Keypad.h>

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


// Keypad pins
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


// Test parameters
int   cycleCount;
float trvSpeed;    // Travel speed
float trvDistance; // Travel Distance
float maxRes;      // Max resistance
float maxInForce;  // Insertion force
float maxRemForce; // Removal force
float maxInSpeed;  // Insertion speed 
float dwellTime;

//Main
int mainState = 0;
// Menu
int menuState = 0;
// Test
int testState = 0;

void setup() 
{ 
  // Initialize top and bottom LCD
  top.begin(40,2);
  bot.begin(40,2);

  // Welcome prompt
  top.setCursor(1,0);
  top.print("Welcome to C.R.A.F.T.!");
  top.setCursor(1,1);
  top.print("Connection Resistance And Force Tester ");

  // Menu prompt
  bot.setCursor(1,1);
  bot.print("<A> Begin test  <B> Test Cycle ");
}

void loop() 
{
    char key = myKeypad.getKey();
  
  switch(mainState){
    case 0: // Start Menu
      switch(menuState){ // Sub-state
        case 0: // Main menu prompt
            bot.clear();
            top.setCursor(1,0);
            top.print("Welcome to C.R.A.F.T.!");
            top.setCursor(1,1);
            top.print("Connection Resistance And Force Tester ");

            bot.setCursor(1,1);
            bot.print("<A> Begin test  <B> Test Cycle ");

          if(key == 'A'){
            menuState++; // Menu state = 1, does it go to case 1 now?
          }break;
        case 1: // Cycle count
          top.clear();
              bot.clear();
              bot.setCursor(21,1);
              bot.print("<*> Back  <#> Next");
              top.setCursor(1,0);
              top.print("Set test parameters...");
    
            bot.setCursor(1,0); 
              bot.print("Cycle count: ");
          /* Code where keypad is scanned */

          if(key == '#'){
            menuState++;
          } else if(key == '*'){
            menuState--;
          }break;
        case 2: // Travel speed
          top.clear();
              bot.clear();
              bot.setCursor(21,1);
              bot.print("<*> Back  <#> Next");
              top.setCursor(1,0);
              top.print("Set test parameters...");
    
            bot.setCursor(1,0); 
              bot.print("Travel speed: ");
          /* Code where keypad is scanned */

          if(key == '#'){
            menuState++;
          } else if(key == '*'){
            menuState--;
          }break;
        case 3: // Travel distance
          top.clear();
              bot.clear();
              bot.setCursor(21,1);
              bot.print("<*> Back  <#> Next");
              top.setCursor(1,0);
              top.print("Set test parameters...");
    
            bot.setCursor(1,0); 
              bot.print("Travel distance: ");
          /* Code where keypad is scanned */

          if(key == '#'){
            menuState++;
          } else if(key == '*'){
            menuState--;
          }break;
        case 4: // Max resistance
          top.clear();
              bot.clear();
              bot.setCursor(21,1);
              bot.print("<*> Back  <#> Next");
              top.setCursor(1,0);
              top.print("Set test parameters...");
    
            bot.setCursor(1,0); 
              bot.print("Resistance (mOhms): ");
          /* Code where keypad is scanned */

          if(key == '#'){
            menuState++;
          } else if(key == '*'){
            menuState--;
          }break;
        case 5: // Max insertion force
          top.clear();
              bot.clear();
              bot.setCursor(21,1);
              bot.print("<*> Back  <#> Next");
              top.setCursor(1,0);
              top.print("Set test parameters...");
    
            bot.setCursor(1,0); 
              bot.print("Insertion force (kg): ");
          /* Code where keypad is scanned */

          if(key == '#'){
            menuState++;
          } else if(key == '*'){
            menuState--;
          }break;
        case 6: // Max removal force
          top.clear();
              bot.clear();
              bot.setCursor(21,1);
              bot.print("<*> Back  <#> Next");
              top.setCursor(1,0);
              top.print("Set test parameters...");
    
            bot.setCursor(1,0); 
              bot.print("Removal force (kg): ");
          /* Code where keypad is scanned */

          if(key == '#'){
            menuState++;
          } else if(key == '*'){
            menuState--;
          }break;
        case 7: // Insertion speed
          top.clear();
              bot.clear();
              bot.setCursor(21,1);
              bot.print("<*> Back  <#> Next");
              top.setCursor(1,0);
              top.print("Set test parameters...");
    
            bot.setCursor(1,0); 
              bot.print("Insertion speed: ");
          /* Code where keypad is scanned */

          if(key == '#'){
            menuState++;
          } else if(key == '*'){
            menuState--;
          }break;
        case 8: // Dwell time
          top.clear();
              bot.clear();
              bot.setCursor(21,1);
              bot.print("<*> Back  <#> Next");
              top.setCursor(1,0);
              top.print("Set test parameters...");
    
            bot.setCursor(1,0); 
              bot.print("Dwell time: ");
          /* Code where keypad is scanned */

          if(key == '#'){
            mainState++; // Main state is incremented
          } else if(key == '*'){
            menuState--;
          }break;
      }
    //case 1: // Testing
    //case 2: // Testing menu
  }
}

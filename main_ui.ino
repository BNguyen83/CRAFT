#include <LiquidCrystal.h>
#include <Keypad.h>

// LCD Pins
uint8_t DB7 = 22; // Pins for 4-bit operation
uint8_t DB6 = 23;
uint8_t DB5 = 24;
uint8_t DB4 = 25;
uint8_t RS = 33; // Register select
uint8_t RW = 32; // Read/Write
uint8_t E1 = 30; // top enable, first 2 rows
uint8_t E2 = 31; // bot enable, bottom 2 rows

// Keypad pins
uint8_t K1 = 36; // Columns
uint8_t K2 = 37;
uint8_t K3 = 38;
uint8_t K4 = 39;
uint8_t K5 = 40; // Rows
uint8_t K6 = 41;
uint8_t K7 = 42;
uint8_t K8 = 43;

// Top half LCD initialization
LiquidCrystal top(RS, RW, E1, DB4, DB5, DB6, DB7);

// Bottom half LCD initialization
LiquidCrystal bot(RS, RW, E2, DB4, DB5, DB6, DB7);

const byte ROWS = 4; // four rows
const byte COLS = 4; // four columns

// Key definitions
char hexaKeys[ROWS][COLS] = { {'1', '2', '3', 'A'},
                              {'4', '5', '6', 'B'},
                              {'7', '8', '9', 'C'},
                              {'*', '0', '#', 'D'} };

// Arduino digital pin connection
byte rowPins[ROWS] = {K5, K6, K7, K8}; 
byte colPins[COLS] = {K1, K2, K3, K4};

// Creating the keypad
Keypad keyp = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

// Test parameters
int cycleCount;
float trvSpeed;
float trvDistance;
float maxRes;
float maxInForce;
float maxRemForce;
float maxInSpeed;
float dwellTime;

int keyCount = 0; // Count keys pressed

void setup() 
{ 
  // Initialize top and bottom LCD
  top.begin(40,2);
  bot.begin(40,2);

  // Welcome prompt
  top.setCursor(0,10);
  top.print(" Welcome to C.R.A.F.T.!");
  top.setCursor(0,1);
  top.print(" Connection Resistance And Force Tester ");

  // Menu prompt
  bot.setCursor(0,0);
  bot.print("  <A> Begin test     <B> Test Cycle ");
  bot.setCursor(0,11);
  bot.print("<C> Exit C.R.A.F.T.");

  // Keypad baud rate
  Serial.begin(9600);
}

void loop() 
{
  char key0 = keyp.getKey();

  if(key0 == '*'){
    setup();
  }

  if(keyCount == 0)
  {
    if(key0 == 'A'){ // Begin Test

      if(key0 == '*'){
        setup();
      }

      keyCount++; // 1, The intention is such that since keycount =/= 0, you can't jump menu options after picking one
      top.clear();
      bot.clear();

      top.setCursor(0,0);
      top.print("Set test parameters");
      top.setCursor(0,1);

      // Cycle count
      top.setCursor(0,1);
      top.print("Cycle count:");
      bot.setCursor(0,0);
      //bot.print(keyp); // Intention is that user types string that is converted and stored in corresponding variable 
      bot.setCursor(0,0);
      bot.print("<#> Next <*> MENU");
      
      if(key0 = '#'){ // Travel speed
        top.setCursor(0,1);
        top.print("Travel Speed (mm/s):");
        bot.setCursor(0,0);
        //bot.print(keyp);
        bot.setCursor(0,0);
        bot.print("<#> Next <*> MENU");

        if(key0 = '#'){ // Travel distance
          top.setCursor(0,1);
          top.print("Travel distance (mm):");
          bot.setCursor(0,0);
          //top.print(keyp);
          bot.setCursor(0,0);
          bot.print("<#> Next <*> MENU");
    
          if(key0 = '#'){ // Max resistance
            top.setCursor(0,1);
            top.print("Resistance (mOhms):");
            bot.setCursor(0,0);
            //top.print(keyp);
            bot.setCursor(0,0);
            bot.print("<#> Next <*> MENU");
          
            if(key0 = '#'){ // Max insertion force
              top.setCursor(0,1);
              top.print("Max insertion force (kg):");
              bot.setCursor(0,0);
              //top.print(keyp);
              bot.setCursor(0,0);
              bot.print("<#> Next <*> MENU");
            
              if(key0 = '#'){ // Max removal force
                top.setCursor(0,1);
                top.print("Max removal force (kg):");
                bot.setCursor(0,0);
                //top.print(keyp);
                bot.setCursor(0,0);
                bot.print("<#> Next <*> MENU");
              
                if(key0 = '#'){ // Max insertion speed
                  top.setCursor(0,1);
                  top.print("Max insertion speed (sec):");
                  bot.setCursor(0,0);
                  //top.print(keyp);
                  bot.setCursor(0,0);
                  bot.print("<#> Next <*> MENU");

                  if(key0 = '#'){ // Dwell time
                    top.setCursor(0,1);
                    top.print("Dwell time (sec):");
                    bot.setCursor(0,0);
                    //top.print(keyp);
                    bot.setCursor(0,0);
                    bot.print("<#> Next <*> MENU");

                    if(key0 = '#'){
                      // And this is the part where all the values would get sent to the various functions you guys wrote and stuff
                      // I'm very dissatisfied with this and I'm pretty sure it doesn't work the way I want it to .__.
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
 
   if(key0 == 'B'){ // Test Cycle
    
      top.clear();
      bot.clear();

      /* Should be similar to the way 'Begin test' works except all values are predetermined */
    }
    if(key0 == 'C'){ // Exit
    
      top.clear();
      bot.clear();
    
      top.setCursor(0,0);
      top.print("     Are you sure you want to exit?     ");
      bot.setCursor(11,0);
      bot.print("<A> Yes");
      bot.setCursor(24,0);
      bot.print("<B> No");
    }
}

/*  ------------------------- Code INFO ----------------------------

    Title: DrinkMachine
    Authour: Jack Maro
    Description: "Arduino Program for Jack's drinkmachine"
    Files: A_Drinkmachine (Variables and libraries), B_Setup, C_Loop, D_DisplayFunctions, I_ISR


    --------------------- General Function ------------------------

    Startup: Steppermotor calibrated & Main Menu: Pumps

    Main Menu - Two modes: Drinks/Pumps... (switch with blue btn)
    
      1. Drinks
        - red btn = change page... 10 drinks/page
        - scroll = select drink on page
        - click =  DrinkLauncher(Selected Drink Number)
        
      2. Pumps (Admin Mode)
        - red btn = change loaded drink on selected pump
        - scroll = select pump or drink size
        - click = go to DrinkPourer(Selected Drink Number) or DrinkSize()

    More to be made:

    - After each drink calibrate stepper again.
    - LED's
    - Buzzer sounds (haptic feedback)
    - Loading backgrounds
    - Recipe check on startup

   --------------------- Recipe Explanation -----------------------

    0001Vodka           (100%0001)(000%0000)(000%0000)(000%0000)(000%0000)(000%0000)(000%0000)(000%0000)
    0002Apelsin Saft    [000%0000](000%0000)(000%0000)(000%0000)(000%0000)(000%0000)(000%0000)(000%0000)
    0003Vodka Juice     [015%0001](085%0002)(000%0000)(000%0000)(000%0000)(000%0000)(000%0000)(000%0000)

    A recipe looks like this above. We see the first 3 drinks on the list 1. Vodka, 2. Apelsin Saft & 3. Vodka Juice.
    It consist of a number, the name and then 8 ingredients.
    Ingredients are marked with [] if its a strong ingredient -like vodka... or () with soft ingredients -like juice...
    First three numbers of an ingredient indicates procentage, last four is the number of the ingredient.

    Recipe rules:
    1. Name must consist of ONLY these characters (SPACE, A-Z, a-z, 0-9)
    2. Structure has to be as above
    - Name must be 16 chars long (fill rest with SPACE)
    - Delete empty space behind... Each DRINK-RECIPE must consist of 102 chars (including '\r' and '\n')
    - procent numbers must be 3 chars and drink numbers 4 chars
    3. The total procentage of ingreedients should be 100% or 0%.
    4. Ingredients must have a valid-corresponding drink.

    
*/
// -------------------- Libraries -------------------------

#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include <SPI.h>
#include <SD.h>
#include <ctype.h>

// ----------------- Pin Variables ------------------------

// TFT Display
#define TFT_DC 9
#define TFT_CS 10

//Encoder
#define ENCODER_A 19 //Encoder's A output (INT 2)
#define ENCODER_B 22 //Encoder's B output
#define ENCODER_PRESS 18 // (INT 3)

//Buttons
#define RED_BTN 20 //(INT 1)
#define BLUE_BTN 21 //(INT 0)
#define MODE_BTN 42

//Motors
#define MOTOR_PWM 45
#define MOTOR_DIR 47
#define STEPPER_STEP 48
#define STEPPER_ENABLE 49
#define STEPPER_HOME 2 //(INT 4)

//Others
#define LED_PREPARE_PIN 27
#define BUZZ_PIN 44

// ------------------- SD Variables ---------------------------

File recipesFile;
const int SD_CC = 4;
char byteHolder = ' ';

// ----------------- Display Variables ------------------------

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
uint8_t tftWidth;
uint16_t tftHeight;

// ----------------- Encoder Variables ------------------------

volatile bool encoderTurned = false, encoderPressed = false;
volatile bool EncoderNewA = 0, EncoderNewB = 0, EncoderPreA = 0, EncoderPreB = 0;
volatile int encoderValue = 0, MaxEncoderValue = 20, MinEncoderValue = 0;
volatile bool encoderTriedToPassMin;
volatile bool encoderTriedToPassMax;
byte encoderSensitivity = 2; //Encoder increments or decrements by 2 each tick. A sensitivity of "2" would make the menu-options have 2 encoderValues. Ex: option 2 would have encoderVal 3 & 4.

// ----------------- Pump & Motor Variables ------------------------

bool pumpRunning = false;
uint16_t amountPoured = 0; // Used to keep track of how many centiliters there has been pumped of the currently selected loaded-drink.
volatile byte currentPumpSelected = 1; // Used in multiple menues. PumpMenu & submenues: option 1 -> 9 ... Pump 1 -> calibrate
const byte stepsInFullRotation = 200;
volatile bool stepperIsHome = false;
byte activePump = 0; // 0 = none

// ----------------- Time Variables ------------------------

volatile unsigned int buttonDelay = 400; //millisecconds
volatile unsigned long lastButtonPressTime = 0; //millisecconds
unsigned int stepDelay = 10; //millisecconds
unsigned long pumpStartTime = 0;
unsigned long pumpTotalRunTime = 0;

uint16_t pumpsCentiliterTime[] = {800, 800, 800, 800, 800, 800, 800, 800}; //Default 


// ----------------- Structure Variables ------------------------

enum Menus
{
  DRINK_MENU,
  PUMP_MENU,
  PUMP_MENU_SIZE_SELECTION,
  POUR_MENU,
  SIZE_MENU,
  STRENGTH_MENU,
  LOADED_DRINK_MENU,
  MIX_MENU,
  CALIBRATION_MENU
};

volatile bool adminMode = false;
volatile enum Menus currentMenu = DRINK_MENU; //start with the pump menu
volatile bool changeSearchModeFlag = false;
bool alphabeticalSearch = false;
byte numberOfSelectedSearchChar = 0; //0-25 ... a-z
unsigned int currentPage = 1;
volatile bool returnFromDrinkLaucher = false;
volatile bool updateQuickSizeSelected = false;
volatile bool calSizeStart = false;
volatile bool calSizeActive = false;
volatile bool drinkAccepted = false;

// ----------------- General Variables ------------------------

const byte numberOfPumps = 8;
const byte numberOfDrinkSizes = 3;
const byte numberOfSavedDrinks = 10;

String drinksLoaded[numberOfPumps] = {"", "", "", "", "", "", "", ""}; //Max name size: 16 - "" is none
unsigned int drinksLoadedNumbers[numberOfPumps] = {0, 0, 0, 0, 0, 0, 0, 0}; //0 is none
byte numberOfDrinksLoaded = 0;
byte drinkSizes[numberOfDrinkSizes] = {10, 20, 40}; //centiliter [cl]
byte selectedQuickSize = 0;
const char alphabet[26] = {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};

String savedDrinkNames[numberOfSavedDrinks] = {"", "", "", "", "", "", "", ""}; //Always sorted LOW->HIGH
volatile unsigned int savedDrinkNumbers[numberOfSavedDrinks] = {0,0,0,0,0,0,0,0}; //Follows savedDrinkNames
unsigned int highestSavedDrink = 0; //Should be the Last value in "savedDrinkNumbers"
unsigned int lowestSavedDrink = 0; //Should be the First value in "savedDrinkNumbers"
byte numberOfMakeableDrinksToGet;

byte currentDrinkOptionSelected;
byte currentLoadedDrinkOptionSelected;
// ----------------- MISC ------------------------

volatile bool runNextLight = false;
byte lightChar = 0;

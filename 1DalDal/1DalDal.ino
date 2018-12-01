#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <ESP32_Servo.h>

class Encoder;

////////////////////////////////////////////////////////////////////////
// Pin Definition

#define ENCODER_1_CLK 32
#define ENCODER_1_DT 21
#define ENCODER_1_SW 25

#define ENCODER_2_CLK 33
#define ENCODER_2_DT 19
#define ENCODER_2_SW 26
                
#define ENCODER_3_CLK 4
#define ENCODER_3_DT 18
#define ENCODER_3_SW 14

#define ENCODER_4_CLK 34
#define ENCODER_4_DT 2
#define ENCODER_4_SW 35

//WiFiServer server(80);

////////////////////////////////////////////////////////////////////////
// LCD Display

LiquidCrystal_I2C lcd(0x27,16,2);//set the LCD address to 0x27 for a 16 chars and 2 line display

void displayStrings(String text1, String text2, LiquidCrystal_I2C& lcd) {

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(text1);
  lcd.setCursor(0,1);
  lcd.print(text2);
}


////////////////////////////////////////////////////////////////////////
// LED diods

int LED1 = 23; 
int LED2 = 27;
int LED3 = 22;
int LED4 = 15 ;
int LED5 = 5;

int freq = 5000;

int led1Channel = 0;
int led2Channel = 1;
int led3Channel = 2;
int led4Channel = 3;
int led5Channel = 4;

int resolution = 8;

int dutyCycle = 0;


const int minX = 0;
const int maxX = 180;
const int minY = 0;
const int maxY = 90;
const int minZ = 0;
const int maxZ = 90;
const int minAngle = 0;
const int maxAngle = 180;

const int minPulseWidth = 500;
const int maxPulseWidth = 2500;


////////////////////////////////////////////////////////////////////////
// Servos

Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;


////////////////////////////////////////////////////////////////////////
// State

#define ST_INITIAL          0
#define ST_MAIN_MENU        1

#define ST_RESET_POSITION   10
#define ST_PLAY_PROGRAM     20
#define ST_CREATE_PROGRAM   30
#define ST_CREATE_PROGRAM_CONFIRM_STEP   31
#define ST_MANUAL_MODE      40
#define ST_BLUETOOTH_MODE   50
#define ST_DEMO             60

#define MENU_RESET_POSITION 0
#define MENU_PLAY_PROGRAM   1
#define MENU_CREATE_PROGRAM 2
#define MENU_MANUAL_MODE    3
#define MENU_BLUETOOTH_MODE 4
#define MENU_DEMO           5

int currentState = ST_INITIAL;
int selectedMenuItem = 0;
int menuOffset = 0;

int currentX = 90;
int currentY = 90;
int currentZ = 90;
int currentAngle = 90;
bool currentlyPumpEnabled = false;

int lastX = 90;
int lastY = 90;
int lastZ = 90;
int lastAngle = 90;
bool beforePumpEnabled = false;

long currentXUpdate = 0;
long currentYUpdate = 0;
long currentZUpdate = 0;
long currentAngleUpdate = 0;

bool refreshDisplay = true;

////////////////////////////////////////////////////////////////////////
// Program


#define STEP_INITIAL 0
#define STEP_PAUSE_BEFORE 1
#define STEP_MOVEMENT 2
#define STEP_PAUSE_AFTER 3

const int maxStepCount = 20;

struct ProgramStep {
  int x;
  int y;
  int z;
  int angle;
  bool pump;
  int duration;
  int pauseBefore;
  int pauseAfter;
};

struct ProgramStep program[maxStepCount];

int programStepCount = 0;
int currentStep = -1;
int currentStepPhase = STEP_INITIAL;
unsigned long currentStepBegin = 0;

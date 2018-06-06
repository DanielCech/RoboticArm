#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>

class Encoder;

////////////////////////////////////////////////////////////////////////
// Pin Definition

#define ENCODER_1_CLK 32
#define ENCODER_1_DT 21
#define ENCODER_1_SW 25

#define ENCODER_2_CLK 33
#define ENCODER_2_DT 19
#define ENCODER_2_SW 26
                
// Puvodni hodnota, co funguje
//#define ENCODER_3_CLK 27
//#define ENCODER_3_DT 22
//#define ENCODER_3_SW 14

#define ENCODER_3_CLK 4
#define ENCODER_3_DT 18
#define ENCODER_3_SW 14

#define ENCODER_4_CLK 34
#define ENCODER_4_DT 2//34
#define ENCODER_4_SW 35//35

WiFiServer server(80);

////////////////////////////////////////////////////////////////////////
// LCD Display

LiquidCrystal_I2C lcd(0x27,16,2);//set the LCD address to 0x27 for a 16 chars and 2 line display


////////////////////////////////////////////////////////////////////////
// LED diods

int LED1 = 23; //23
int LED2 = 27; //34
int LED3 = 22;
int LED4 = 15 ;
int LED5 = 5;

int freq = 5000;
int ledChannel = 0;
int resolution = 8;

int dutyCycle = 0;


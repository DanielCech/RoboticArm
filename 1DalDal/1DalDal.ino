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

#define ENCODER_3_CLK 27
#define ENCODER_3_DT 22
#define ENCODER_3_SW 14

#define ENCODER_4_CLK 4
#define ENCODER_4_DT 18
#define ENCODER_4_SW 15

WiFiServer server(80);




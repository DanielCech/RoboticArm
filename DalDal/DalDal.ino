#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>

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

////////////////////////////////////////////////////////////////////////
// Classes

class Encoder {
  int encoderIndex;
  
  int clkState = 1;
  int clkPreviousState = 1;
  int dtState = 1;
  int buttonState = 1;
  int buttonPreviousState = 1;

  public:
    Encoder(int index) {
    encoderIndex = index;
  }

  void readEncoder(int clk, int dt, int sw) {
    clkState = digitalRead(clk);
    dtState = digitalRead(dt);
    buttonState = digitalRead(sw);

    if (buttonState && !buttonPreviousState) {
      Serial.printf("Encoder %d: button", encoderIndex);
      Serial.println("");
    }

    if (clkState < clkPreviousState) {
      if (clkState < dtState) {
        Serial.printf("Encoder %d: down", encoderIndex);
        Serial.println("");
      }
      else{
        Serial.printf("Encoder %d: up", encoderIndex);
        Serial.println("");
      }  
    }

    clkPreviousState = clkState;
    buttonPreviousState = buttonState;
  }  
};


class WebServer {
public:

  const char* ssid     = "Bublifuk";
  const char* password = "775116855";

  void connectToWifi() {
     WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    Serial.println("Place this IP address into a browser window");
  }

  void processClients(WiFiServer& server) {
    WiFiClient client = server.available();   // listen for incoming clients

    if (client) {                             
      Serial.println("new client");          
      String currentLine = "";                   // make a String to hold incoming data from the client
      while (client.connected()) {            
        if (client.available()) {                // if there's client data
          char c = client.read();                // read a byte
            if (c == '\n') {                     // check for newline character, 
            if (currentLine.length() == 0) {     // if line is blank it means its the end of the client HTTP request
              client.println("<!DOCTYPE html>"); // open wrap the web page
              client.print("<html><head><meta name='viewport' content='initial-scale=1.0'><meta charset='utf-8'><style>#map {height: 100%;}html, body {height: 100%;margin: 0;padding: 0;}</style></head>");
              client.print("<body><h1>ESP32 WebServer Stepper Motor's</h1>");
          // Gui buttons start here
                  client.print("<input type=image style=width:33%;height:20% src='http://myrobotlab.org/sites/default/files/users/user25images/ButtonBlueLong.png'onmousedown=location.href='/dec1' >");
                  client.print("<input type=image style=width:33%;height:20% src='http://myrobotlab.org/sites/default/files/users/user25images/nixiesmall1.png'>");
                  client.print("<input type=image style=width:33%;height:20% src='http://myrobotlab.org/sites/default/files/users/user25images/ButtonOrangeLong.png'onmousedown=location.href='/inc1' >");
                     
                  client.print("<input type=image style=width:33%;height:20% src='http://myrobotlab.org/sites/default/files/users/user25images/ButtonPinkLong.png'onmousedown=location.href='/dec2' >");
                  client.print("<input type=image style=width:33%;height:20% src='http://myrobotlab.org/sites/default/files/users/user25images/nixiesmall2.png'>");
                  client.print("<input type=image style=width:33%;height:20% src='http://myrobotlab.org/sites/default/files/users/user25images/ButtonGreenLong.png'onmousedown=location.href='/inc2' >"); 
  
                  client.print("<input type=image style=width:33%;height:20% src='http://myrobotlab.org/sites/default/files/users/user25images/ButtonTurqLong.png'onmousedown=location.href='/dec3' >");
                  client.print("<input type=image style=width:33%;height:20% src='http://myrobotlab.org/sites/default/files/users/user25images/nixiesmall3.png'>");
                  client.print("<input type=image style=width:33%;height:20% src='http://myrobotlab.org/sites/default/files/users/user25images/ButtonPurpleLong.png'onmousedown=location.href='/inc3' >"); 
                  
                  client.print("<input type=image style=width:100%;height:20% src='http://myrobotlab.org/sites/default/files/users/user25images/ButtonRedLong.png'onmousedown=location.href='/stopall' >");
                  client.print("</body></html>"); // close wrap the web page
              
              client.println(); // The HTTP response ends with an extra blank line:
            
              break;  // break out of the while loop:
            } else {    // if you got a newline, then clear currentLine:
              currentLine = "";
            }
           } else if (c != '\r') {  // if you got anything else but a carriage return character,
            currentLine += c;       // add it to the end of the currentLine
          }
  
          // Check to see if the client request was "GET /H" or "GET /L":
          if (currentLine.endsWith("GET /dec1")) {digitalWrite(16, HIGH);
                for (int i=0; i <= 43; i++){ digitalWrite(17, HIGH);delay(10);digitalWrite(17,LOW );delay(10); }
          }
           if (currentLine.endsWith("GET /inc1")) {digitalWrite(16, LOW);
                for (int i=0; i <= 43; i++){ digitalWrite(17, HIGH);delay(10);digitalWrite(17,LOW );delay(10); }
          }
           if (currentLine.endsWith("GET /dec2")) {digitalWrite(16, HIGH);
                for (int i=0; i <= 43; i++){ digitalWrite(18, HIGH);delay(10);digitalWrite(18,LOW );delay(10); }
          }
           if (currentLine.endsWith("GET /inc2")) {digitalWrite(16, LOW);
                for (int i=0; i <= 43; i++){ digitalWrite(18, HIGH);delay(10);digitalWrite(18,LOW );delay(10); }
          }
           if (currentLine.endsWith("GET /dec3")) {digitalWrite(16, HIGH);
                for (int i=0; i <= 43; i++){ digitalWrite(19, HIGH);delay(10);digitalWrite(19,LOW );delay(10); }
          }
           if (currentLine.endsWith("GET /inc3")) {digitalWrite(16, LOW);
                for (int i=0; i <= 43; i++){ digitalWrite(19, HIGH);delay(10);digitalWrite(19,LOW );delay(10); }
          }
           if (currentLine.endsWith("GET /stopall")) {digitalWrite(16, HIGH);
            }
          }
        }
      }
  }
  
};

////////////////////////////////////////////////////////////////////////
// Wifi Setup

WebServer webServer = WebServer();

////////////////////////////////////////////////////////////////////////
// Rotary Encoders

Encoder firstEncoder = Encoder(1);
Encoder secondEncoder = Encoder(2);
Encoder thirdEncoder = Encoder(3);
Encoder fourthEncoder = Encoder(4);

////////////////////////////////////////////////////////////////////////
// LCD Display

LiquidCrystal_I2C lcd(0x27,16,2);//set the LCD address to 0x27 for a 16 chars and 2 line display

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  pinMode(ENCODER_1_CLK, INPUT);
  pinMode(ENCODER_1_DT, INPUT);
  pinMode(ENCODER_1_SW, INPUT);

  pinMode(ENCODER_2_CLK, INPUT);
  pinMode(ENCODER_2_DT, INPUT);
  pinMode(ENCODER_2_SW, INPUT);

  pinMode(ENCODER_3_CLK, INPUT);
  pinMode(ENCODER_3_DT, INPUT);
  pinMode(ENCODER_3_SW, INPUT);

  pinMode(ENCODER_4_CLK, INPUT);
  pinMode(ENCODER_4_DT, INPUT);
  pinMode(ENCODER_4_SW, INPUT);

  lcd.begin(12, 13);// initialize the lcd with SDA and SCL pins
//  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Tucnak Buclik");
  lcd.setCursor(0,1);
  lcd.print("tlaci vajcko");

  webServer.connectToWifi();
  server.begin();
}




void loop() {
  // put your main code here, to run repeatedly:

  firstEncoder.readEncoder(ENCODER_1_CLK, ENCODER_1_DT, ENCODER_1_SW);
  secondEncoder.readEncoder(ENCODER_2_CLK, ENCODER_2_DT, ENCODER_2_SW);
  thirdEncoder.readEncoder(ENCODER_3_CLK, ENCODER_3_DT, ENCODER_3_SW);
  fourthEncoder.readEncoder(ENCODER_4_CLK, ENCODER_4_DT, ENCODER_4_SW);

  webServer.processClients(server);
  
}

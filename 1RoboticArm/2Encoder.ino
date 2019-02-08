class Encoder {
public:
  int encoderIndex;
  
  int clkState = 1;
  int clkPreviousState = 1;
  int dtState = 1;
  int buttonState = 1;
  int buttonPreviousState = 1;

  bool buttonPressed = false;
  int direction = 0;
  int previousDirection = 0;
  long backwardStepUpdate;
  long forwardStepUpdate;


  Encoder(int index) {
    encoderIndex = index;
  }

  void readEncoder(int clk, int dt, int sw) {
    clkState = digitalRead(clk);
    dtState = digitalRead(dt);
    buttonState = digitalRead(sw);

    buttonPressed = buttonState && !buttonPreviousState;
    if (buttonPressed) {
      Serial.printf("Encoder %d: button", encoderIndex);
      Serial.println("");
    }

    if (clkState < clkPreviousState) {
      if (clkState < dtState) {
        Serial.printf("Encoder %d: down", encoderIndex);
        Serial.println("");
        direction = -1;
        backwardStepUpdate = millis();
      }
      else{
        Serial.printf("Encoder %d: up", encoderIndex);
        Serial.println("");
        direction = 1;
        forwardStepUpdate = millis();
      }

      long now = millis();
      if ((direction == -1) && (now - forwardStepUpdate < 200)) {
        direction = 0;
      } 
      else if ((direction == 1) && (now - backwardStepUpdate < 200)) {
        direction = 0;
      }
 
      previousDirection = direction;
    }
    else {
      direction = 0;
    }

    clkPreviousState = clkState;
    buttonPreviousState = buttonState;
  }  
};

////////////////////////////////////////////////////////////////////////
// Rotary Encoders

Encoder firstEncoder = Encoder(1);
Encoder secondEncoder = Encoder(2);
Encoder thirdEncoder = Encoder(3);
Encoder fourthEncoder = Encoder(4);

bool encoder4normalDirection = true;


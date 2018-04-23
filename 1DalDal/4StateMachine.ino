#define ST_INITIAL          0
#define ST_MAIN_MENU        1

#define ST_PLAY_PROGRAM     10
#define ST_CREATE_PROGRAM   20
#define ST_MANUAL_MODE      30
#define ST_WIFI_MODE        40
#define ST_BLUETOOTH_MODE   50
#define ST_DEMO             60

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

class StateMachine {
public:

  int previousState = -1;
  int currentState = ST_INITIAL;
  int selectedMenuItem = 0;
  int previousSelectedMenuItem = -1;
  int menuOffset = 0;
  int previousMenuOffset = -1;

  String menuItems[6] = {"Play Program", "Create Program", "Manual Mode", "WiFi Mode", "Bluetooth Mode", "Demo"};

  void displayStrings(String text1, String text2, LiquidCrystal_I2C& lcd) {
//    Serial.println(text1 + " : " + text2);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(text1);
    lcd.setCursor(0,1);
    lcd.print(text2);
  }

  void processState(Encoder& encoder1, Encoder& encoder2, Encoder& encoder3, Encoder& encoder4, LiquidCrystal_I2C& lcd) {

    switch (currentState) {
    case ST_INITIAL:
      if (currentState != previousState) {
        displayStrings("RoboticArm 0.1", "JM-DC-01", lcd);
        previousState = currentState;
      }
     
      if (encoder1.buttonPressed) {
        previousState = currentState;
        currentState = ST_MAIN_MENU;
        Serial.println("Button Pressed");
        return;
      
      }
      break;

    case ST_MAIN_MENU:
//      Serial.printf("selectedMenuItem %d menuOffset %d", selectedMenuItem, menuOffset);
//      Serial.println("");
      
      if ((selectedMenuItem != previousSelectedMenuItem) || (menuOffset != previousMenuOffset)) {
        String first = ((selectedMenuItem == menuOffset) ? "> ": "  ") + menuItems[menuOffset];
        String second = ((selectedMenuItem == menuOffset + 1) ? "> ": "  ") + menuItems[menuOffset + 1];
        displayStrings(first, second, lcd);
        previousSelectedMenuItem = selectedMenuItem;
        previousMenuOffset = menuOffset;
      }

      if (encoder1.direction < 0) {
        previousSelectedMenuItem = selectedMenuItem;
        selectedMenuItem = MAX(selectedMenuItem - 1, 0);
      }
      if (encoder1.direction > 0) {
        previousSelectedMenuItem = selectedMenuItem;
        selectedMenuItem = MIN(selectedMenuItem + 1, 5);
      }

      if (selectedMenuItem > menuOffset + 1) {
        previousMenuOffset = menuOffset;
        menuOffset = selectedMenuItem - 1;
      }
      else if (selectedMenuItem < menuOffset) {
        previousMenuOffset = menuOffset;
        menuOffset = selectedMenuItem;
      }

      break;
      
      
    case ST_CREATE_PROGRAM:
      break;
      
    case ST_MANUAL_MODE:
      break;
      
    case ST_WIFI_MODE:
      break;
      
    case ST_BLUETOOTH_MODE:
      break;
      
    case ST_DEMO:
      break;

      
    }    
  }

  

  


  
};


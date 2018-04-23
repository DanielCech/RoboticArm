#define ST_INITIAL          0
#define ST_MAIN_MENU        1

#define ST_PLAY_PROGRAM     10
#define ST_CREATE_PROGRAM   20
#define ST_MANUAL_MODE      30
#define ST_WIFI_MODE        40
#define ST_DEMO             50

#define MENU_PLAY_PROGRAM   0
#define MENU_CREATE_PROGRAM 1
#define MENU_MANUAL_MODE    2
#define MENU_WIFI_MODE      3
#define MENU_DEMO           4


#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

class StateMachine {
public:

  int previousState = -1;
  int currentState = ST_INITIAL;
  int selectedMenuItem = 0;
//  int previousSelectedMenuItem = -1;
  int menuOffset = 0;
//  int previousMenuOffset = -1;

  bool refreshDisplay = true;

  bool wifiMode = false;
  bool bluetoothMode = false;
  
  String menuItems[6] = {"Play Program", "Create Program", "Manual Mode", "WiFi Mode", "Demo"};

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
      if (refreshDisplay) {
        displayStrings("RoboticArm 0.1", "JM-DC-01", lcd);
        refreshDisplay = false;
      }
     
      if (encoder1.buttonPressed) {
        currentState = ST_MAIN_MENU;
        refreshDisplay = true;
        Serial.println("Button Pressed");
        delay(200);
        return;
      }
      
      break;

    case ST_MAIN_MENU:

      if (encoder1.buttonPressed) {
        switch (selectedMenuItem) {
          case MENU_PLAY_PROGRAM:

          case MENU_CREATE_PROGRAM:
            break;

          case MENU_MANUAL_MODE:
            break;

          case MENU_WIFI_MODE:
            currentState = ST_WIFI_MODE;
            refreshDisplay = true;
            delay(200);
            return;

          case MENU_DEMO:
            break;
        }
      }

      if (encoder2.buttonPressed) {
        currentState = ST_INITIAL;
        refreshDisplay = true;
        return;
      }
      
      if (refreshDisplay) {
        String first = ((selectedMenuItem == menuOffset) ? "> ": "  ") + menuItems[menuOffset];
        String second = ((selectedMenuItem == menuOffset + 1) ? "> ": "  ") + menuItems[menuOffset + 1];
        displayStrings(first, second, lcd);
        refreshDisplay = false;
      }

      if (encoder1.direction < 0) {
        selectedMenuItem = MAX(selectedMenuItem - 1, 0);
        refreshDisplay = true;
      }
      if (encoder1.direction > 0) {
        selectedMenuItem = MIN(selectedMenuItem + 1, 5);
        refreshDisplay = true;
      }

      if (selectedMenuItem > menuOffset + 1) {
        menuOffset = selectedMenuItem - 1;
        refreshDisplay = true;
      }
      else if (selectedMenuItem < menuOffset) {
        menuOffset = selectedMenuItem;
        refreshDisplay = true;
      }

      break;
      
      
    case ST_CREATE_PROGRAM:
      break;
      
    case ST_MANUAL_MODE:
      break;
      
    case ST_WIFI_MODE: {
        String line = wifiMode ? " [On]     Off" : "  On     [Off]";
      
        if (refreshDisplay) {
          displayStrings("Wifi Mode", line, lcd);
          refreshDisplay = false;
        }
  
        // Return to main menu
        if (encoder2.buttonPressed) {
          currentState = ST_MAIN_MENU;
          refreshDisplay = true;
          return;
        }
  
        if (encoder1.direction != 0) {
          wifiMode = !wifiMode;
          refreshDisplay = true;
        }
        
        break;
      }
      
    case ST_DEMO:
      break;

      
    }    
  }

  

  


  
};


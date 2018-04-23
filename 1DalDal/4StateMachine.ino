#define ST_INITIAL          0
#define ST_MAIN_MENU        1

#define ST_PLAY_PROGRAM     10
#define ST_CREATE_PROGRAM   20
#define ST_MANUAL_MODE      30
#define ST_WIFI_MODE        40
#define ST_WIFI_ACTIVE      45
#define ST_DEMO             50

#define MENU_PLAY_PROGRAM   0
#define MENU_CREATE_PROGRAM 1
#define MENU_MANUAL_MODE    2
#define MENU_WIFI_MODE      3
#define MENU_DEMO           4


#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

int currentState = ST_INITIAL;
int selectedMenuItem = 0;
int menuOffset = 0;

bool refreshDisplay = true;

bool wifiMode = false;
bool bluetoothMode = false;

String menuItems[6] = {"Play Program", "Create Program", "Manual Mode", "WiFi Mode", "Demo"};

void displayStrings(String text1, String text2, LiquidCrystal_I2C& lcd) {

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(text1);
  lcd.setCursor(0,1);
  lcd.print(text2);
}

void processState() {

  switch (currentState) {
    
  case ST_INITIAL: {
      Serial.println("Initial state");
    
      if (refreshDisplay) {
        displayStrings("RoboticArm 0.1", "JM-DC-01", lcd);
        refreshDisplay = false;
      }
     
      if (firstEncoder.buttonPressed) {
        currentState = ST_MAIN_MENU;
        refreshDisplay = true;
        Serial.println("Button Pressed");
        delay(200);
        return;
      }
      
      return;
    }

  case ST_MAIN_MENU: {

      Serial.println("Main menu");

      if (firstEncoder.buttonPressed) {
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
  
      if (secondEncoder.buttonPressed) {
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
  
      if (firstEncoder.direction < 0) {
        selectedMenuItem = MAX(selectedMenuItem - 1, 0);
        refreshDisplay = true;
      }
      if (firstEncoder.direction > 0) {
        selectedMenuItem = MIN(selectedMenuItem + 1, 4);
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
  
      return;
    }

  case ST_PLAY_PROGRAM: {
      Serial.println("Play program");
      return;
    }
    
  case ST_CREATE_PROGRAM: {
      Serial.println("Create program");
      return;
    }
    
  case ST_MANUAL_MODE: {
      Serial.println("Manual mode");
      return;
    }
    
  case ST_WIFI_MODE: {
      Serial.println("Wifi mode");
      String line = wifiMode ? " [On]     Off" : "  On     [Off]";
    
      if (refreshDisplay) {
        displayStrings("Wifi Mode", line, lcd);
        refreshDisplay = false;
      }

      // Return to main menu
      if (secondEncoder.buttonPressed) {
        currentState = ST_MAIN_MENU;
        refreshDisplay = true;
        delay(200);
        return;
      }

      if (firstEncoder.buttonPressed) {
        if (wifiMode) {
          currentState = ST_WIFI_ACTIVE;
        }
        else {
          server.end();
          currentState = ST_MAIN_MENU;
        }
        
        refreshDisplay = true;
        delay(200);
        return;
        
      }

      if (firstEncoder.direction != 0) {
        wifiMode = !wifiMode;
        refreshDisplay = true;
      }
      
      break;
    }

  case ST_WIFI_ACTIVE: {
      Serial.println("Wifi Active");
  
      if (refreshDisplay) {
          displayStrings("Wifi: active", "...", lcd);
          webServer.connectToWifi();
          server.begin();
  
          IPAddress address = WiFi.localIP();
  
          displayStrings("Wifi: active", address.toString(), lcd);
          refreshDisplay = false;
       }
  
      // Return to main menu
      if (firstEncoder.buttonPressed || secondEncoder.buttonPressed) {
          currentState = ST_MAIN_MENU;
          refreshDisplay = true;
          delay(200);
          return;
      }
  
      return;
    }
    
    
  case ST_DEMO: {
      Serial.println("Demo");
      return;
    }
    
  }    
}



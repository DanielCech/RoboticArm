#define ST_INITIAL          0
#define ST_MAIN_MENU        1

#define ST_PLAY_PROGRAM     10
#define ST_CREATE_PROGRAM   20
#define ST_MANUAL_MODE      30
#define ST_BLUETOOTH_MODE   40
#define ST_DEMO             50

#define MENU_PLAY_PROGRAM   0
#define MENU_CREATE_PROGRAM 1
#define MENU_MANUAL_MODE    2
#define MENU_BLUETOOTH_MODE 3
#define MENU_DEMO           4


#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

int currentState = ST_INITIAL;
int selectedMenuItem = 0;
int menuOffset = 0;

bool refreshDisplay = true;

bool bluetoothModeSelection = false;
bool bluetoothModeEnabled = false;

int currentX = 0;
int currentY = 0;
int currentZ = 0;
int currentAngle = 0;
bool currentlyPumpEnabled = false;
bool beforePumpEnabled = false;

String menuItems[6] = {"Play Program", "Create Program", "Manual Mode", "Bluetooth Mode", "Demo"};

void displayStrings(String text1, String text2, LiquidCrystal_I2C& lcd) {

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(text1);
  lcd.setCursor(0,1);
  lcd.print(text2);
}

void initialState() {

  //Serial.println("Initial state");

  if (refreshDisplay) {
    displayStrings("RoboticArm 0.2", "JVDA-01", lcd);
    refreshDisplay = false;
  }
 
  if (firstEncoder.buttonPressed) {
    currentState = ST_MAIN_MENU;
    refreshDisplay = true;
    Serial.println("Button Pressed");
    delay(200);
  }
}


void mainMenu() {
  Serial.println("Main menu");

  if (firstEncoder.buttonPressed) {
    switch (selectedMenuItem) {
      case MENU_PLAY_PROGRAM:

      case MENU_CREATE_PROGRAM:
        break;

      case MENU_MANUAL_MODE:
        currentState = ST_MANUAL_MODE;
        refreshDisplay = true;
        delay(200);
        return;

       case MENU_BLUETOOTH_MODE:
        currentState = ST_BLUETOOTH_MODE;
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
}

void playProgram() {
  Serial.println("Play program");
}

void createProgram() {
  Serial.println("Create program");
}

void manualMode() {
  Serial.println("Manual mode");

  if (refreshDisplay) {

    char firstLine[20];
    sprintf(firstLine, "Angle:%-3d Pump:%s", currentAngle, currentlyPumpEnabled ? "1" : "0");

    char secondLine[20];
    sprintf(secondLine, "X:%-3d Y:%-3d Z:%-3d", currentX, currentY, currentZ);

    displayStrings(String(firstLine), String(secondLine), lcd);
    refreshDisplay = false;
  }

  if (firstEncoder.direction < 0) {
    currentX = MAX(currentX - 1, 0);
    refreshDisplay = true;
    return;
  }
  if (firstEncoder.direction > 0) {
    currentX = MIN(currentX + 1, 999);
    refreshDisplay = true;
    return;
  }

  if (secondEncoder.direction < 0) {
    currentY = MAX(currentY - 1, 0);
    refreshDisplay = true;
    return;
  }
  if (secondEncoder.direction > 0) {
    currentY = MIN(currentY + 1, 999);
    refreshDisplay = true;
    return;
  }

  if (thirdEncoder.direction < 0) {
    currentZ = MAX(currentZ - 1, 0);
    refreshDisplay = true;
    return;
  }
  if (thirdEncoder.direction > 0) {
    currentZ = MIN(currentZ + 1, 999);
    refreshDisplay = true;
    return;
  }

  if (fourthEncoder.direction < 0) {
    currentAngle = MAX(currentAngle - 1, 0);
    refreshDisplay = true;
    return;
  }
  if (fourthEncoder.direction > 0) {
    if (encoder4normalDirection) {
      currentAngle = MIN(currentAngle + 1, 359);  
    }
    else {
      currentAngle = MAX(currentAngle - 1, 0);
    }
    
    refreshDisplay = true;
    return;
  }

  // Return to main menu
  if (secondEncoder.buttonPressed) {
    currentState = ST_MAIN_MENU;
    refreshDisplay = true;
    delay(200);
    return;
  }

  if (thirdEncoder.buttonPressed) {
    currentlyPumpEnabled = !currentlyPumpEnabled;
    refreshDisplay = true;
    delay(200);
    return;
  }

  if (fourthEncoder.buttonPressed) {
    encoder4normalDirection = !encoder4normalDirection;
  }
  
}

void bluetoothMode() {
  Serial.println("Bluetooth mode");
  String line = bluetoothModeSelection ? " [On]     Off" : "  On     [Off]";

  if (refreshDisplay) {
    displayStrings("Bluetooth Mode", line, lcd);
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
    bluetoothModeEnabled = bluetoothModeSelection;

    if (bluetoothModeEnabled) {
      enableBluetooth();
    }
    currentState = ST_MAIN_MENU;
    
    refreshDisplay = true;
    delay(200);
    return;
    
  }

  if (firstEncoder.direction != 0) {
    bluetoothModeSelection = !bluetoothModeSelection;
    refreshDisplay = true;
  }
}

void demoMode() {
  Serial.println("Demo");
}

void processState() {

  switch (currentState) {
    
  case ST_INITIAL: {
      initialState();
      return;
    }

  case ST_MAIN_MENU: {
      mainMenu();
      return;
    }

  case ST_PLAY_PROGRAM: {
      playProgram();
      return;
    }
    
  case ST_CREATE_PROGRAM: {
      createProgram();
      return;
    }
    
  case ST_MANUAL_MODE: {
      manualMode();
      return;
    }
    
  case ST_BLUETOOTH_MODE: {
      bluetoothMode();
      return;
    }
      
    
  case ST_DEMO: {
      demoMode();
      return;
    }
    
  }    
}



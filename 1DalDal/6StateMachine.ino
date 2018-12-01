#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

bool bluetoothModeSelection = false;
bool bluetoothModeEnabled = false;

String menuItems[6] = {"Reset Position", "Play Program", "Create Program", "Manual Mode", "Bluetooth Mode", "Demo"};

int stepSize(long before, long after) {
  long difference = after - before;
  if (difference > 500) {
    return 1;
  }
  if (difference > 300) {
    return 3;
  }
  if (difference > 250) {
    return 6;
  }
  if (difference > 180) {
    return 10;
  }
  return 15;
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
//  Serial.println("Main menu");

  if (firstEncoder.buttonPressed) {
    switch (selectedMenuItem) {
      case MENU_RESET_POSITION:
        currentState = ST_RESET_POSITION;
        refreshDisplay = true;
        Serial.println("Button Pressed");
        delay(200);
        return;
        
      case MENU_PLAY_PROGRAM:
        currentState = ST_PLAY_PROGRAM;
        refreshDisplay = true;
        delay(200);
        return;
        

      case MENU_CREATE_PROGRAM:
        currentState = ST_CREATE_PROGRAM;
        refreshDisplay = true;
        delay(200);
        return;

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

void resetPosition() {
  currentX = 90;
  currentY = 90;
  currentZ = 90;
  currentAngle = 90;
  currentlyPumpEnabled = false;
  beforePumpEnabled = false;

  currentState = ST_MAIN_MENU;
}

void createProgram() {
//  Serial.println("Create program");
}

void manualMode() {
//  Serial.println("Manual mode");

  if (refreshDisplay) {

    char firstLine[20];
    sprintf(firstLine, "XYZ:%3d,%3d,%3d", currentX, currentY, currentZ);
    

    char secondLine[20];
    sprintf(secondLine, "Angle:%-3d Pump:%s", currentAngle, currentlyPumpEnabled ? "1" : "0");

    displayStrings(String(firstLine), String(secondLine), lcd);
    refreshDisplay = false;
  }

  if (firstEncoder.direction < 0) {
    int step = stepSize(currentXUpdate, millis());
    currentX = MAX(currentX - step, minX);
    currentXUpdate = millis();
    refreshDisplay = true;
    return;
  }
  if (firstEncoder.direction > 0) {
    int step = stepSize(currentXUpdate, millis());
    currentX = MIN(currentX + step, maxX);
    currentXUpdate = millis();
    refreshDisplay = true;
    return;
  }

  if (secondEncoder.direction < 0) {
    int step = stepSize(currentYUpdate, millis());
    currentY = MAX(currentY - step, minY);
    currentYUpdate = millis();
    refreshDisplay = true;
    return;
  }
  if (secondEncoder.direction > 0) {
    int step = stepSize(currentYUpdate, millis());
    currentY = MIN(currentY + step, maxY);
    currentYUpdate = millis();
    refreshDisplay = true;
    return;
  }

  if (thirdEncoder.direction < 0) {
    int step = stepSize(currentZUpdate, millis());
    currentZ = MAX(currentZ - step, minZ);
    currentZUpdate = millis();
    refreshDisplay = true;
    return;
  }
  if (thirdEncoder.direction > 0) {
    int step = stepSize(currentZUpdate, millis());
    currentZ = MIN(currentZ + step, maxZ);
    currentZUpdate = millis();
    refreshDisplay = true;
    return;
  }

  if (fourthEncoder.direction < 0) {
    int step = stepSize(currentAngleUpdate, millis());
    currentAngle = MAX(currentAngle - step, minAngle);
    currentZUpdate = millis();
    refreshDisplay = true;
    return;
  }
  if (fourthEncoder.direction > 0) {
    int step = stepSize(currentAngleUpdate, millis());
    if (encoder4normalDirection) {
      currentAngle = MIN(currentAngle + step, maxAngle);  
    }
    else {
      currentAngle = MAX(currentAngle - step, 0);
    }
    currentAngleUpdate = millis();
    refreshDisplay = true;
    return;
  }

  if (firstEncoder.buttonPressed && currentState == ST_CREATE_PROGRAM) {
    currentState = ST_CREATE_PROGRAM_CONFIRM_STEP;
    refreshDisplay = true;
    delay(200);
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
//  Serial.println("Bluetooth mode");
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

void confirmProgramStep() {
  String line = "Step stored";

  if (refreshDisplay) {
    displayStrings("Create Program:", line, lcd);
    refreshDisplay = false;
  }
}

void demoMode() {
//  Serial.println("Demo");
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

   case ST_RESET_POSITION: {
      resetPosition();
      return;
   }

  case ST_PLAY_PROGRAM: {
      playProgram();
      return;
    }
    
  case ST_CREATE_PROGRAM: {
      manualMode();
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



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
        programStepCount = 0;
        currentStep = -1;
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
  currentInputX = startX;
  currentInputY = startY;
  currentInputZ = startZ;
  currentInputAngle = startAngle;
  currentlyPumpEnabled = false;
  beforePumpEnabled = false;

  startX;
  realY = startY;
  realZ = startZ;
  realAngle = startAngle;

  currentState = ST_MAIN_MENU;
}

void createProgram() {
//  Serial.println("Create program");
}

void manualMode() {
//  Serial.println("Manual mode");

  if (refreshDisplay) {

    char firstLine[20];
    sprintf(firstLine, "XYZ:%3d,%3d,%3d", currentInputX, currentInputY, currentInputZ);
    

    char secondLine[20];
    sprintf(secondLine, "A:%-3d Pump:%s #%d", currentInputAngle, currentlyPumpEnabled ? "1" : "0", currentStep + 1 );
    


    displayStrings(String(firstLine), String(secondLine), lcd);
    refreshDisplay = false;
  }

  if (firstEncoder.direction < 0) {
    int step = stepSize(currentInputXUpdate, millis());
    currentInputX = MAX(currentInputX - step, minInputX);
    realX = currentInputX;
    currentInputXUpdate = millis();
    refreshDisplay = true;
    return;
  }
  if (firstEncoder.direction > 0) {
    int step = stepSize(currentInputXUpdate, millis());
    currentInputX = MIN(currentInputX + step, maxInputX);
    realX = currentInputX;
    currentInputXUpdate = millis();
    refreshDisplay = true;
    return;
  }

  if (secondEncoder.direction < 0) {
    int step = stepSize(currentInputYUpdate, millis());
    currentInputY = MAX(currentInputY - step, minInputY);
    realY = minRealY + ((currentInputY - minInputY) / float(maxInputY - minInputY)) * (maxRealY - minRealY);
    currentInputYUpdate = millis();
    refreshDisplay = true;
    return;
  }
  if (secondEncoder.direction > 0) {
    int step = stepSize(currentInputYUpdate, millis());
    currentInputY = MIN(currentInputY + step, maxInputY);
    realY = minRealY + ((currentInputY - minInputY) / float(maxInputY - minInputY)) * (maxRealY - minRealY);
    currentInputYUpdate = millis();
    refreshDisplay = true;
    return;
  }

  if (thirdEncoder.direction < 0) {
    int step = stepSize(currentInputZUpdate, millis());
    currentInputZ = MAX(currentInputZ - step, minInputZ);
    realZ = minRealZ + ((currentInputZ - minInputZ) / float(maxInputZ - minInputZ)) * (maxRealZ - minRealZ);
    currentInputZUpdate = millis();
    refreshDisplay = true;
    return;
  }
  if (thirdEncoder.direction > 0) {
    int step = stepSize(currentInputZUpdate, millis());
    currentInputZ = MIN(currentInputZ + step, maxInputZ);
    realZ = minRealZ + (currentInputZ - minInputZ) / float(maxInputZ - minInputZ) * (maxRealZ - minRealZ);
    currentInputZUpdate = millis();
    refreshDisplay = true;
    return;
  }

  if (fourthEncoder.direction < 0) {
    int step = stepSize(currentInputAngleUpdate, millis());
    currentInputAngle = MAX(currentInputAngle - step, minInputAngle);
    realAngle = currentInputAngle;
    currentInputZUpdate = millis();
    refreshDisplay = true;
    return;
  }
  if (fourthEncoder.direction > 0) {
    int step = stepSize(currentInputAngleUpdate, millis());
    if (encoder4normalDirection) {
      currentInputAngle = MIN(currentInputAngle + step, maxInputAngle);  
    }
    else {
      currentInputAngle = MAX(currentInputAngle - step, 0);
    }
    realAngle = currentInputAngle;
    currentInputAngleUpdate = millis();
    refreshDisplay = true;
    return;
  }

  if (firstEncoder.buttonPressed && currentState == ST_CREATE_PROGRAM) {
    struct ProgramStep newStep;
    newStep.x = currentInputX;
    newStep.y = currentInputY;
    newStep.z = currentInputZ;
    newStep.angle = currentInputAngle;
    newStep.pump = currentlyPumpEnabled;
    newStep.duration = 1000;
    newStep.pauseBefore = 300;
    newStep.pauseAfter = 300;
    currentStep++;
    programStepCount++;
    program[currentStep] = newStep;

    refreshDisplay = true;
    delay(200);
    return;
  }

  // Return to main menu
  if (secondEncoder.buttonPressed) {
    currentStep = -1;
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



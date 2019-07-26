///////////////////////////////////////////////////////////////////////
// Self-Test

void selfTestProgress() {
  if (secondEncoder.buttonPressed) {
      currentState = ST_SELF_TEST;
      refreshDisplay = true;
      movementType = MV_NONE;
      return;
    }

    movementType = MV_TEST;

    switch (testType) {
      case TEST_MENU_SERVO_1: 
        testServo1();
        break;
    }

    if (refreshDisplay) {
      displayStrings(testFirstLine, testSecondLine, lcd);
      refreshDisplay = false;
    }
}


void testServo1() {
  testFirstLine = "Servo1";
  testSecondLine = "";

  switch (testState) {
  case 0:
    testPhaseStart = millis();
    testPhaseLength = 4000;
    testState = 1;
    break;

  case 1:
    testPhaseTimeDelta = millis() - testPhaseStart;
    if (testPhaseTimeDelta < testPhaseLength) {
      servo1Angle = 180.0 * (testPhaseTimeDelta / float(testPhaseLength));
    }
    else {
      testState = 2;
    }
    break;

  case 2:
    testPhaseStart = millis();
    testPhaseLength = 4000;
    testState = 3;
    break;

  case 3:
    testPhaseTimeDelta = millis() - testPhaseStart;
    if (testPhaseTimeDelta < testPhaseLength) {
      servo1Angle = 180.0 * (1 - (testPhaseTimeDelta / float(testPhaseLength)));
    }
    else {
      testState = 0;
    }
    break;
    
    
    
  


    
  }
}


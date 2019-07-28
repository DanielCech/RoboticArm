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
      case TEST_MENU_SERVO_1_SLOW: 
        testPhaseLength = slowSpeed;
        testServo1();
        break;

      case TEST_MENU_SERVO_2_SLOW: 
        testPhaseLength = slowSpeed;
        testServo2();
        break;

      case TEST_MENU_SERVO_3_SLOW: 
        testPhaseLength = slowSpeed;
        testServo3();
        break;

      case TEST_MENU_SERVO_4_SLOW: 
        testPhaseLength = slowSpeed;
        testServo4();
        break;

        case TEST_MENU_SERVO_1_FAST: 
        testPhaseLength = fastSpeed;
        testServo1();
        break;

      case TEST_MENU_SERVO_2_FAST: 
        testPhaseLength = fastSpeed;
        testServo2();
        break;

      case TEST_MENU_SERVO_3_FAST: 
        testPhaseLength = fastSpeed;
        testServo3();
        break;

      case TEST_MENU_SERVO_4_FAST: 
        testPhaseLength = fastSpeed;
        testServo4();
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
    testState = 1;
    break;

  case 1:
    testPhaseTimeDelta = millis() - testPhaseStart;
    if (testPhaseTimeDelta < testPhaseLength) {
      servo1Angle = minServo1Angle + (testPhaseTimeDelta / float(testPhaseLength)) * (maxServo1Angle - minServo1Angle);
    }
    else {
      testState = 2;
    }
    break;

  case 2:
    testPhaseStart = millis();
    testState = 3;
    break;

  case 3:
    testPhaseTimeDelta = millis() - testPhaseStart;
    if (testPhaseTimeDelta < testPhaseLength) {
      servo1Angle = minServo1Angle + (1 - (testPhaseTimeDelta / float(testPhaseLength))) * (maxServo1Angle - minServo1Angle);
    }
    else {
      testState = 0;
    }
    break;
  }
}

void testServo2() {
  testFirstLine = "Servo2";
  testSecondLine = "";

  switch (testState) {
  case 0:
    testPhaseStart = millis();
    testState = 1;
    break;

  case 1:
    testPhaseTimeDelta = millis() - testPhaseStart;
    if (testPhaseTimeDelta < testPhaseLength) {
      servo2Angle = minServo2Angle + (testPhaseTimeDelta / float(testPhaseLength)) * (maxServo2Angle - minServo2Angle);
    }
    else {
      testState = 2;
    }
    break;

  case 2:
    testPhaseStart = millis();
    testState = 3;
    break;

  case 3:
    testPhaseTimeDelta = millis() - testPhaseStart;
    if (testPhaseTimeDelta < testPhaseLength) {
      servo2Angle = minServo2Angle + (1 - (testPhaseTimeDelta / float(testPhaseLength))) * (maxServo2Angle - minServo2Angle);
    }
    else {
      testState = 0;
    }
    break;
  }
}


void testServo3() {
  testFirstLine = "Servo3";
  testSecondLine = "";

  switch (testState) {
  case 0:
    testPhaseStart = millis();
    testState = 1;
    break;

  case 1:
    testPhaseTimeDelta = millis() - testPhaseStart;
    if (testPhaseTimeDelta < testPhaseLength) {
      servo3Angle = minServo3Angle + (testPhaseTimeDelta / float(testPhaseLength)) * (maxServo3Angle - minServo3Angle);
    }
    else {
      testState = 2;
    }
    break;

  case 2:
    testPhaseStart = millis();
    testState = 3;
    break;

  case 3:
    testPhaseTimeDelta = millis() - testPhaseStart;
    if (testPhaseTimeDelta < testPhaseLength) {
      servo3Angle = minServo3Angle + (1 - (testPhaseTimeDelta / float(testPhaseLength))) * (maxServo3Angle - minServo3Angle);
    }
    else {
      testState = 0;
    }
    break;
  }
}

void testServo4() {
  testFirstLine = "Servo4";
  testSecondLine = "";

  switch (testState) {
  case 0:
    testPhaseStart = millis();
    testState = 1;
    break;

  case 1:
    testPhaseTimeDelta = millis() - testPhaseStart;
    if (testPhaseTimeDelta < testPhaseLength) {
      servo4Angle = minServo4Angle + (testPhaseTimeDelta / float(testPhaseLength)) * (maxServo4Angle - minServo4Angle);
    }
    else {
      testState = 2;
    }
    break;

  case 2:
    testPhaseStart = millis();
    testState = 3;
    break;

  case 3:
    testPhaseTimeDelta = millis() - testPhaseStart;
    if (testPhaseTimeDelta < testPhaseLength) {
      servo4Angle = minServo4Angle + (1 - (testPhaseTimeDelta / float(testPhaseLength))) * (maxServo4Angle - minServo4Angle);
    }
    else {
      testState = 0;
    }
    break;
  }
}

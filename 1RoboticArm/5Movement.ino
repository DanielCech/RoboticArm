
////////////////////////////////////////////////////////////////////////
// Playing program

void playProgram() {
//  
//  if (currentStep == -1) {
//    startNewStep();
//    return;
//  }
//
//  if (refreshDisplay) {
//    char line[20];
//    sprintf(line, "%d / %d", currentStep + 1, programStepCount);
//    
//    displayStrings("Playing Program", line, lcd);
//    refreshDisplay = false;
//  }
//
//  double timeDelta = millis() - currentStepBegin;
//  
//  ProgramStep currentProgramStep = program[currentStep];
//
//  switch (currentStepPhase) {
//    case STEP_PAUSE_BEFORE: {
//      if (timeDelta <= currentProgramStep.duration) {
//        // Do nothing 
//      }
//      else {
//        currentStepBegin = millis();
//        float currentProgramStepRealY = minRealY + (currentProgramStep.y - minInputY) / float(maxInputY - minInputY) * (maxRealY - minRealY);
//        float currentProgramStepRealZ = minRealZ + (currentProgramStep.z - minInputZ) / float(maxInputZ - minInputZ) * (maxRealZ - minRealZ);
//        convertCoordinatesToAngles(currentProgramStep.x, currentProgramStepRealY, currentProgramStepRealZ, currentProgramStep.angle);
//        convertedToNextServoAngles();
//        
//        currentStepPhase = STEP_MOVEMENT;
//      }
//      return;
//    }
//
//    case STEP_MOVEMENT: {
//      if (timeDelta <= currentProgramStep.duration) {
//        
//        servo1Angle = lastServo1Angle + (nextServo1Angle - lastServo1Angle) * easeInOutCubic(timeDelta / (double)currentProgramStep.duration);
//        servo2Angle = lastServo2Angle + (nextServo2Angle - lastServo2Angle) * easeInOutCubic(timeDelta / (double)currentProgramStep.duration);
//        servo3Angle = lastServo3Angle + (nextServo3Angle - lastServo3Angle) * easeInOutCubic(timeDelta / (double)currentProgramStep.duration);
//        servo4Angle = lastServo4Angle + (nextServo4Angle - lastServo4Angle) * easeInOutCubic(timeDelta / (double)currentProgramStep.duration);
//      }
//      else {
//        currentStepPhase = STEP_PAUSE_AFTER;
//        currentStepBegin = millis();
//      }
//      return;
//    }
//
//    case STEP_PAUSE_AFTER: {
//      if (timeDelta <= currentProgramStep.duration) {
//        // Do nothing 
//      }
//      else {
//        startNewStep();
//      }
//      return;
//    }
//  }
}

//void startNewStep() {
//  Serial.printf("startNewStep\n");
//  
//  if (currentStep < programStepCount - 1) {
//      currentStep++;
//      currentStepPhase = STEP_PAUSE_BEFORE;
//      lastServo1Angle = servo1Angle;
//      lastServo2Angle = servo2Angle;
//      lastServo3Angle = servo3Angle;
//      lastServo4Angle = servo4Angle;
//
//      currentStepBegin = millis();
//
//      Serial.printf("Step: %d\n", currentStep);
//      ProgramStep currentProgramStep = program[currentStep];
//
//      currentlyPumpEnabled = currentProgramStep.pump;
//      
//      refreshDisplay = true;
//    }
//    else {
//      Serial.printf("Program End\n");
//      currentState = ST_MAIN_MENU;
//      refreshDisplay = true;
//      delay(200);
//
//      currentStep = -1;
//      currentStepPhase = STEP_PAUSE_BEFORE;
//      return;
//    }
//}


////////////////////////////////////////////////////////////////////////
// Movement transition functions

double linear(double t) {
  return t;
}

double easeInOutCubic(double t) {
  return t<.5 ? 4*t*t*t : (t-1)*(2*t-2)*(2*t-2)+1;
}

float toDegrees(float angle) {
  return 180 * angle / PI;
}

////////////////////////////////////////////////////////////////////////
// Input coordinates to real coordinates

void convertInputToRealCoordinates(float inputX, float inputY, float inputZ, float inputAngle, float& realX, float& realY, float& realZ, float& realAngle) {
  realX = inputX;
  realY = minRealY + (inputY - minInputY) / float(maxInputY - minInputY) * (maxRealY - minRealY);
  realZ = minRealZ + (inputZ - minInputZ) / float(maxInputZ - minInputZ) * (maxRealZ - minRealZ);
  realAngle = inputAngle;

  checkRealCoordinateLimits(realX, realY, realZ, realAngle);
};



////////////////////////////////////////////////////////////////////////
// Real coordinates to angles

void convertRealCoordinatesToAngles(float realX, float realY, float realZ, float realAngle, float& outputServo1Angle, float& outputServo2Angle, float& outputServo3Angle, float& outputServo4Angle) {

  float heightDelta = realY - baseHeight;
  float chord = sqrt(realZ * realZ + heightDelta * heightDelta);
  

  float betaComplement = toDegrees(acos((chord / 2) / armSegmentLength));
  float delta = toDegrees(asin(abs(heightDelta) / chord));
  float gamaComplement = toDegrees(asin((chord / 2) / armSegmentLength)) / 2; //180 - 2 * betaComplement;
  
  float beta;
  float gama;

  if (heightDelta > 0) {
    beta = 90 - betaComplement - delta;
    gama = 90 - gamaComplement - 1.1 * delta;
  }
  else if (heightDelta < 0) {
    beta = 90 - betaComplement + delta;
    gama = 90 - gamaComplement + 1.1 * delta;
  }
  else {
    beta = 90 - betaComplement;
    gama = 90 - gamaComplement;
  }

  outputServo1Angle = 180 - realX;
  outputServo2Angle = gama - 10;
  outputServo3Angle = 107 - beta;
  outputServo4Angle = 181 - realAngle;

  Serial.printf("iX: %d, iY: %d, iZ: %d, rX: %.2f, rY: %.2f, rZ: %.2f, hD: %.2f, chord: %.2f, b: %.2f(%.2f), d: %.2f, g: %.2f(%.2f), s1: %.2f, s2: %.2f, s3: %.2f, s4: %.2f\n",  targetInputX, targetInputY, targetInputZ, realX, realY, realZ, heightDelta, chord, beta, betaComplement, delta, gama, gamaComplement, outputServo1Angle, outputServo2Angle, outputServo3Angle, outputServo4Angle);
};


////////////////////////////////////////////////////////////////////////
// Limits checking

void checkServoAngleLimits(float& angle1, float& angle2, float& angle3, float& angle4) {
  if (angle1 < minServo1Angle) {
    angle1 = minServo1Angle;
  }

  if (angle1 > maxServo1Angle) {
    angle1 = maxServo1Angle;
  }

  if (angle2 < minServo2Angle) {
    angle2 = minServo2Angle;
  }

  if (angle2 > maxServo2Angle) {
    angle2 = maxServo2Angle;
  }

  if (angle3 < minServo3Angle) {
    angle3 = minServo3Angle;
  }

  if (angle3 > maxServo3Angle) {
    angle3 = maxServo3Angle;
  }

  if (angle4 < minServo4Angle) {
    angle4 = minServo4Angle;
  }

  if (angle4 > maxServo4Angle) {
    angle4 = maxServo4Angle;
  }
}


void checkInputCoordinateLimits(int& inputX, int& inputY, int& inputZ, int& inputAngle) {
  if (inputX < minInputX) {
    inputX = minInputX;
  }

  if (inputX > maxInputX) {
    inputX = maxInputX;
  }

  if (inputY < minInputY) {
    inputY = minInputY;
  }

  if (inputY > maxInputY) {
    inputY = maxInputY;
  }

  if (inputZ < minInputZ) {
    inputY = minInputZ;
  }

  if (inputZ > maxInputZ) {
    inputZ = maxInputZ;
  }

  if (inputAngle < minInputAngle) {
    inputAngle = minInputAngle;
  }

  if (inputAngle > maxInputAngle) {
    inputAngle = maxInputAngle;
  }

  // Avoid collision with vacuum pump
  if (inputY < 50) {
    if (inputX < 55) {
      inputX = 55;
    }
  }
}


void checkRealCoordinateLimits(float& realX, float& realY, float& realZ, float& realAngle) {
  if (realX < minRealX) {
    realX = minRealX;
  }

  if (realX > maxRealX) {
    realX = maxRealX;
  }

  if (realY < minRealY) {
    realY = minRealY;
  }

  if (realY > maxRealY) {
    realY = maxRealY;
  }

  if (realZ < minRealZ) {
    realZ = minRealZ;
  }

  if (realZ > maxRealZ) {
    realZ = maxRealZ;
  }

  if (realAngle < minRealAngle) {
    realAngle = minRealAngle;
  }

  if (realAngle > maxRealAngle) {
    realAngle = maxRealAngle;
  }
}

////////////////////////////////////////////////////////////////////////
// Movement

void startMovement(float toInputX, float toInputY, float toInputZ, float toInputAngle) {
  if (movePhase != MOVE_NONE) { return; }
  if (equal(currentInputX, toInputX) && equal(currentInputY, toInputY) && equal(currentInputZ, toInputZ) && equal(currentInputAngle, toInputAngle)) { return; }

  targetInputX = toInputX;
  targetInputY = toInputY;
  targetInputZ = toInputZ;
  targetInputAngle = toInputAngle;

  fromServo1Angle = servo1Angle;
  fromServo2Angle = servo2Angle;
  fromServo3Angle = servo3Angle;
  fromServo4Angle = servo4Angle;

//  convertCoordinatesToAngles(fromInputX, fromInputY, fromInputZ, fromInputAngle, fromServo1Angle, fromServo2Angle, fromServo3Angle, fromServo4Angle);

  float toRealX;
  float toRealY;
  float toRealZ;
  float toRealAngle;

  convertInputToRealCoordinates(toInputX, toInputY, toInputZ, toInputAngle, toRealX, toRealY, toRealZ, toRealAngle);  
  convertRealCoordinatesToAngles(toRealX, toRealY, toRealZ, toRealAngle, toServo1Angle, toServo2Angle, toServo3Angle, toServo4Angle);
  
  movePhase = MOVE_BEGIN;
  
//  switch (movePhase) {
//    case MOVE_BEGIN:
//    case MOVE_FINISHED: 
//      convertCoordinatesToAngles(fromInputX, fromInputY, fromInputZ, fromInputAngle, fromServo1Angle, fromServo2Angle, fromServo3Angle, fromServo4Angle);
//      convertCoordinatesToAngles(toInputX, toInputY, toInputZ, toInputAngle, toServo1Angle, toServo2Angle, toServo3Angle, toServo4Angle);
//      movePhase = MOVE_BEGIN;
//      return;
//
//    case MOVE_IN_PROGRESS: 
//      fromServo1Angle = servo1Angle;
//      fromServo2Angle = servo2Angle;
//      fromServo3Angle = servo3Angle;
//      fromServo4Angle = servo4Angle;   
//   
//      convertCoordinatesToAngles(toInputX, toInputY, toInputZ, toInputAngle, toServo1Angle, toServo2Angle, toServo3Angle, toServo4Angle);
//      movePhase = MOVE_IN_PROGRESS;
//      currentStepBegin = millis();
//      return; 
//    }
}

void movement() {
  switch (movementType) {
    case none:
      break;
      
    case localManual:
    case remoteManual:
      manualMovement();
      break;

    // TODO: complete
    case localProgram:
      break;  
      
    case remoteProgram:
      immediateMovement();
  }

  moveServos();
}

void moveServos() {
  checkServoAngleLimits(servo1Angle, servo2Angle, servo3Angle, servo4Angle);

  servo1.write(pulseWidthForAngle(servo1Angle));
  servo2.write(pulseWidthForAngle(servo2Angle));
  servo3.write(pulseWidthForAngle(servo3Angle));
  servo4.write(pulseWidthForAngle(servo4Angle)); 

//  if (loopPhase == 0) {
//    Serial.printf("Values: s1:%.3f s2:%.3f s3:%.3f s4:%.3f\n", servo1Angle, servo2Angle, servo3Angle, servo4Angle); 
//  }

  loopPhase = (loopPhase + 1) % 20; 

  previousServo1Angle = servo1Angle;
  previousServo2Angle = servo2Angle;
  previousServo3Angle = servo3Angle;
  previousServo4Angle = servo4Angle;
}

void manualMovement() {  
  switch (movePhase) {
    case MOVE_NONE:
      break;
      
    case MOVE_BEGIN: {
      currentStepBegin = millis();
      movePhase = MOVE_IN_PROGRESS;
//      Serial.printf("Begin\n");
      return;
    }

    case MOVE_IN_PROGRESS: {      
      double timeDelta = millis() - currentStepBegin;
      if (timeDelta <= moveStepDuration) {
        double progress = easeInOutCubic(timeDelta / (double)moveStepDuration);
        servo1Angle = fromServo1Angle + (toServo1Angle - fromServo1Angle) * progress;
        servo2Angle = fromServo2Angle + (toServo2Angle - fromServo2Angle) * progress;
        servo3Angle = fromServo3Angle + (toServo3Angle - fromServo3Angle) * progress;
        servo4Angle = fromServo4Angle + (toServo4Angle - fromServo4Angle) * progress;
      }
      else {
        movePhase = MOVE_FINISHED;
      }
      return;
    }

    case MOVE_FINISHED: {
      //Serial.printf("Finished\n");
      currentInputX = targetInputX;
      currentInputY = targetInputY;
      currentInputZ = targetInputZ;
      currentInputAngle = targetInputAngle;
      movePhase = MOVE_NONE;
      return;
    }
  }
}

void immediateMovement() {
//          
//  double timeDelta = millis() - lastBluetoothUpdate;
//  
//  if (timeDelta <= bluetoothStepDuration) {
//    
//    servo1Angle = lastServo1Angle + (nextServo1Angle - lastServo1Angle) * linear(timeDelta / (double)bluetoothStepDuration);
//    servo2Angle = lastServo2Angle + (nextServo2Angle - lastServo2Angle) * linear(timeDelta / (double)bluetoothStepDuration);
//    servo3Angle = lastServo3Angle + (nextServo3Angle - lastServo3Angle) * linear(timeDelta / (double)bluetoothStepDuration);
//    servo4Angle = lastServo4Angle + (nextServo4Angle - lastServo4Angle) * linear(timeDelta / (double)bluetoothStepDuration);
//
//  }
}

////////////////////////////////////////////////////////////////////////
// Updating

//void servoAnglesToLastServoAngles() {
//  lastServo1Angle = servo1Angle;
//  lastServo2Angle = servo2Angle;
//  lastServo3Angle = servo3Angle;
//  lastServo4Angle = servo4Angle;
//}

//void convertedToNextServoAngles() {
//  nextServo1Angle = convertedServo1Angle;
//  nextServo2Angle = convertedServo2Angle;
//  nextServo3Angle = convertedServo3Angle;
//  nextServo4Angle = convertedServo4Angle;
//}

void numbersToCurrentInput() {
  if ((numberX >= minInputX) && (numberX <= maxInputX)) {
    selectedInputX = numberX;
  }

  if ((numberY >= minInputY) && (numberY <= maxInputY)) {
    selectedInputY = numberY;
  }

  if ((numberZ >= minInputZ) && (numberZ <= maxInputZ)) {
    selectedInputZ = numberZ;
  }

  if ((numberAngle >= minInputAngle) && (numberAngle <= maxInputAngle)) {
    selectedInputAngle = numberAngle;
  }
}

//void updateNextServoAngles(bool changePhase) {
//  servoAnglesToLastServoAngles();
//      
//  float currentProgramStepRealY = minRealY + (selectedInputY - minInputY) / float(maxInputY - minInputY) * (maxRealY - minRealY);
//  float currentProgramStepRealZ = minRealZ + (selectedInputZ - minInputZ) / float(maxInputZ - minInputZ) * (maxRealZ - minRealZ);
//  convertCoordinatesToAngles(selectedInputX, currentProgramStepRealY, currentProgramStepRealZ, selectedInputAngle);
//  convertedToNextServoAngles();
//  currentStepBegin = millis();
//
//  if (changePhase) {
//    if (movePhase != MOVE_IN_PROGRESS) {
//      movePhase = MOVE_BEGIN;
//    }  
//  }
//}


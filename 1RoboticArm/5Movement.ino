
void playProgram() {
  
  if (currentStep == -1) {
    startNewStep();
    return;
  }

  if (refreshDisplay) {
    char line[20];
    sprintf(line, "%d / %d", currentStep + 1, programStepCount);
    
    displayStrings("Playing Program", line, lcd);
    refreshDisplay = false;
  }

  double timeDelta = millis() - currentStepBegin;
  
  ProgramStep currentProgramStep = program[currentStep];

  switch (currentStepPhase) {
    case STEP_PAUSE_BEFORE: {
      if (timeDelta <= currentProgramStep.duration) {
        // Do nothing 
      }
      else {
        currentStepBegin = millis();
        float currentProgramStepRealY = minRealY + (currentProgramStep.y - minInputY) / float(maxInputY - minInputY) * (maxRealY - minRealY);
        float currentProgramStepRealZ = minRealZ + (currentProgramStep.z - minInputZ) / float(maxInputZ - minInputZ) * (maxRealZ - minRealZ);
        convertCoordinatesToAngles(currentProgramStep.x, currentProgramStepRealY, currentProgramStepRealZ, currentProgramStep.angle);
        nextServo1Angle = convertedServo1Angle;
        nextServo2Angle = convertedServo2Angle;
        nextServo3Angle = convertedServo3Angle;
        nextServo4Angle = convertedServo4Angle;
        
        currentStepPhase = STEP_MOVEMENT;
      }
      return;
    }

    case STEP_MOVEMENT: {
      if (timeDelta <= currentProgramStep.duration) {
        
        servo1Angle = lastServo1Angle + (nextServo1Angle - lastServo1Angle) * easeInOutCubic(timeDelta / (double)currentProgramStep.duration);
        servo2Angle = lastServo2Angle + (nextServo2Angle - lastServo2Angle) * easeInOutCubic(timeDelta / (double)currentProgramStep.duration);
        servo3Angle = lastServo3Angle + (nextServo3Angle - lastServo3Angle) * easeInOutCubic(timeDelta / (double)currentProgramStep.duration);
        servo4Angle = lastServo4Angle + (nextServo4Angle - lastServo4Angle) * easeInOutCubic(timeDelta / (double)currentProgramStep.duration);
    
        Serial.printf("Values: s1:%.2f s2:%.2f s3:%.2f s4:%.2f\n", servo1Angle, servo2Angle, servo3Angle, servo4Angle);
      }
      else {
        currentStepPhase = STEP_PAUSE_AFTER;
        currentStepBegin = millis();
      }
      return;
    }

    case STEP_PAUSE_AFTER: {
      if (timeDelta <= currentProgramStep.duration) {
        // Do nothing 
      }
      else {
        startNewStep();
      }
      return;
    }
  }
}

void startNewStep() {
  Serial.printf("startNewStep\n");
  
  if (currentStep < programStepCount - 1) {
      currentStep++;
      currentStepPhase = STEP_PAUSE_BEFORE;
      lastServo1Angle = servo1Angle;
      lastServo2Angle = servo2Angle;
      lastServo3Angle = servo3Angle;
      lastServo4Angle = servo4Angle;

      currentStepBegin = millis();

      Serial.printf("Step: %d\n", currentStep);
      ProgramStep currentProgramStep = program[currentStep];

      currentlyPumpEnabled = currentProgramStep.pump;
      
      refreshDisplay = true;
    }
    else {
      Serial.printf("Program End\n");
      currentState = ST_MAIN_MENU;
      refreshDisplay = true;
      delay(200);

      currentStep = -1;
      currentStepPhase = STEP_PAUSE_BEFORE;
      return;
    }
}


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

// Coordinates to angles

void convertCoordinatesToAngles(float x, float y, float z, float angle) {

  float heightDelta = y - baseHeight;
  float chord = sqrt(z * z + heightDelta * heightDelta);
  

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

  convertedServo1Angle = 180 - x;
  convertedServo2Angle = gama - 10;
  convertedServo3Angle = 107 - beta;
  convertedServo4Angle = 181 - angle;

  //Serial.printf("rX: %.2f, rY: %.2f, rZ: %.2f, hD: %.2f, chord: %.2f, b: %.2f(%.2f), d: %.2f, g: %.2f(%.2f), s2: %.2f, s3: %.2f, s4: %.2f\n",  realX, realY, realZ, heightDelta, chord, beta, betaComplement, delta, gama, gamaComplement, servo2Angle, servo3Angle, servo4Angle);
};


void checkServoAngleLimits() {
  if (servo1Angle < minServo1Angle) {
    servo1Angle = minServo1Angle;
  }

  if (servo1Angle > maxServo1Angle) {
    servo1Angle = maxServo1Angle;
  }

  if (servo2Angle < minServo2Angle) {
    servo2Angle = minServo2Angle;
  }

  if (servo2Angle > maxServo2Angle) {
    servo2Angle = maxServo2Angle;
  }

  if (servo3Angle < minServo3Angle) {
    servo3Angle = minServo3Angle;
  }

  if (servo3Angle > maxServo3Angle) {
    servo3Angle = maxServo3Angle;
  }

  if (servo4Angle < minServo4Angle) {
    servo4Angle = minServo4Angle;
  }

  if (servo4Angle > maxServo4Angle) {
    servo4Angle = maxServo4Angle;
  }
}

void checkRealCoordinateLimits() {
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


void checkInputCoordinateLimits() {
  if (currentInputX < minInputX) {
    currentInputX = minInputX;
  }

  if (currentInputX > maxInputX) {
    currentInputX = maxInputX;
  }

  if (currentInputY < minInputY) {
    currentInputY = minInputY;
  }

  if (currentInputY > maxInputY) {
    currentInputY = maxInputY;
  }

  if (currentInputZ < minInputZ) {
    currentInputZ = minInputZ;
  }

  if (currentInputZ > maxInputZ) {
    currentInputZ = maxInputZ;
  }

  if (currentInputAngle < minInputAngle) {
    currentInputAngle = minInputAngle;
  }

  if (currentInputAngle > maxInputAngle) {
    currentInputAngle = maxInputAngle;
  }
}


void manualMovement() {  
  double timeDelta = millis() - currentStepBegin;
  
  switch (movePhase) {
    case MOVE_BEGIN: {
      updateNextServoAngles(false);
      
      movePhase = MOVE_IN_PROGRESS;
      Serial.printf("Begin\n");
      return;
    }

    case MOVE_IN_PROGRESS: {
      if (timeDelta <= moveStepDuration) {
        
        servo1Angle = lastServo1Angle + (nextServo1Angle - lastServo1Angle) * easeInOutCubic(timeDelta / (double)moveStepDuration);
        servo2Angle = lastServo2Angle + (nextServo2Angle - lastServo2Angle) * easeInOutCubic(timeDelta / (double)moveStepDuration);
        servo3Angle = lastServo3Angle + (nextServo3Angle - lastServo3Angle) * easeInOutCubic(timeDelta / (double)moveStepDuration);
        servo4Angle = lastServo4Angle + (nextServo4Angle - lastServo4Angle) * easeInOutCubic(timeDelta / (double)moveStepDuration);

        Serial.printf("Movement\n");
        //Serial.printf("Values: s1:%.2f s2:%.2f s3:%.2f s4:%.2f\n", servo1Angle, servo2Angle, servo3Angle, servo4Angle);
      }
      else {
        movePhase = MOVE_FINISHED;
      }
      return;
    }

    case MOVE_FINISHED: {
      //Serial.printf("Finished\n");
      return;
    }
  }
}

void updateNextServoAngles(bool changePhase) {
  lastServo1Angle = servo1Angle;
  lastServo2Angle = servo2Angle;
  lastServo3Angle = servo3Angle;
  lastServo4Angle = servo4Angle;
      
  float currentProgramStepRealY = minRealY + (currentInputY - minInputY) / float(maxInputY - minInputY) * (maxRealY - minRealY);
  float currentProgramStepRealZ = minRealZ + (currentInputZ - minInputZ) / float(maxInputZ - minInputZ) * (maxRealZ - minRealZ);
  convertCoordinatesToAngles(currentInputX, currentProgramStepRealY, currentProgramStepRealZ, currentInputAngle);
  nextServo1Angle = convertedServo1Angle;
  nextServo2Angle = convertedServo2Angle;
  nextServo3Angle = convertedServo3Angle;
  nextServo4Angle = convertedServo4Angle;
  currentStepBegin = millis();

  if (changePhase) {
    if (movePhase != MOVE_IN_PROGRESS) {
      movePhase = MOVE_BEGIN;
    }  
  }
}


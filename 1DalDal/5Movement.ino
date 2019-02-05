
void playProgram() {
//  Serial.printf("playProgram\n");
  
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
        currentStepPhase = STEP_MOVEMENT;
      }
      return;
    }

    case STEP_MOVEMENT: {
      if (timeDelta <= currentProgramStep.duration) {
        int nextX = currentProgramStep.x;
        int nextY = currentProgramStep.y;
        int nextZ = currentProgramStep.z;
        int nextAngle = currentProgramStep.angle;
        
        realX = lastInputX + (nextX - lastInputX) * easeInOutCubic(timeDelta / (double)currentProgramStep.duration);
        realY = lastInputY + (nextY - lastInputY) * easeInOutCubic(timeDelta / (double)currentProgramStep.duration);
        realZ = lastInputZ + (nextZ - lastInputZ) * easeInOutCubic(timeDelta / (double)currentProgramStep.duration);
        realAngle = lastInputAngle + (nextAngle - lastInputAngle) * easeInOutCubic(timeDelta / (double)currentProgramStep.duration);
    
        Serial.printf("Values: %.2f %.2f %.2f %.2f\n", realX, realY, realZ, realAngle);
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
      lastInputX = currentInputX;
      lastInputY = currentInputY;
      lastInputZ = currentInputZ;
      lastInputAngle = currentInputAngle;

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

double linear(double t) {
  return t;
}

double easeInOutCubic(double t) {
  return t<.5 ? 4*t*t*t : (t-1)*(2*t-2)*(2*t-2)+1;
}

float toDegrees(float angle) {
  return 180 * angle / PI;
}

void convertCoordinatesToAngles() {

  checkRealCoordinateLimits();

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

  servo1Angle = realX;
  servo2Angle = gama - 10;
  servo3Angle = 107 - beta;
  servo4Angle = realAngle;

  Serial.printf("rX: %.2f, rY: %.2f, rZ: %.2f, hD: %.2f, chord: %.2f, b: %.2f(%.2f), d: %.2f, g: %.2f(%.2f), s2: %.2f, s3: %.2f\n",  realX, realY, realZ, heightDelta, chord, beta, betaComplement, delta, gama, gamaComplement, servo2Angle, servo3Angle);

  checkServoAngleLimits();
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



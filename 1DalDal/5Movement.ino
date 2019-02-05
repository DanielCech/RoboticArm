
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

      currentStep == -1;
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
//
//  realX = currentX;
//  realY = currentY;
//  realZ = currentZ;
//  realAngle = currentAngle;

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

  
  
//  Serial.printf("centimeterY: %.2f, centimeterZ: %.2f, heightDelta: %.2f, chord: %.2f\n", centimeterY, centimeterZ, heightDelta, chord);


//  float beta = toDegrees(acos((chord / 2) / armSegmentLength));
//  float delta = toDegrees(asin(abs(heightDelta) / chord));
//  float gama = 180 - 2 * beta;
//
//  float epsilon;
//
//  if (heightDelta > 0) {
//    epsilon = beta + delta;
//  }
//  else if (heightDelta < 0) {
//    epsilon = beta - delta;
//  }
//  else {
//    epsilon = beta;
//  }
//
//  float servoEpsilon = 90 - epsilon + 77;
//  float servoGama = 90 - gama + 28;

  
/////////////////////////////

//  float x_1 = 0;
//  float d = 0;
//  float gama = 0;
//  float alfa = 0;
//  float alfa_1 = 0;
//  float alfa_2 = 0;
//  float beta = 0;
//  float delta = 0;
//  float offset_0 = 92;
//  float offset_1 = 1;
//  float offset_2 = 45; //ak je servo_2 otocene na 45°, vtedy  je beta=0
//  float compensation = 0; //vykompenzovanie sklonu drahy pri pohybe po osi X
//
//  x_1 = sqrt((x * x) + (y * y));
//  delta = ((180 * asin(y / x_1)) / 3.14);
//  x_1 = x_1 - 45;
//  d = sqrt((z * z) + (x_1 * x_1));
//  gama = ((2 * 180 * asin(d / 400)) / 3.14);
//  alfa_1 = ((180 * asin(z / d)) / 3.14);
//  alfa_2 = (180 - gama) / 2;
//  alfa = alfa_1 + alfa_2;
//  beta = 180 - alfa - gama;
//
//  compensation = x_1 * 0.018;
//
//  float a = delta + offset_0;
//  float b = alfa + offset_1 + compensation;
//  float c = beta + offset_2 - compensation;
//
//  if (b < minY) {
//    b = minY;
//  }
//
//  if (b > maxY) {
//    b = maxY;
//  }
//
//  if (c < minZ) {
//    c = minZ;
//  }
//
//  if (c > maxY) {
//    c = maxZ;
//  }
//
////  if (currentZ < minZ) {
////    currentZ = minZ;
////  }
////
////  if (currentZ > maxZ) {
////    currentZ = maxZ;
////  }
//
//  if (currentAngle < minAngle) {
//    currentAngle = minAngle;
//  }
//
//  if (currentAngle > maxAngle) {
//    currentAngle = maxAngle;
//  }

//  Serial.printf("cY: %.2f, cZ: %.2f, hD: %.2f, chord: %.2f, b: %.2f, d: %.2f, e: %.2f(%.2f), g: %.2f(%.2f)\n",  centimeterY, centimeterZ, heightDelta, chord, beta, delta, epsilon, servoEpsilon, gama, servoGama);

//  if (servoEpsilon < minEpsilon) {
//    servoEpsilon = minEpsilon;
//  }
//
//  if (servoEpsilon > maxEpsilon) {
//    servoEpsilon = maxEpsilon;
//  }
//
//  if (servoGama < minGama) {
//    servoGama = minGama;
//  }
//
//  if (servoGama > maxGama) {
//    servoGama = maxGama;
//  }

  servo1Angle = realX;
  servo2Angle = gama - 20;
  servo3Angle = 97 - beta;
  servo4Angle = realAngle;

  Serial.printf("rX: %.2f, rY: %.2f, rZ: %.2f, hD: %.2f, chord: %.2f, b: %.2f(%.2f), d: %.2f, g: %.2f(%.2f), s2: %.2f, s3: %.2f\n",  realX, realY, realZ, heightDelta, chord, beta, betaComplement, delta, gama, gamaComplement, servo2Angle, servo3Angle);

  checkServoAngleLimits();

//
//  float x_1 = 0;
//  float d = 0;
//  float gama = 0;
//  float alfa = 0;
//  float alfa_1 = 0;
//  float alfa_2 = 0;
//  float beta = 0;
//  float delta = 0;
//  float offset_0 = 92;
//  float offset_1 = 1;
//  float offset_2 = 45; //ak je servo_2 otocene na 45°, vtedy  je beta=0
//  float compensation = 0; //vykompenzovanie sklonu drahy pri pohybe po osi X

//  servo3.write(servoEpsilon); //rameno dolne
//  servo2.write(servoGama); //rameno horne
//
//  servo3.write(c); //rameno dolne
//  servo2.write(b); //rameno horne
  


//  servo3.write(90); //rameno dolne
//  servo2.write(servoEpsilon); //rameno horne
//  
  


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


void setupProgram() {
  struct ProgramStep first;
  first.x = 50;
  first.y = 50;
  first.z = 50;
  first.angle = 50;
  first.pump = false;
  first.duration = 1500;
  first.pauseBefore = 1500;
  first.pauseAfter = 1500;
  program[0] = first;

  struct ProgramStep second;
  second.x = 140;
  second.y = 75;
  second.z = 50;
  second.angle = 50;
  second.pump = false;
  second.duration = 1500;
  second.pauseBefore = 1500;
  second.pauseAfter = 1500;
  program[1] = second;

  struct ProgramStep third;
  third.x = 140;
  third.y = 75;
  third.z = 45;
  third.angle = 50;
  third.pump = true;
  third.duration = 1500;
  third.pauseBefore = 1500;
  third.pauseAfter = 1500;
  program[2] = third;

  struct ProgramStep fourth;
  fourth.x = 56;
  fourth.y = 68;
  fourth.z = 90;
  fourth.angle = 50;
  fourth.pump = false;
  fourth.duration = 1500;
  fourth.pauseBefore = 1500;
  fourth.pauseAfter = 1500;
  program[3] = fourth;

  programStepCount = 4;
}


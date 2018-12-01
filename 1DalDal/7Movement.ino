

void playProgram() {
  
  if (currentStepBegin == 0) {
    currentStep = 0;
    currentStepBegin = millis();
    Serial.printf("Step: %d\n", currentStep);
  }

  if (refreshDisplay) {
    char line[20];
    sprintf(line, "%d / %d", currentStep + 1, programStepCount);
    
    displayStrings("Playing Program", line, lcd);
    refreshDisplay = false;
  }

  double timeDelta = millis() - currentStepBegin;
  
  ProgramStep currentProgramStep = program[currentStep];

  if (timeDelta <= currentProgramStep.duration) {
    int nextX = currentProgramStep.x;
    int nextY = currentProgramStep.y;
    int nextZ = currentProgramStep.z;
    int nextAngle = currentProgramStep.angle;
    
    currentX = lastX + (nextX - lastX) * easeInOutCubic(timeDelta / (double)currentProgramStep.duration);
    currentY = lastY + (nextY - lastY) * easeInOutCubic(timeDelta / (double)currentProgramStep.duration);
    currentZ = lastZ + (nextZ - lastZ) * easeInOutCubic(timeDelta / (double)currentProgramStep.duration);
    currentAngle = lastAngle + (nextAngle - lastAngle) * easeInOutCubic(timeDelta / (double)currentProgramStep.duration);

    Serial.printf("Values: %d %d %d %d\n", currentX, currentY, currentZ, currentAngle);
  }
  else {
    if (currentStep < programStepCount - 1) {
      currentStep++;
      lastX = currentX;
      lastY = currentY;
      lastZ = currentZ;
      lastAngle = currentAngle;

      currentStepBegin = millis();

      Serial.printf("Step: %d\n", currentStep);
      refreshDisplay = true;
    }
    else {
      Serial.printf("Program End\n");
      currentState = ST_MAIN_MENU;
      refreshDisplay = true;
      delay(200);

      currentStepBegin = 0;
      return;
    }
  }
}

double linear(double t) {
  return t;
}

double easeInOutCubic(double t) {
  return t<.5 ? 4*t*t*t : (t-1)*(2*t-2)*(2*t-2)+1;
}

void convertCoordinatesToAngles(float x, float y, float z) {

  float x_1 = 0;
  float d = 0;
  float gama = 0;
  float alfa = 0;
  float alfa_1 = 0;
  float alfa_2 = 0;
  float beta = 0;
  float delta = 0;
  float offset_0 = 92;
  float offset_1 = 1;
  float offset_2 = 45; //ak je servo_2 otocene na 45°, vtedy  je beta=0
  float compensation = 0; //vykompenzovanie sklonu drahy pri pohybe po osi X

  x_1 = sqrt((x * x) + (y * y));
  delta = ((180 * asin(y / x_1)) / 3.14);
  x_1 = x_1 - 45;
  d = sqrt((z * z) + (x_1 * x_1));
  gama = ((2 * 180 * asin(d / 400)) / 3.14);
  alfa_1 = ((180 * asin(z / d)) / 3.14);
  alfa_2 = (180 - gama) / 2;
  alfa = alfa_1 + alfa_2;
  beta = 180 - alfa - gama;

  compensation = x_1 * 0.018;

  float a = delta + offset_0;
  float b = alfa + offset_1 + compensation;
  float c = beta + offset_2 - compensation;

  if (a < minX) {
    a = minX;
  }

  if (a > maxX) {
    a = maxX;
  }

  if (b < minY) {
    b = minY;
  }

  if (b > maxY) {
    b = maxY;
  }

  if (c < minZ) {
    c = minZ;
  }

  if (c > maxZ) {
    c = maxZ;
  }

  servo1.write(a); //serva trupu
  servo2.write(b); //rameno dolne
  servo3.write(c); //rameno horne

  Serial.printf("A: %.2f, B: %.2f, C: %.2f\n", a, b, c);
};


void checkLimits() {
  if (currentX < minX) {
    currentX = minX;
  }

  if (currentX > maxX) {
    currentX = maxX;
  }

  if (currentY < minY) {
    currentY = minY;
  }

  if (currentY > maxY) {
    currentY = maxY;
  }

  if (currentZ < minZ) {
    currentZ = minZ;
  }

  if (currentZ > maxZ) {
    currentZ = maxZ;
  }

  if (currentAngle < minAngle) {
    currentAngle = minAngle;
  }

  if (currentAngle > maxAngle) {
    currentAngle = maxAngle;
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
  program[0] = first;

  struct ProgramStep second;
  second.x = 140;
  second.y = 75;
  second.z = 50;
  second.angle = 50;
  second.pump = false;
  second.duration = 1500;
  program[1] = second;

  struct ProgramStep third;
  third.x = 140;
  third.y = 75;
  third.z = 45;
  third.angle = 50;
  third.pump = false;
  third.duration = 1500;
  program[2] = third;

  struct ProgramStep fourth;
  fourth.x = 56;
  fourth.y = 68;
  fourth.z = 90;
  fourth.angle = 50;
  fourth.pump = false;
  fourth.duration = 1500;
  program[3] = fourth;

  programStepCount = 4;
}


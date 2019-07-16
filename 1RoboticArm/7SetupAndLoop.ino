
////////////////////////////////////////////////////////////////////////
// Main

int loopPhase = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  pinMode(ENCODER_1_CLK, INPUT);
  pinMode(ENCODER_1_DT, INPUT);
  pinMode(ENCODER_1_SW, INPUT);

  pinMode(ENCODER_2_CLK, INPUT);
  pinMode(ENCODER_2_DT, INPUT);
  pinMode(ENCODER_2_SW, INPUT);

  pinMode(ENCODER_3_CLK, INPUT);
  pinMode(ENCODER_3_DT, INPUT);
  pinMode(ENCODER_3_SW, INPUT);

  pinMode(ENCODER_4_CLK, INPUT);
  pinMode(ENCODER_4_DT, INPUT);
  pinMode(ENCODER_4_SW, INPUT_PULLUP);

  lcd.begin(12, 13);// initialize the lcd with SDA and SCL pins
  lcd.backlight();

  // Servos

  servo1.attach(servo1Pin, minPulseWidth, maxPulseWidth);
  servo1.setTimerWidth(20);
  
  servo2.attach(servo2Pin, minPulseWidth, maxPulseWidth);
  servo2.setTimerWidth(20);
  
  servo3.attach(servo3Pin, minPulseWidth, maxPulseWidth);
  servo3.setTimerWidth(20);
  
  servo4.attach(servo4Pin, minPulseWidth, maxPulseWidth);
  servo4.setTimerWidth(20);
  
  // LED
  pinMode(pumpPin, OUTPUT);  // !! important for pump
}

int pulseWidthForAngle(float angle) {
  return 500 + (angle / 180.0) * 2000;
}


void loop() {
  // put your main code here, to run repeatedly:

  firstEncoder.readEncoder(ENCODER_1_CLK, ENCODER_1_DT, ENCODER_1_SW);
  secondEncoder.readEncoder(ENCODER_2_CLK, ENCODER_2_DT, ENCODER_2_SW);
  thirdEncoder.readEncoder(ENCODER_3_CLK, ENCODER_3_DT, ENCODER_3_SW);
  fourthEncoder.readEncoder(ENCODER_4_CLK, ENCODER_4_DT, ENCODER_4_SW);
  
  processState();

  if (currentlyPumpEnabled != beforePumpEnabled) {
    digitalWrite(pumpPin, currentlyPumpEnabled ? HIGH : LOW);
    beforePumpEnabled = currentlyPumpEnabled;
  }
  
//  servo1Angle = currentX;
//  servo2Angle = currentY;
//  servo3Angle = currentZ;
//  servo4Angle = currentAngle;
//  checkCoordinateLimits();

//  if (immediately) {
//    checkRealCoordinateLimits();
//    convertCoordinatesToAngles(realX, realY, realZ, realAngle);
//    servo1Angle = convertedServo1Angle;
//    servo2Angle = convertedServo2Angle;
//    servo3Angle = convertedServo3Angle;  
//    servo4Angle = convertedServo4Angle;
//  }

  movement();
  checkServoAngleLimits();

  loopPhase = (loopPhase + 1) % 20; 


//  if (loopPhase != 0) {
//    return;
//  }
  
  servo1.write(pulseWidthForAngle(servo1Angle));
  servo2.write(pulseWidthForAngle(servo2Angle));
  servo3.write(pulseWidthForAngle(servo3Angle));
  servo4.write(pulseWidthForAngle(servo4Angle)); 

  if (loopPhase == 0) {
    Serial.printf("Values: s1:%.3f s2:%.3f s3:%.3f s4:%.3f\n", servo1Angle, servo2Angle, servo3Angle, servo4Angle); 
  }

//  if ((servo1Angle != previousServo1Angle) || (servo2Angle != previousServo2Angle) || (servo3Angle != previousServo3Angle) || (servo4Angle != previousServo4Angle)) {
//    Serial.printf("Values: s1:%.2f s2:%.2f s3:%.2f s4:%.2f\n", servo1Angle, servo2Angle, servo3Angle, servo4Angle);
//  }

  previousServo1Angle = servo1Angle;
  previousServo2Angle = servo2Angle;
  previousServo3Angle = servo3Angle;
  previousServo4Angle = servo4Angle;
}






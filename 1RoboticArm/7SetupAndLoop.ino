
////////////////////////////////////////////////////////////////////////
// Main

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

  servo1.attach(23, minPulseWidth, maxPulseWidth);
  servo1.setTimerWidth(20);
  
  servo2.attach(27, minPulseWidth, maxPulseWidth);
  servo2.setTimerWidth(20);
  
  servo3.attach(22, minPulseWidth, maxPulseWidth);
  servo3.setTimerWidth(20);
  
  servo4.attach(15, minPulseWidth, maxPulseWidth);
  servo4.setTimerWidth(20);
  
  // LED
  pinMode(LED5, OUTPUT);  // !! important for pump
}




void loop() {
  // put your main code here, to run repeatedly:

  firstEncoder.readEncoder(ENCODER_1_CLK, ENCODER_1_DT, ENCODER_1_SW);
  secondEncoder.readEncoder(ENCODER_2_CLK, ENCODER_2_DT, ENCODER_2_SW);
  thirdEncoder.readEncoder(ENCODER_3_CLK, ENCODER_3_DT, ENCODER_3_SW);
  fourthEncoder.readEncoder(ENCODER_4_CLK, ENCODER_4_DT, ENCODER_4_SW);

  if (bluetoothModeEnabled) {
    processBluetoothLoop();
  }
  
  processState();

  if (currentlyPumpEnabled != beforePumpEnabled) {
    digitalWrite(LED5, currentlyPumpEnabled ? HIGH : LOW);
    beforePumpEnabled = currentlyPumpEnabled;
  }
  
//  servo1Angle = currentX;
//  servo2Angle = currentY;
//  servo3Angle = currentZ;
//  servo4Angle = currentAngle;
//  checkCoordinateLimits();

  if (currentState != ST_PLAY_PROGRAM) {
    convertCoordinatesToAngles(realX, realY, realZ, realAngle);
    servo1Angle = convertedServo1Angle;
    servo2Angle = convertedServo2Angle;
    servo3Angle = convertedServo3Angle;  
    servo4Angle = convertedServo4Angle;
  }

  checkServoAngleLimits();
  
  servo1.write(servo1Angle);
  servo2.write(servo2Angle);
  servo3.write(servo3Angle);
  servo4.write(servo4Angle); 
}






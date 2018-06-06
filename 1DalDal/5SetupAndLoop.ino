
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
  pinMode(ENCODER_4_SW, INPUT);

  lcd.begin(12, 13);// initialize the lcd with SDA and SCL pins
  lcd.backlight();

  // LED

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(LED5, OUTPUT);

//  digitalWrite(LED1, HIGH);
//  digitalWrite(LED2, HIGH);
//  digitalWrite(LED3, HIGH);
//  digitalWrite(LED4, HIGH);
//  digitalWrite(LED5, HIGH);

  ledcSetup(led1Channel, freq, resolution);
  ledcAttachPin(LED1, led1Channel);
  ledcWrite(led1Channel, 0);

  ledcSetup(led2Channel, freq, resolution);
  ledcAttachPin(LED2, led2Channel);
  ledcWrite(led2Channel, 0);

  ledcSetup(led3Channel, freq, resolution);
  ledcAttachPin(LED3, led3Channel);
  ledcWrite(led3Channel, 0);

  ledcSetup(led4Channel, freq, resolution);
  ledcAttachPin(LED4, led4Channel);
  ledcWrite(led4Channel, 0);

  ledcSetup(led5Channel, freq, resolution);
  ledcAttachPin(LED5, led5Channel);
  ledcWrite(led5Channel, 0);
}




void loop() {
  // put your main code here, to run repeatedly:

  firstEncoder.readEncoder(ENCODER_1_CLK, ENCODER_1_DT, ENCODER_1_SW);
  secondEncoder.readEncoder(ENCODER_2_CLK, ENCODER_2_DT, ENCODER_2_SW);
  thirdEncoder.readEncoder(ENCODER_3_CLK, ENCODER_3_DT, ENCODER_3_SW);
  fourthEncoder.readEncoder(ENCODER_4_CLK, ENCODER_4_DT, ENCODER_4_SW);

  if (wifiModeEnabled) {
    webServer.processClients(server);  
  }
  
  processState();

  ledcWrite(led1Channel, map(currentX, 0, 255, 0, 255));
  ledcWrite(led2Channel, map(currentY, 0, 255, 0, 255));
  ledcWrite(led3Channel, map(currentZ, 0, 255, 0, 255));
  ledcWrite(led4Channel, map(currentAngle, 0, 255, 0, 255));
}

////////////////////////////////////////////////////////////////////////
// Wifi Setup

WebServer webServer = WebServer();

////////////////////////////////////////////////////////////////////////
// Rotary Encoders

Encoder firstEncoder = Encoder(1);
Encoder secondEncoder = Encoder(2);
Encoder thirdEncoder = Encoder(3);
Encoder fourthEncoder = Encoder(4);

////////////////////////////////////////////////////////////////////////
// LCD Display

LiquidCrystal_I2C lcd(0x27,16,2);//set the LCD address to 0x27 for a 16 chars and 2 line display

////////////////////////////////////////////////////////////////////////
// States

StateMachine stateMachine = StateMachine();

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
//  lcd.setCursor(0,0);
//  lcd.print("Tucnak Buclik");
//  lcd.setCursor(0,1);
//  lcd.print("tlaci vajcko");

  webServer.connectToWifi();
  server.begin();
}




void loop() {
  // put your main code here, to run repeatedly:

  firstEncoder.readEncoder(ENCODER_1_CLK, ENCODER_1_DT, ENCODER_1_SW);
  secondEncoder.readEncoder(ENCODER_2_CLK, ENCODER_2_DT, ENCODER_2_SW);
  thirdEncoder.readEncoder(ENCODER_3_CLK, ENCODER_3_DT, ENCODER_3_SW);
  fourthEncoder.readEncoder(ENCODER_4_CLK, ENCODER_4_DT, ENCODER_4_SW);

  webServer.processClients(server);
  stateMachine.processState(firstEncoder, secondEncoder, thirdEncoder, fourthEncoder, lcd);
}

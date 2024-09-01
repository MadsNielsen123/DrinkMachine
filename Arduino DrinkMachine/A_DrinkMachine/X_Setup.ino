void setup()
{
  pinModeSetup();
  interruptSetup();
  Serial.begin(9600);
  Serial3.begin(9600);
  changeLight();
  tftShieldSetup();

}

void pinModeSetup()
{
  // PinModes Inputs
  pinMode(ENCODER_A, INPUT);
  pinMode(ENCODER_B, INPUT);
  pinMode(ENCODER_PRESS, INPUT);
  pinMode(RED_BTN, INPUT);
  pinMode(BLUE_BTN, INPUT);
  pinMode(MODE_BTN, INPUT);
  pinMode(STEPPER_HOME, INPUT);

  // PinModes Outputs
  pinMode(MOTOR_PWM, OUTPUT);
  pinMode(MOTOR_DIR, OUTPUT);
  pinMode(STEPPER_STEP, OUTPUT);
  pinMode(STEPPER_ENABLE, OUTPUT);
  pinMode(BUZZ_PIN, OUTPUT);
  pinMode(LED_PREPARE_PIN, OUTPUT);
}

void interruptSetup()
{
  //ExternalINTSetups
  EncoderPreA = digitalRead(ENCODER_A);
  EncoderPreB = digitalRead(ENCODER_B);
  attachInterrupt(digitalPinToInterrupt(ENCODER_A), EncoderTurn, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENCODER_PRESS), EncoderPress, FALLING);
  attachInterrupt(digitalPinToInterrupt(RED_BTN), RedButtonPress, FALLING);
  attachInterrupt(digitalPinToInterrupt(BLUE_BTN), BlueButtonPress, FALLING);
  attachInterrupt(digitalPinToInterrupt(STEPPER_HOME), StepperNowHome, RISING);

  //EncoderTimerINTSetup (5 ms)
  cli();
  TCCR4A = 0; //clear Timer/Counter Control Register 4A
  TCCR4B = 0; //clear Timer/Counter Control Register 4B
  TCCR4B |= B00000000; //Disable Timer for now
  TCNT4 = 0; //clear counter
  OCR4A = 77;//(~5ms with 1024 prescaler)
  TIMSK4 |= B00000010; //Enable timer compare interrupt
  sei();
}

void tftShieldSetup()
{
  //Inits
  tft.begin();
  tftWidth = tft.width(); //240
  tftHeight = tft.height(); //320
  tft.setTextWrap(false);
  tft.setRotation(2);

  //SD Init
  Serial.println("*Initializing SD card*");
  if (!SD.begin(SD_CC))
  {
    Serial.println(" - initialization failed!");
    while (1);
  }
  Serial.println(" - initialization done.");
}

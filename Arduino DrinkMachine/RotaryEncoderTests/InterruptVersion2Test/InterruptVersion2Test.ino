//Encoder
#define ENCODER_A 18 //Encoder's A output
#define ENCODER_B 46 //Encoder's B output

volatile bool encoderTurned = false; 
volatile bool EncoderNewA = 0;
volatile bool EncoderNewB = 0;
volatile bool EncoderPreA = 0;
volatile bool EncoderPreB = 0;
volatile int encoderValue = 0;

void setup() 
{
  //EncoderTimerINTSetup (10ms)
  cli();
  TCCR4A = 0; //clear Timer/Counter Control Register 4A
  TCCR4B = 0; //clear Timer/Counter Control Register 4B
  TCCR4B |= B00001101; //CTC mode with 1024 prescaler
  TCNT4 = 0; //clear counter
  OCR4A = 155;//(~10ms with 1024 prescaler)
  TIMSK4 |= B00000010; //Enable timer compare interrupt
  sei();
  
  pinMode(ENCODER_A, INPUT);
  pinMode(ENCODER_B, INPUT);
  EncoderPreA = digitalRead(ENCODER_A);
  EncoderPreB = digitalRead(ENCODER_B);
  
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(ENCODER_A), EncoderTurnISR, CHANGE);

}

void loop() 
{
  if(encoderTurned)
  {
    encoderTurned = false;
    Serial.println(encoderValue);
  }
}


void EncoderTurnISR()
{
  encoderTurned = true;
  EncoderNewA = digitalRead(ENCODER_A);
  EncoderNewB = digitalRead(ENCODER_B);

  //----------------- Check direction------------------
  if(!EncoderPreA && EncoderPreB && EncoderNewA && !EncoderNewB) //CW: 01 -> 10
    encoderValue++;

  if(EncoderPreA && !EncoderPreB && !EncoderNewA && EncoderNewB) //CW: 10 -> 01
    encoderValue++;

  if(EncoderPreA && EncoderPreB && !EncoderNewA && EncoderNewB) //CW: 11 -> 01
    encoderValue++;

  if(!EncoderPreA && !EncoderPreB && EncoderNewA && !EncoderNewB) //CW: 00 -> 10
    encoderValue++;

  if(!EncoderPreA && !EncoderPreB && EncoderNewA && EncoderNewB) //CCW: 00 -> 11
    encoderValue--;

  if(EncoderPreA && EncoderPreB && !EncoderNewA && !EncoderNewB) //CCW: 11 -> 00
    encoderValue--;

  if(EncoderPreA && !EncoderPreB && !EncoderNewA && !EncoderNewB) //CCW: 10 -> 00
    encoderValue--;

  if(!EncoderPreA && EncoderPreB && EncoderNewA && EncoderNewB) //CCW: 01 -> 11
    encoderValue--;

  EncoderPreA = EncoderNewA;
  EncoderPreB = EncoderNewB;

  // ------------ Activate Encoder Delay ------------
  bitClear(EIMSK, INT3); //Disable Encoder
  TCCR4B |= B00001101; //Enable Counter
}

ISR (TIMER4_COMPA_vect) //Enables encoder again after x secconds
{
  TCCR4B |= B00000000; //disable counter
  TCNT4 = 0; //clear counter
  
  bitSet(EIFR, 3); //Clear INTF4 (External Interrupt Flag 4)
  bitSet(EIMSK, INT3);  //Énable Encoder
}

/*
 * Increment number from 0 to 100 (InterruptBased)
 */

#define EncoderA 18 //Encoder's A output
#define EncoderB 46 //Encoder's B output

volatile bool EncoderState; //True if encoder position was changed
volatile bool EncoderChanged; //State of A output

int value = 0;

void setup() 
{
  Serial.begin(9600);
  pinMode(EncoderA, INPUT);
  pinMode(EncoderB, INPUT);
  attachInterrupt(digitalPinToInterrupt(18), EncoderTurnISR, RISING);
}

void loop() 
{
  if(EncoderChanged)
  {
    EncoderChanged = false;
    if(EncoderState != digitalRead(EncoderB))
      value++;
    else
      value--;
      
    Serial.println(value);    
  }
}

void EncoderTurnISR()
{
  EncoderChanged = true;
  EncoderState = digitalRead(EncoderA);
}

/*
 * Quick Test, increment number from 0 to 100
 */

#define EncoderA 45 //Encoder's A output
#define EncoderB 46 //Encoder's B output

bool EncoderState; //State of A output
bool LastEncoderState; //Last state of A output
int value = 0;
void setup() 
{
  Serial.begin(9600);
  pinMode(EncoderA, INPUT);
  pinMode(EncoderB, INPUT);

  LastEncoderState = digitalRead(EncoderA);
  Serial.print("Ready:" );

}

void loop() 
{
  //check if encoder was turned
  EncoderState = digitalRead(EncoderA);
  if(EncoderState != LastEncoderState)
  {    
   //if turned - Check direction and inc/dec value
   if(EncoderState == digitalRead(EncoderB)) //Counter-Clockwise
   {
    if(value != 0)
      value--;
   }
   else //Clockwise
   {
    if(value != 100)
      value++;
   }
   LastEncoderState = EncoderState;
   Serial.println(value);
  }
}

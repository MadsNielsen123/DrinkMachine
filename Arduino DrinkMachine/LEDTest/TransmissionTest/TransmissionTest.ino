#define LEDChangePin 27

void setup() {
  Serial3.begin(9600);
  pinMode(LEDChangePin, OUTPUT);
}

void loop() 
{
  digitalWrite(LEDChangePin, HIGH);
  delay(1);
  digitalWrite(LEDChangePin, LOW);
  Serial3.write('d');
  while(1);

}

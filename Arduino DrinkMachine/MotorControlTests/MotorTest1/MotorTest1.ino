#define PWM_PIN 45


void setup() 
{
  Serial.begin(9600);
  pinMode(PWM_PIN, OUTPUT);
}

void loop() 
{
  Serial.println("Beginning acceleration");
  for(int i = 30; i<=100; i++)
  {
    analogWrite(PWM_PIN, i);
    delay(15);
  }
  Serial.println("Max speed");
  
  while(true);

} 

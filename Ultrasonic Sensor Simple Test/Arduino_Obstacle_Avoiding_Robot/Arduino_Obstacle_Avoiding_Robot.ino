const int trig_1 = 6;
const int echo_1 = 11;
const int LM_IN1 = 8; //IN 1
const int LM_IN2 = 7; //IN2
const int RM_IN3 = 5; //IN 3
const int RM_IN4 = 4; //IN 4

int duration_1 = 0;
int distance_1 = 0;

void setup() 
{
  pinMode(trig_1 , OUTPUT);
  pinMode(echo_1 , INPUT);
  pinMode(LM_IN1 , OUTPUT);
  pinMode(LM_IN2 , OUTPUT);
  pinMode(RM_IN3 , OUTPUT);
  pinMode(RM_IN4 , OUTPUT);
  
  Serial.begin(9600);

}

void loop()
{
  digitalWrite(trig_1 , HIGH);
  delayMicroseconds(1000);
  digitalWrite(trig_1 , LOW);


  duration_1 = pulseIn(echo_1 , HIGH);
  distance_1 = (duration_1/2) / 28.5 ;
  Serial.println(distance_1);
  

  if ( distance_1 < 20 )
  {
    digitalWrite(LM_IN1 , LOW);
    digitalWrite(LM_IN2 , HIGH);
    digitalWrite(RM_IN3 , HIGH);
    digitalWrite(RM_IN4 , LOW);
    delay(100);
  }
  else
  {
    digitalWrite(LM_IN1 , HIGH);
    digitalWrite(LM_IN2 , LOW);
    digitalWrite(RM_IN3 , HIGH);
    digitalWrite(RM_IN4 , LOW);
  }
 
}

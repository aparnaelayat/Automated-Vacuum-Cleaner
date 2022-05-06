const int trig_1 = 0; //front
const int echo_1 = 1; //front
const int trig_2 = 15; //right when looking from the back
const int echo_2 = 14; //right
const int trig_3 = 19; //left
const int echo_3 = 18; //left
const int trig_4 = 17; //back
const int echo_4 = 16; //back

float distance_1 = 0;
float distance_2 = 0;
float distance_3 = 0;
float distance_4 = 0;

float check_dist(int trig, int echo);

void setup() {
  pinMode(trig_1 , OUTPUT);
  pinMode(echo_1 , INPUT);
  pinMode(trig_2 , OUTPUT);
  pinMode(echo_2 , INPUT);
  pinMode(trig_3 , OUTPUT);
  pinMode(echo_3 , INPUT);
  pinMode(trig_4 , OUTPUT);
  pinMode(echo_4 , INPUT);
  
  Serial.begin(9600);
//  check_dist(trig_1,echo_1);
//  check_dist(trig_2,echo_2);
//  check_dist(trig_3,echo_3);
//  check_dist(trig_4,echo_4);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print("distance_1: ");
  Serial.println(check_dist(trig_1,echo_1));

//  Serial.print("distance_2: ");
//  Serial.println(check_dist(trig_2,echo_2));
//
//  Serial.print("distance_3: ");
//  Serial.println(check_dist(trig_3,echo_3));
//
//  Serial.print("distance_4: ");
//  Serial.println(check_dist(trig_4,echo_4));

}

float check_dist(int trig,int echo)
{
  digitalWrite(trig , HIGH);
  delayMicroseconds(1000);
  digitalWrite(trig , LOW);

  float duration = pulseIn(echo, HIGH);
//  Serial.println(duration);
  float distance = duration/2/ 28.5 ;
//  Serial.println(distance);
  return distance;
}

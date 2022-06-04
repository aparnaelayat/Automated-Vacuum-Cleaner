const int trig_1 = 0; //front
const int echo_1 = 1; //front
const int trig_2 = 15; //right when looking from the back
const int echo_2 = 14; //right
const int trig_3 = 19; //left
const int echo_3 = 18; //left
const int trig_4 = 17; //back
const int echo_4 = 16; //back

const int buzz = 6;
unsigned long CurrentTime;
unsigned long ElapsedTime;
unsigned long StartTime;

const int LM_IN1 = 8; //IN 1
const int LM_IN2 = 7; //IN 2
const int RM_IN3 = 5; //IN 3
const int RM_IN4 = 4; //IN 4

double distance_1 = 0;
double distance_2 = 0;
double distance_3 = 0;
double distance_4 = 0;

int pos = 0;

double check_dist(int trig, int echo);

void movement_Inst_Fwd();
void movement_Inst_Rgt();
void movement_Inst_Bwd();
void movement_Inst_Lft();
void movement_Inst_Stp();

int curr1;
int reqTime;

void setup() 
{
  pinMode(trig_1 , OUTPUT);
  pinMode(echo_1 , INPUT);
  pinMode(trig_2 , OUTPUT);
  pinMode(echo_2 , INPUT);
  pinMode(trig_3 , OUTPUT);
  pinMode(echo_3 , INPUT);
  pinMode(trig_4 , OUTPUT);
  pinMode(echo_4 , INPUT);
  
  pinMode(LM_IN1 , OUTPUT);
  pinMode(LM_IN2 , OUTPUT);
  pinMode(RM_IN3 , OUTPUT);
  pinMode(RM_IN4 , OUTPUT);
  
  Serial.begin(9600);
}

void loop()
{
  distance_1= check_dist(trig_1,echo_1); //Checking Front
  Serial.print("distance_3:");
  Serial.println(distance_3);
  if ( distance_1 < 20 )
  {
    Serial.println("front blocked");
    distance_2= check_dist(trig_2,echo_2);
    Serial.print("distance_3:");
    Serial.println(distance_3);
    if ( distance_2 < 20 ) //Checking Right
    {
          Serial.println("right blocked");
          distance_3= check_dist(trig_2,echo_2);
          Serial.print("distance_3:");
          Serial.println(distance_3);
          if ( distance_3 < 20 )// Checking Left
          {
                    Serial.println("left blocked");
                    distance_4= check_dist(trig_4,echo_4);
                    Serial.print("distance_4:");
                    Serial.println(distance_4);
                    if ( distance_4 < 20 )// Checking Backward
                    {
                        Serial.println("completely blocked");
                        digitalWrite(buzz , HIGH); // Alerts user that the vacuum is stuck
                        delay(1000);
                        digitalWrite(buzz , LOW);
                    }
                    else
                    {
                      distance_2= check_dist(trig_2,echo_2);
                      distance_3= check_dist(trig_3,echo_3);
                      while(distance_2 < 20 && distance_3 < 20)
                      movement_Inst_Bwd();

                      if (distance_2 > 20 )
                      {
                        movement_Inst_Rgt();
                        StartTime = millis();
                        do
                        {
                          movement_Inst_Fwd();//Find this duration and use for the last forward to get back to path    
                          distance_3= check_dist(trig_3,echo_3); // Checking Left where the initial obstacle is
                        }while(distance_3 < 20);
                              
                        movement_Inst_Lft();
                        CurrentTime = millis();
                        ElapsedTime = CurrentTime - StartTime;
                              
                        do
                        {
                          movement_Inst_Fwd();
                          distance_3= check_dist(trig_3,echo_3); // Checking Left where the initial obstacle is
                          }while(distance_3 < 20);
                              
                          movement_Inst_Lft();
                          curr1=millis();
                          reqTime=curr1+ElapsedTime;
                          do
                          {
                            movement_Inst_Fwd();
                          }while(reqTime>millis());
                          movement_Inst_Rgt();
                          movement_Inst_Fwd();      
                      }
                      if(distance_3>20)
                      {
                        movement_Inst_Lft();
                        StartTime = millis();
                        do
                        {
                          movement_Inst_Fwd();//Find this duration and use for the last forward to get back to path    
                          distance_2= check_dist(trig_2,echo_2); // Checking Left where the initial obstacle is
                        }
                        while(distance_2 < 20);
                        movement_Inst_Rgt();
                        CurrentTime = millis();
                        ElapsedTime = CurrentTime - StartTime;
                        do
                        {
                          movement_Inst_Fwd();
                          distance_2= check_dist(trig_2,echo_2); // Checking Left where the initial obstacle is
                        }
                        while(distance_2 < 20);
                        movement_Inst_Rgt();
                        curr1=millis();
                        reqTime=curr1+ElapsedTime;
                        do
                        {
                          movement_Inst_Fwd();
                        }
                        while(reqTime>millis());
                        movement_Inst_Lft();
                        movement_Inst_Fwd();
                      }
                     
                    }
          }
          else
          {
            movement_Inst_Lft();
            StartTime = millis();
            do
            {
              movement_Inst_Fwd();//Find this duration and use for the last forward to get back to path    
              distance_2= check_dist(trig_2,echo_2); // Checking Left where the initial obstacle is
            }
            while(distance_2 < 20);
            movement_Inst_Rgt();
            CurrentTime = millis();
            ElapsedTime = CurrentTime - StartTime;
            do
            {
              movement_Inst_Fwd();
              distance_2= check_dist(trig_2,echo_2); // Checking Left where the initial obstacle is
            }
            while(distance_2 < 20);
            movement_Inst_Rgt();
            curr1=millis();
            reqTime=curr1+ElapsedTime;
            do
            {
              movement_Inst_Fwd();
            }
            while(reqTime>millis());
            movement_Inst_Lft();
            movement_Inst_Fwd();
          }
    }           
    else // Go right
    {
      movement_Inst_Rgt();
      StartTime = millis();
      do
      {
        movement_Inst_Fwd();//Find this duration and use for the last forward to get back to path    
        distance_3= check_dist(trig_3,echo_3); // Checking Left where the initial obstacle is
      }while(distance_3 < 20);
      
      movement_Inst_Lft();
      CurrentTime = millis();
      ElapsedTime = CurrentTime - StartTime;
      
      do
      {
        movement_Inst_Fwd();
        distance_3= check_dist(trig_3,echo_3); // Checking Left where the initial obstacle is
      }while(distance_3 < 20);
      
      movement_Inst_Lft();
      curr1=millis();
      reqTime=curr1+ElapsedTime;
      do
      {
        movement_Inst_Fwd();
      }while(reqTime>millis());
      
      movement_Inst_Rgt();
      movement_Inst_Fwd();
  }

  
}
else                    // Go forward
      movement_Inst_Fwd();
}

double check_dist(int trig,int echo)
{
  digitalWrite(trig , HIGH);
  delayMicroseconds(1000);
  digitalWrite(trig , LOW);

  double duration = pulseIn(echo , HIGH);
  double distance = (duration/2) / 28.5 ;
  return distance;
}

void movement_Inst_Fwd() {
  // forward movement instructions
  Serial.println("Going_Forward");
  digitalWrite(LM_IN1, HIGH);
  digitalWrite(LM_IN2, LOW);
  digitalWrite(RM_IN3, HIGH);
  digitalWrite(RM_IN4, LOW);
}

void movement_Inst_Lft() {
  // Left movement instructions
  Serial.println("Going_Left");
  digitalWrite(LM_IN1, LOW);
  digitalWrite(LM_IN2, LOW);
  digitalWrite(RM_IN3, HIGH);
  digitalWrite(RM_IN4, LOW);
  delay(1600);// default delay for smooth rotation of about 23 degrees is 500.
  digitalWrite(LM_IN1, LOW);
  digitalWrite(LM_IN2, LOW);
  digitalWrite(RM_IN3, LOW);
  digitalWrite(RM_IN4, LOW);
  delay(1600);
  // NOTE: The minimum delay for RIGHT/LEFT movement is 1S(inluding .5s ON time & .5s OFF time). Hence subtract 1s before repeating this movement
}

void movement_Inst_Rgt() {
  // Rgt movement instructions
  Serial.println("Going_Right"); 
  digitalWrite(LM_IN1, HIGH);
  digitalWrite(LM_IN2, LOW);
  digitalWrite(RM_IN3, LOW);
  digitalWrite(RM_IN4, LOW);
  delay(1600);// default delay for smooth rotation of about 45 degrees is 825.
  digitalWrite(LM_IN1, LOW);
  digitalWrite(LM_IN2, LOW);
  digitalWrite(RM_IN3, LOW);
  digitalWrite(RM_IN4, LOW);
  delay(1600);
  // NOTE: The minimum delay for RIGHT/LEFT movement is 1S(inluding .5s ON time & .5s OFF time). Hence subtract 1s before repeating this movement 

}

void movement_Inst_Bwd() {
  // Bwd movement instructions
  Serial.println("Going_Backward"); 
  digitalWrite(LM_IN1, LOW);
  digitalWrite(LM_IN2, HIGH);
  digitalWrite(RM_IN3, LOW);
  digitalWrite(RM_IN4, HIGH);
}

void movement_Inst_Stp() {
  // Stp movement instructions
  Serial.println("Stopping");
  digitalWrite(LM_IN1, LOW);
  digitalWrite(LM_IN2, LOW);
  digitalWrite(RM_IN3, LOW);
  digitalWrite(RM_IN4, LOW);
}

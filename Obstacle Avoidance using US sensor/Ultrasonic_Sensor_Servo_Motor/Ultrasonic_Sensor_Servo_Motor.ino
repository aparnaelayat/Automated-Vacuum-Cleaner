//Pin Declaration- All digital
const int trig_1 = 22; //front
const int echo_1 = 23; //front
const int trig_2 = 24; //right1 when looking from the back- side
const int echo_2 = 25; //right1- side
const int trig_5 = 26; //right2- corner
const int echo_5 = 27; //right2- corner
const int trig_3 = 28; //left1- side
const int echo_3 = 29; //left1- side
const int trig_6 = 30; //left2- corner
const int echo_6 = 31; //left2- corner
const int trig_4 = 32; //back
const int echo_4 = 33; //back

const int buzz = 34; //buzzer that goes off when vacuum is trapped

//Same as in path memorisation code
const int LM_IN1 = 8; //IN 1
const int LM_IN2 = 7; //IN 2
const int RM_IN3 = 5; //IN 3
const int RM_IN4 = 4; //IN 4

//Time variables like in path memorisation code
unsigned long CurrentTime;
unsigned long ElapsedTime;
unsigned long StartTime;
unsigned long curr1;
unsigned long reqTime;

//Distance variables
double distance_1 = 0;
double distance_2 = 0; //Right side
double distance_3 = 0; //Left side
double distance_4 = 0;
double distance_5 = 0; //Right corner
double distance_6 = 0; //Left corner

//Function Declarations
double check_dist(int trig, int echo);

void movement_Inst_Fwd();
void movement_Inst_Rgt();
void movement_Inst_Bwd();
void movement_Inst_Lft();
void movement_Inst_Stp();

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
  pinMode(trig_5 , OUTPUT);
  pinMode(echo_5 , INPUT);
  pinMode(trig_6 , OUTPUT);
  pinMode(echo_6 , INPUT);
  
  pinMode(LM_IN1 , OUTPUT);
  pinMode(LM_IN2 , OUTPUT);
  pinMode(RM_IN3 , OUTPUT);
  pinMode(RM_IN4 , OUTPUT);
  
  Serial.begin(9600);
}

void loop()
{
  distance_1= check_dist(trig_1,echo_1); //Checking Obstacles in Front
  Serial.print("distance_1:");
  Serial.println(distance_1);
  if ( distance_1 < 20 ) // Obstacle in front
  {
    Serial.println("Front is blocked");
    
    distance_2= check_dist(trig_2,echo_2); //Checking Right
    Serial.print("distance_2:");
    Serial.println(distance_2);
    
    distance_5= check_dist(trig_5,echo_5);
    Serial.print("distance_5:");
    Serial.println(distance_5);
    
    if ( distance_2 < 20 or distance_5 < 20) // Right Blocked
    {
          Serial.println("Right side is blocked");
          
          distance_3= check_dist(trig_3,echo_3);// Checking Left
          Serial.print("distance_3:");
          Serial.println(distance_3);
          
          distance_6= check_dist(trig_6,echo_6);
          Serial.print("distance_6:");
          Serial.println(distance_6);
          
          if ( distance_3 < 20 or distance_6 < 20) // Left Blocked
          {
                    Serial.println("Left side is blocked");
                    
                    distance_4= check_dist(trig_4,echo_4); //Checking backwards
                    Serial.print("distance_4:");
                    Serial.println(distance_4);
                    if ( distance_4 < 20 )// Back Blocked
                    {
                        Serial.println("Vacuum is blocked from all 4 sides and needs the attention of user. ");
                        digitalWrite(buzz , HIGH); // Alerts user that the vacuum is stuck
                        delay(1000); // The duration of buzzer can be increased after testing
                        digitalWrite(buzz , LOW);
                    }
                    
                    else // Back is not blocked
                    {
                      distance_2= check_dist(trig_2,echo_2); // Right- side
                      distance_5= check_dist(trig_5,echo_5); // Right- corner
                      
                      distance_3= check_dist(trig_3,echo_3); // Left- side
                      distance_6= check_dist(trig_6,echo_6); // Left- corner
                      
                      while((distance_2 < 20 or  distance_5 < 20) and (distance_3 < 20 or distance_6 < 20)) // Right and Left Blocked
                      movement_Inst_Bwd();

                      if (distance_2 > 20 and  distance_5 > 20) // Right not blocked
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
                      
                      if(distance_3>20 &&  distance_6 > 20) // Left not blocked
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

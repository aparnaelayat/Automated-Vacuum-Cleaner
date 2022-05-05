#include <Servo.h>

const int trig_1 = 14;
const int echo_1 = 15;
const int trig_2 = 16;
const int echo_2 = 17;
const int trig_3 = 18;
const int echo_3 = 19;
const int trig_4 = 20;
const int echo_4 = 21;

const int buzz = 6;
unsigned long CurrentTime;
unsigned long ElapsedTime;
unsigned long StartTime;

const int LM_IN1 = 8; //IN 1
const int LM_IN2 = 7; //IN2
const int RM_IN3 = 5; //IN 3
const int RM_IN4 = 4; //IN 4

int duration_1 = 0;
int duration_2 = 0;
int duration_3 = 0;
int duration_4 = 0;

int distance_1 = 0;
int distance_2 = 0;
int distance_3 = 0;
int distance_4 = 0;

int pos = 0;

int check_dist();
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
  distance_1= check_dist(); //Checking Front Sensor
  if ( distance_1 < 20 )
  {
    distance_2= check_dist();
    if ( distance_2 < 20 ) //Checking Right Sensor
    {
          distance_3= check_dist();
          if ( distance_3 < 20 )// Checking Left Sensor
          {
                    distance_4= check_dist();
                    if ( distance_4 < 20 )// Checking Back Sensor
                    {
                        digitalWrite(buzz , HIGH); // Alerts user that the vacuum is stuck
                        delay(1000);
                        digitalWrite(buzz , LOW);
                    }
                    else // Go Back
                    {
                      distance_2= check_dist();
                      distance_3= check_dist();
                      while(distance_2 < 20 && distance_3 < 20)
                        movement_Inst_Bwd();

                      if (distance_2 > 20 )
                      {
                        movement_Inst_Rgt();
                        StartTime = millis();
                        do
                        {
                          movement_Inst_Fwd();//Find this duration and use for the last forward to get back to path    
                          distance_3= check_dist(); // Checking Left where the initial obstacle is
                        }while(distance_3 < 20);
                        CurrentTime = millis();                             
                        movement_Inst_Lft();

                        ElapsedTime = CurrentTime - StartTime;
                              
                        do
                        {
                          movement_Inst_Fwd();
                          distance_3= check_dist(); // Checking Left where the initial obstacle is
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
                          distance_2= check_dist(); // Checking Left where the initial obstacle is
                        }
                        while(distance_2 < 20);
                        CurrentTime = millis();
                        movement_Inst_Rgt();

                        ElapsedTime = CurrentTime - StartTime;
                        do
                        {
                          movement_Inst_Fwd();
                          distance_2= check_dist(); // Checking Left where the initial obstacle is
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
          else //go left
          {
            movement_Inst_Lft();
            StartTime = millis();
            do
            {
              movement_Inst_Fwd();//Find this duration and use for the last forward to get back to path    
              distance_2= check_dist(); // Checking Left where the initial obstacle is
            }
            while(distance_2 < 20);
            CurrentTime = millis();
            movement_Inst_Rgt();

            ElapsedTime = CurrentTime - StartTime;
            do
            {
              movement_Inst_Fwd();
              distance_2= check_dist(); // Checking Left where the initial obstacle is
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
        distance_3= check_dist(); // Checking Left where the initial obstacle is
      }while(distance_3 < 20);
      
      CurrentTime = millis();
      movement_Inst_Lft();
      ElapsedTime = CurrentTime - StartTime;
      
      do
      {
        movement_Inst_Fwd();
        distance_3= check_dist(); // Checking Left where the initial obstacle is
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

int check_dist()
{
  digitalWrite(trig_1 , HIGH);
  delayMicroseconds(1000);
  digitalWrite(trig_1 , LOW);

  duration_1 = pulseIn(echo_1 , HIGH);
  distance_1 = (duration_1/2) / 28.5 ;
  Serial.println(distance_1);
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

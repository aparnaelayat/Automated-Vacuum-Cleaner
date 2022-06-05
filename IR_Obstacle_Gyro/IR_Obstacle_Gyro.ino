#include <IRremote.h>// IR remote library
#include <EEPROM.h>// EEPROM pre-installed library

//Pin 9 of mega does not work. Use pin 12 instead. 

/*
   ***********Left Motor***********
*/
// IN 1
const int LM_IN1 =  8; //to pin 2 of L293D (IN 1)
// IN 2
const int LM_IN2 =  7; //to pin 7 of L293D (IN 2)

/*
   ************Right Motor************
*/
// IN 3
const int RM_IN3 = 5; //to pin 10 of L293D (IN 3)
// IN 4
const int RM_IN4 = 4; //to pin 15 of L293D (IN 4)

/*
   ************Enables************
*/
const int enA = 12; //Right motor (looking from the back, the projected thingy), to pin 1 of L293D (EN A)- Needs to be PWM 
const int enB = 3; //Left motor, to pin 9 of L293D (EN B)- Needs to be PWM 

/*
   ********************IR Receiver**********************
*/
const int RECV_PIN = 10;// Pin to which left pin (pin 1) of IR reciever is connected. Ground of IR is the central pin and the Vcc (5V) is the right pin. 

/*
 ************Arduino Reset Pin**************
 */
const int RESET_PIN = A0;

IRrecv irrecv(RECV_PIN);// Pass the pin number to the function
decode_results results;// variable results stores the decoded hex values

/*
 ************HEX code of all the buttons used in the project**************
 */
#define FWD       0x117DD // go forward(2) 
#define LFT       0x197CD // go left(4) 
#define RGT       0x157D5 // go right(6) 
#define BWD       0x1D7C5 // go backward(8) 
#define STOP      0x57F5 // stop(5) 0xFD30CF
#define RPEAT     0x137D9// repeat the stored sequence of movement from the temporary memory(automatically stores) (0)  
#define DEL       0x17FD// delete the stored sequence of movement from temporary memory(1)  
#define PERST     0x97ED // save from temp. memory to the permanent memory(3) 
#define PLAYEPROM 0xD7E5 // repeat the sequence stored in memory(7) 
#define RESET     0x37F9// resets the Arduino Board(9)  

/*
 ************Global Variables and Arrays**************
 */
unsigned long int value = 0; // stores the incoming hex value
byte seq = 0; //stores the current number of executed sequences
byte seq_Array[50];// array to store the movement sequence in terms of integers(1 for FWD, 2 for LEFT and so on..)

//counter for counting the number of times program pass through a movement function(fwd, lft etc.)
int fwd_Counter = -1;
int lft_Counter = -1;
int rgt_Counter = -1;
int bwd_Counter = -1;
int stp_Counter = -1;

//global "current time" variables for different movement functions(fwd, lft etc.)
unsigned long int current_Time0 = 0;// for FWD movement
unsigned long int current_Time1 = 0;// for LEFT movement
unsigned long int current_Time2 = 0;// for RIGHT movement
unsigned long int current_Time3 = 0;// for BWD movement
unsigned long int current_Time4 = 0;// for STOP

//total time spend by the pgm in executing the movement(fwd, lft etc.) for a particular movement counter
unsigned long int total_Fwd_Time[10];
unsigned long int total_Lft_Time[10];
unsigned long int total_Rgt_Time[10];
unsigned long int total_Bwd_Time[10];
unsigned long int total_Stp_Time[10];

/////Obstacle detection//////////

//Pin Declaration- digital pins
const int trig_1 = 11; //front
const int echo_1 = 10; //front
const int trig_2 = 15; //right when looking from the back
const int echo_2 = 14; //right
const int trig_3 = 19; //left
const int echo_3 = 18; //left
const int trig_4 = 17; //back
const int echo_4 = 16; //back

const int buzz = 34; //buzzer that goes off when vacuum is trapped from all 4 sides

//Motor pins defined above
 
//Time variables for Obstacle Detection
unsigned long CurrentTime;
unsigned long ElapsedTime;
unsigned long StartTime;
unsigned long curr1;
unsigned long reqTime;

//Distance variables for Obstacle Detection
double distance_1 = 0;
double distance_2 = 0; //Right side
double distance_3 = 0; //Left side
double distance_4 = 0;

//Function Declarations for Obstacle Detection
double check_dist(int trig, int echo);
void movement_Inst_Rgt_Obstacle();
void movement_Inst_Lft_Obstacle();
void Obstacle_Detection();
//rest are already defined above

void setup() {

  //Ultrasonic sensor pins setup
  
  pinMode(trig_1 , OUTPUT);
  pinMode(echo_1 , INPUT);
  pinMode(trig_2 , OUTPUT);
  pinMode(echo_2 , INPUT);
  pinMode(trig_3 , OUTPUT);
  pinMode(echo_3 , INPUT);
  pinMode(trig_4 , OUTPUT);
  pinMode(echo_4 , INPUT);
  
  // set mode of the pins as output
  pinMode(LM_IN1, OUTPUT);
  pinMode(LM_IN2, OUTPUT);
  pinMode(RM_IN3, OUTPUT);
  pinMode(RM_IN4, OUTPUT);
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(RECV_PIN, INPUT);
  
  // start serial communication
  Serial.begin(9600);
  // In case the interrupt driver crashes on setup, give a clue to the user what's going on.
  //Serial.println("Enabling IRin");
  irrecv.enableIRIn(); // Start the receiver
  //Serial.println("Enabled IRin");
  Serial.println("Commands:\n 2 to move forward\n 4 to move left\n 6 to move right\n 8 to move backward\n 5 to Stop\n 0 to repeat from temporary  memory \n DISPLAY to save path\n Menu to repeat saved path from memory\n Power to Delete stored path,\n Mute to Reset the Arduino");
}

void loop() {
  
  //Speed Control
  analogWrite(enA, 243);//Right motor
  analogWrite(enB, 220);//Left motor
  if (irrecv.decode(&results)) {
    value = results.value;
    Serial.println(value, HEX);
    delay(100);
    irrecv.resume(); // Receive the next value
    delay(100);
  }
  delay(100);
  check_Inst(value);
  value=0;
}

void check_Inst(long int value) 
{
  switch (value) 
  {
    case FWD:
      go_Forward();
      delay(10);
      break;
    case LFT:
      go_Left();
      delay(10);
      break;
    case RGT:
      go_Right();
      delay(10);
      break;
    case BWD:
      go_Backward();
      delay(10);
      break;
    case STOP:
      go_Stop();
      delay(10);
      break;
    case RPEAT:
      go_In_Seq();
      delay(10);
      break;
    case DEL:
      del_From_Local_Mem();
      delay(10);
      break;
    case PERST:
      write_To_Permt_Mem();
      delay(10);
      break;  
    case PLAYEPROM:
      Read_Permt_Mem();
      delay(10);
      break;   
    case RESET:
      Serial.println("Resetting Stored Path");
      pinMode(RESET_PIN, OUTPUT);
      digitalWrite(RESET_PIN,HIGH);   
      break;
                
     default:
      value = 0;
  }
}

void go_Forward() 
{
  movement_Inst_Fwd();
  current_Time0 = millis();
  int i = seq_Array[(seq - 1)];
  switch (i) 
  {
    case 2:
      // total time elaspsed since Left button is pressed including rest time 
      total_Lft_Time[lft_Counter + 1] = (current_Time0 - current_Time1);
      lft_Counter++;
      break;

    case 3:
      // total time elapsed since Right button is pressed including rest time 
      total_Rgt_Time[rgt_Counter + 1] = (current_Time0 - current_Time2);
      rgt_Counter++;
      break;

    case 4:
      total_Bwd_Time[bwd_Counter + 1] = (current_Time0 - current_Time3);
      bwd_Counter++;
      break;

    case 5:
      total_Stp_Time[stp_Counter + 1] = (current_Time0 - current_Time4);
      stp_Counter++;
      break;
  }
  seq_Array[seq] = 1;
  seq++;
}

void go_Left() 
{
  movement_Inst_Lft();
  current_Time1 = millis();
  int i = seq_Array[(seq - 1)];
  switch (i) 
  {
    case 1:
      total_Fwd_Time[fwd_Counter + 1] = (current_Time1 - current_Time0);
      fwd_Counter++;
      break;

    case 3:
      total_Rgt_Time[rgt_Counter + 1] = (current_Time1 - current_Time2);
      rgt_Counter++;
      break;

    case 4:
      total_Bwd_Time[bwd_Counter + 1] = (current_Time1 - current_Time3);
      bwd_Counter++;
      break;

    case 5:
      total_Stp_Time[stp_Counter + 1] = (current_Time1 - current_Time4);
      stp_Counter++;
      break;
  }
  seq_Array[seq] = 2;
  seq++;
}

void go_Right() 
{
  movement_Inst_Rgt();
  current_Time2 = millis();
  int i = seq_Array[(seq - 1)];
  switch (i) 
  {
    case 1:
      total_Fwd_Time[fwd_Counter + 1] = (current_Time2 - current_Time0);
      fwd_Counter++;
      break;

    case 2:
      total_Lft_Time[lft_Counter + 1] = (current_Time2 - current_Time1);
      lft_Counter++;
      break;

    case 4:
      total_Bwd_Time[bwd_Counter + 1] = (current_Time2 - current_Time3);
      bwd_Counter++;
      break;

    case 5:
      total_Stp_Time[stp_Counter + 1] = (current_Time2 - current_Time4);
      stp_Counter++;
      break;
  }
  seq_Array[seq] = 3;
  seq++;
}

void go_Backward() 
{
  movement_Inst_Bwd();
  current_Time3 = millis();
  int i = seq_Array[(seq - 1)];
  switch (i) 
  {
    case 1:
      total_Fwd_Time[fwd_Counter + 1] = (current_Time3 - current_Time0);
      fwd_Counter++;
      break;

    case 2:
      total_Lft_Time[lft_Counter + 1] = (current_Time3 - current_Time1);
      lft_Counter++;
      break;

    case 3:
      total_Rgt_Time[rgt_Counter + 1] = (current_Time3 - current_Time2);
      rgt_Counter++;
      break;

    case 5:
      total_Stp_Time[stp_Counter + 1] = (current_Time3 - current_Time4);
      stp_Counter++;
      break;
  }
  seq_Array[seq] = 4;
  seq++;
}

void go_Stop() 
{
  movement_Inst_Stp();
  current_Time4 = millis();
  int i = seq_Array[(seq - 1)];
  switch (i) 
  {
    case 1:
      total_Fwd_Time[fwd_Counter + 1] = (current_Time4 - current_Time0);
      fwd_Counter++;
      break;

    case 2:
      total_Lft_Time[lft_Counter + 1] = (current_Time4 - current_Time1);
      lft_Counter++;
      break;

    case 3:
      total_Rgt_Time[rgt_Counter + 1] = (current_Time4 - current_Time2);
      rgt_Counter++;
      break;

    case 4:
      total_Bwd_Time[bwd_Counter + 1] = (current_Time4 - current_Time3);
      bwd_Counter++;
      break;
  }
  seq_Array[seq] = 5;
  seq++;
}

void go_In_Seq(void) 
{
  Serial.println("Repeating sequence");
  value = 0;
  for (int i = 0; i < (seq + 1); i++) 
  {
    // Obstacle detection part
    //First Checking for an obstacle in front if yes call the obstacle detection function
    
    distance_1= check_dist(trig_1,echo_1); //Checking Obstacles in Front
    Serial.print("distance_1:");
    Serial.println(distance_1);
    Serial.print("\n");

    if ( distance_1 < 20 ) // Obstacle in front
    {
      void Obstacle_Detection();
    }
    
    // Path memorization
    
    int value1 = 0;
    value1 = seq_Array[i];
    switch (value1) 
    {
      case 1:
        static int j = 0;
        go_Forward_Seq(j);
        j++;
        break;
      case 2:
        static int k = 0;
        go_Left_Seq(k);
        k++;
        break;
      case 3:
        static int l = 0;
        go_Right_Seq(l);
        l++;
        break;
      case 4:
        static int m = 0;
        go_Backward_Seq(m);
        m++;
        break;
      case 5:
        static int n = 0;
        go_Stop_Seq(n);
        n++;
        break;
      default:
        j = 0; k = 0; l = 0; m = 0; n = 0;
    }
  }
}

void del_From_Local_Mem() 
{
  //set the movement counters to their default values
  Serial.println("Deleting Stored Path");
  fwd_Counter = -1;
  lft_Counter = -1;
  rgt_Counter = -1;
  bwd_Counter = - 1;
  stp_Counter = - 1;

  //set the total movement time to its default value
  for (int i = 0; i < 10; i++) 
  {
    total_Fwd_Time[i] = 0;
    total_Lft_Time[i] = 0;
    total_Rgt_Time[i] = 0;
    total_Bwd_Time[i] = 0;
    total_Stp_Time[i] = 0;
  }

  // Reset the sequence array(stored movement instructions)
  for (int i = 0; i < 50; i++) 
    seq_Array[i] = 0;
  seq = 0; 
}


/**********************************************************************************************************
     This function copy the data from the arrays to the EEPROM(permanent memory)
************************************************************************************************************/

void write_To_Permt_Mem()
{
  // total number of movement is stored in a random address i.e, 100
  Serial.println("Saving Path");
  EEPROM.write(100,seq);
    int i;
    
  //writing the movement sequence
  for(i=0; i<seq; i++)
    EEPROM.write(2*i,seq_Array[i]);

  //storing the time bw two successive movements
  for(i=1; i<seq+1; i++)
  {           
    if(seq_Array[i-1]==1)
    {
      static byte a=0;
      EEPROM.write(2*i-1,(total_Fwd_Time[a])/1000);// Note: One location can store maximum value of 255, hence the time is divided by 1000 here. And then multiplied by 1000 while retreiving the data from EEPROM location
      a++;
    }
    else if(seq_Array[i-1]==2)
    {
      static byte b=0;
      EEPROM.write(2*i-1,(total_Lft_Time[b])/1000);
      b++;
    }
    else if(seq_Array[i-1]==3)
    {
      static byte c=0;
      EEPROM.write(2*i-1,(total_Rgt_Time[c])/1000);
      c++;
    }
    else if(seq_Array[i-1]==4)
    {
      static byte d=0;
      EEPROM.write(2*i-1,(total_Bwd_Time[d])/1000);  
      d++;
    }
    else if(seq_Array[i-1]==5)
    {
      static byte e=0;
      EEPROM.write(2*i-1,(total_Stp_Time[e])/1000);  
      e++;
    }             
  }
  EEPROM.write(2*i-2,5);
  EEPROM.write(2*i-1,1);
} 

 
/**********************************************************************************************************
     This function reads the stored sequence from the EEPROM(permanent memory)
************************************************************************************************************/

void Read_Permt_Mem()
{
  // Read from permanent memory
  byte x = EEPROM.read(100);
  Serial.println("Repeating stored sequence from memory"); 

   for(int i=0; i<x+1; i++){
    byte r = EEPROM.read(2*i);
    switch(r)
    {
      case 1:
        movement_Inst_Fwd();
        break;
      case 2:
        movement_Inst_Lft();
        break;
      case 3:
        movement_Inst_Rgt();
        break;
      case 4:
        movement_Inst_Bwd();
        break; 
      case 5:
        movement_Inst_Stp();
        break;                          
     }
     delay((EEPROM.read(i+1))*1000);    // multiplied by thousand because the original time was divided by 1000 while storing in EEPROM.
   }
}
 
/**********************************************************************************************************
     These function moves the car in a direction for the time specified/stored in the total_x_time array
************************************************************************************************************/
void go_Forward_Seq(int j) 
{
  //go in forward direction sequence
  movement_Inst_Fwd();
  delay(total_Fwd_Time[j]);
}

void go_Left_Seq(int k) 
{
  //go in Left direction sequence
  movement_Inst_Lft();
  delay(total_Lft_Time[k]);
}

void go_Right_Seq(int l) 
{
  //go in right direction sequence
  movement_Inst_Rgt();
  delay(total_Rgt_Time[l]);
}

void go_Backward_Seq(int m) 
{
  //go in backward direction sequence
  movement_Inst_Bwd();
  delay(total_Bwd_Time[m]);
}

void go_Stop_Seq(int n) 
{
  //go in Stop sequence
  movement_Inst_Stp();
  delay(total_Stp_Time[n]);
}

/**********************************************************************************************************
     This function finds the distance between an ultrasonic sensor and an obstacle
************************************************************************************************************/

double check_dist(int trig,int echo)
{
  digitalWrite(trig , HIGH);
  delayMicroseconds(1000);
  digitalWrite(trig , LOW);

  double duration = pulseIn(echo , HIGH);
  double distance = (duration/2) / 28.5 ;
  return distance;
}

/*********************************************************************************************
          These movement instruction are repeated(required) several times in the code
**********************************************************************************************/
void movement_Inst_Fwd(void) 
{
  // forward movement instructions
  Serial.println("Going_Forward");
  digitalWrite(LM_IN1, HIGH);
  digitalWrite(LM_IN2, LOW);
  digitalWrite(RM_IN3, HIGH);
  digitalWrite(RM_IN4, LOW);
}

void movement_Inst_Lft(void) 
{
  // Left movement instructions
  Serial.println("Going_Left");
  digitalWrite(LM_IN1, LOW);
  digitalWrite(LM_IN2, LOW);
  digitalWrite(RM_IN3, HIGH);
  digitalWrite(RM_IN4, LOW);
  delay(800);// default delay for smooth rotation of about 23 degrees is 500.
  digitalWrite(LM_IN1, LOW);
  digitalWrite(LM_IN2, LOW);
  digitalWrite(RM_IN3, LOW);
  digitalWrite(RM_IN4, LOW);
  delay(800);
  // NOTE: The minimum delay for RIGHT/LEFT movement is 1S(inluding .5s ON time & .5s OFF time). Hence subtract 1s before repeating this movement
}

void movement_Inst_Rgt(void) 
{
  // Rgt movement instructions
  Serial.println("Going_Right"); 
  digitalWrite(LM_IN1, HIGH);
  digitalWrite(LM_IN2, LOW);
  digitalWrite(RM_IN3, LOW);
  digitalWrite(RM_IN4, LOW);
  delay(825);// default delay for smooth rotation of about 45 degrees is 825.
  digitalWrite(LM_IN1, LOW);
  digitalWrite(LM_IN2, LOW);
  digitalWrite(RM_IN3, LOW);
  digitalWrite(RM_IN4, LOW);
  delay(825);
  // NOTE: The minimum delay for RIGHT/LEFT movement is 1S(inluding .5s ON time & .5s OFF time). Hence subtract 1s before repeating this movement 

}

void movement_Inst_Bwd(void) 
{
  // Bwd movement instructions
  Serial.println("Going_Backward"); 
  digitalWrite(LM_IN1, LOW);
  digitalWrite(LM_IN2, HIGH);
  digitalWrite(RM_IN3, LOW);
  digitalWrite(RM_IN4, HIGH);
}

void movement_Inst_Stp(void) 
{
  // Stp movement instructions
  Serial.println("Stopping");
  digitalWrite(LM_IN1, LOW);
  digitalWrite(LM_IN2, LOW);
  digitalWrite(RM_IN3, LOW);
  digitalWrite(RM_IN4, LOW);
}

/*********************************************************************************************
          Going Left during Obstacle detection(different delay)
**********************************************************************************************/
void movement_Inst_Lft_Obstacle() {
  // Left movement instructions
  Serial.println("Going_Left\n");
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
/*********************************************************************************************
          Going Right during Obstacle detection(different delay)
**********************************************************************************************/
void movement_Inst_Rgt_Obstacle() {
  // Rgt movement instructions
  Serial.println("Going_Right\n"); 
  digitalWrite(LM_IN1, HIGH);
  digitalWrite(LM_IN2, LOW);
  digitalWrite(RM_IN3, LOW);
  digitalWrite(RM_IN4, LOW);
  delay(1600);// default delay for smooth rotation of about 45 degrees is 825. Here we need 90 degree turns. 
  digitalWrite(LM_IN1, LOW);
  digitalWrite(LM_IN2, LOW);
  digitalWrite(RM_IN3, LOW);
  digitalWrite(RM_IN4, LOW);
  delay(1600);
  // NOTE: The minimum delay for RIGHT/LEFT movement is 1S(inluding .5s ON time & .5s OFF time). Hence subtract 1s before repeating this movement 

}
/*********************************************************************************************
          Obstacle Detection Code
**********************************************************************************************/
void Obstacle_Detection()
{
  distance_1= check_dist(trig_1,echo_1); //Checking Obstacles in Front
  Serial.print("distance_1:");
  Serial.println(distance_1);
  Serial.print("\n");

  if ( distance_1 < 20 ) // Obstacle in front
  {
    Serial.println("Front is blocked\n");
    
    distance_2= check_dist(trig_2,echo_2); //Checking Right
    Serial.print("distance_2:");
    Serial.println(distance_2);
    Serial.print("\n");
    
    if ( distance_2 < 20 ) // Right Blocked
    {
          Serial.println("Right side is blocked\n");
          
          distance_3= check_dist(trig_3,echo_3);// Checking Left
          Serial.print("distance_3:");
          Serial.println(distance_3);
          Serial.print("\n");

          if ( distance_3 < 20) // Left Blocked
            {
                    Serial.println("Left side is blocked\n");
                    
                    distance_4= check_dist(trig_4,echo_4); //Checking backwards
                    Serial.print("distance_4:");
                    Serial.println(distance_4);
                    Serial.print("\n");

                    if ( distance_4 < 20 )// Back Blocked
                    {
                        Serial.println("Vacuum is blocked from all 4 sides and needs the attention of user. \n");
                        digitalWrite(buzz , HIGH); // Alerts user that the vacuum is stuck
                        delay(1000); // The duration of buzzer can be increased after testing
                        digitalWrite(buzz , LOW);
                    }
                    
                    else // Back is not blocked
                    {
                      distance_2= check_dist(trig_2,echo_2); // Right- side                      
                      distance_3= check_dist(trig_3,echo_3); // Left- side
                                 
                      while((distance_2 < 20) and (distance_3 < 20)) // Move back while both right and left are blocked
                        movement_Inst_Bwd();

                      if (distance_2 > 20 ) // Right is not blocked now
                      {
                        movement_Inst_Rgt_Obstacle();
                        StartTime = millis();
                        do
                        {
                          movement_Inst_Fwd();//Find this duration and use for the last-forward-command to get back on path.     
                          distance_3= check_dist(trig_3,echo_3); // Checking Left where the obstacle that was initial in right, now, is
                        }while(distance_3 < 20); // Ultrasonic still detects the obstacle that was initially in right (now in left)
                        //Haven't fully passed the obstacle that was in right initially, obstacle out of range of ultrasonic but might still be in right of vacuum. 
                        //If so, add forward movement for say 1 second more.  
                              
                        movement_Inst_Lft_Obstacle(); //Going along obstacle. Obstacle is now in close left of the vacuum. 
                        CurrentTime = millis();
                        ElapsedTime = CurrentTime - StartTime;
                              
                        do
                        {
                          movement_Inst_Fwd();
                          distance_3= check_dist(trig_3,echo_3); // Checking left where the initial obstacle is
                        }while(distance_3 < 20);
                              
                          movement_Inst_Lft_Obstacle(); // Going around obstacle. 
                          curr1=millis();
                          reqTime=curr1+ElapsedTime;
                          do
                          {
                            movement_Inst_Fwd();
                          }while(reqTime>millis());
                          movement_Inst_Rgt_Obstacle();
                          movement_Inst_Fwd();      
                      }
                      
                      else if(distance_3>20) // Left not blocked now
                      {
                        movement_Inst_Lft_Obstacle();
                        StartTime = millis();
                        do
                        {
                          movement_Inst_Fwd();//Find this duration and use for the last forward to get back to path    
                          distance_2= check_dist(trig_2,echo_2); // Checking Right where the initial obstacle is
                        }
                        while(distance_2 < 20);
                        movement_Inst_Rgt_Obstacle();
                        CurrentTime = millis();
                        ElapsedTime = CurrentTime - StartTime;
                        do
                        {
                          movement_Inst_Fwd();
                          distance_2= check_dist(trig_2,echo_2); // Checking Left where the initial obstacle is
                        }
                        while(distance_2 < 20 );
                        movement_Inst_Rgt_Obstacle();
                        curr1=millis();
                        reqTime=curr1+ElapsedTime;
                        do
                        {
                          movement_Inst_Fwd();
                        }
                        while(reqTime>millis());
                        movement_Inst_Lft_Obstacle();
                        movement_Inst_Fwd();
                      }
                     
                    }
          }
          else //Front and right are blocked but left is not blocked
          {
            movement_Inst_Lft_Obstacle();
            StartTime = millis();
            do
            {
              movement_Inst_Fwd();//Find this duration and use for the last forward to get back to path    
              distance_2= check_dist(trig_2,echo_2); // Checking Right where the initial obstacle is
            }
            while(distance_2 < 20); 
            movement_Inst_Rgt_Obstacle();
            CurrentTime = millis();
            ElapsedTime = CurrentTime - StartTime;
            do
            {
              movement_Inst_Fwd();
              distance_2= check_dist(trig_2,echo_2); // Checking Left where the initial obstacle is
            }
            while(distance_2 < 20);
            movement_Inst_Rgt_Obstacle();
            curr1=millis();
            reqTime=curr1+ElapsedTime;
            do
            {
              movement_Inst_Fwd();
            }
            while(reqTime>millis());
            movement_Inst_Lft_Obstacle();
            movement_Inst_Fwd();
          }
    }           
    else // Front blocked but right is not blocked
    {
      movement_Inst_Rgt_Obstacle();
      StartTime = millis();
      do
      {
        movement_Inst_Fwd();//Find this duration and use for the last forward to get back on path    
        distance_3= check_dist(trig_3,echo_3); // Checking Left where the initial obstacle is
      }while(distance_3 < 20);
      
      movement_Inst_Lft_Obstacle();
      CurrentTime = millis();
      ElapsedTime = CurrentTime - StartTime;
      
      do
      {
        movement_Inst_Fwd();
        distance_3= check_dist(trig_3,echo_3); // Checking Left where the initial obstacle is
      }while(distance_3 < 20);
      
      movement_Inst_Lft_Obstacle();
      curr1=millis();
      reqTime=curr1+ElapsedTime;
      do
      {
        movement_Inst_Fwd();
      }while(reqTime>millis());
      
      movement_Inst_Rgt_Obstacle();
      movement_Inst_Fwd();
    }
  }
  else                    // No obstacle in front so go forward
      movement_Inst_Fwd();
}

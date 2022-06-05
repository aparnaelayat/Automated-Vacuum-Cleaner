#include <Wire.h>
//Motor A
#define motorPin1  8  
#define motorPin2  7  
//Motor B
#define motorPin3  4 
#define motorPin4  5
//Declaring some global variables
int gyro_x, gyro_y, gyro_z;
long gyro_x_cal, gyro_y_cal, gyro_z_cal;
boolean set_gyro_angles;
long acc_x, acc_y, acc_z, acc_total_vector;
float angle_roll_acc, angle_pitch_acc;
float angle_pitch, angle_roll;
int angle_pitch_buffer, angle_roll_buffer;
float angle_pitch_output, angle_roll_output;
long loop_timer;
int temp;
// gyro values are static measurements and pitch, roll and yaw are rate mesurements they come into action when the device drifts
void setup() 
{
  
  Wire.begin();                                                        
  setup_mpu_6050_registers();                                          
  for (int cal_int = 0; cal_int < 1000 ; cal_int ++)//summing the first 1000 values to create a offset
  {                  
    read_mpu_6050_data();                                             
    gyro_x_cal += gyro_x;                                              
    gyro_y_cal += gyro_y;                                              
    gyro_z_cal += gyro_z;                                             
    delay(3);           // 3us delay + 1us for total calculation                                               
  }
pinMode(motorPin1, OUTPUT); 
pinMode(motorPin2, OUTPUT);
pinMode(motorPin3, OUTPUT);
pinMode(motorPin4, OUTPUT);
  // divide by 1000 to get avarage offset
  gyro_x_cal /= 1000;                                                 
  gyro_y_cal /= 1000;                                                 
  gyro_z_cal /= 1000;                                                 
  Serial.begin(9600);
  loop_timer = micros();   //getting the instant when the serial commmunication begins 
//  Serial.println("gyro_x: ");Serial.print(gyro_x);Serial.print(" | ");Serial.print("gyro_y: ");Serial.print(gyro_y);Serial.print(" | ");Serial.print("gyro_z: ");Serial.print(gyro_z);Serial.print(" | ");                                         
}
void loop()
{
 read_mpu_6050_data();   
 //Subtract the offset values from the raw gyro values
  gyro_x -= gyro_x_cal;                                                
  gyro_y -= gyro_y_cal;                                                
  gyro_z -= gyro_z_cal;                                                
//Gyro angle calculations . Note 0.0000611 = 1 / (250Hz x 65.5) // 250 hz is the frequency of the offset calculation for loop 
  angle_pitch += gyro_x * 0.0000611;                                   
  angle_roll += gyro_y * 0.0000611;   
  // 0.0000611 is converted into degrees by multiplying by pi/180                                        
  angle_pitch += angle_roll * sin(gyro_z * 0.000001066);        
  angle_roll -= angle_pitch * sin(gyro_z * 0.000001066);               
  //Accelerometer angle calculations
  acc_total_vector = sqrt((acc_x*acc_x)+(acc_y*acc_y)+(acc_z*acc_z));  
  //57.296 = 1 / (3.142 / 180) The Arduino sin function is in radians
  angle_pitch_acc = asin((float)acc_y/acc_total_vector)* 57.296;      
  angle_roll_acc = asin((float)acc_x/acc_total_vector)* -57.296; 
  Serial.print("Pitch:");Serial.print(angle_pitch_acc);
  Serial.print(" Roll:");Serial.println(angle_roll_acc);
//  delay(1000); 
// get the value of the angle_pitch_acc by keeping the device in the spirit level
  angle_pitch_acc -= 1.50;   //needs to be edited                                            
  angle_roll_acc -= -2.96;   //needs to be edited 
// angle_pitch = yawed drift calculated with gyro values, angle_pitch_acc = yawed drift calculated via accelerometer values                          
if(set_gyro_angles){                                               
    angle_pitch = angle_pitch * 0.9996 + angle_pitch_acc * 0.0004;     // this is when the sensor has already started moving so the gyro pitch angle is corrected using acceleration pitch angle 
    angle_roll = angle_roll * 0.9996 + angle_roll_acc * 0.0004;        
  }
  else{                                                               
    angle_pitch = angle_pitch_acc;                                    // this is done at the first start here the accelerometer pitch angle is copied to gyro pitch angle  
    angle_roll = angle_roll_acc;                                       
    set_gyro_angles = true;                                           
  }
  //To dampen the pitch and roll angles a complementary filter is used
  angle_pitch_output = angle_pitch_output * 0.9 + angle_pitch * 0.1;  //adding raw pitch value to output pitch value
  angle_roll_output = angle_roll_output * 0.9 + angle_roll * 0.1;      
  Serial.print(" | Angle  = "); Serial.println(angle_pitch_output);
 while(micros() - loop_timer < 4000);                            
 loop_timer = micros();//Reset the loop timer
if(angle_pitch_output<=2 && angle_pitch_output>=-2)
  { //Serial.println("hai");
    digitalWrite(motorPin1, LOW);
    digitalWrite(motorPin2, HIGH);//FWD
    digitalWrite(motorPin3, HIGH);
    digitalWrite(motorPin4, LOW);
  }
  else if(angle_pitch_output>2)
  {
    digitalWrite(motorPin1, LOW);
    digitalWrite(motorPin2, HIGH);//LFT
    digitalWrite(motorPin3, LOW);
    digitalWrite(motorPin4, HIGH);
  }
   else if(angle_pitch_output<-2)
  {
   digitalWrite(motorPin1, HIGH);
    digitalWrite(motorPin2, LOW);//RT
    digitalWrite(motorPin3, HIGH);
    digitalWrite(motorPin4, LOW);
  }
  else
  {
    digitalWrite(motorPin1, LOW);
    digitalWrite(motorPin2, LOW);//STOP
    digitalWrite(motorPin3, LOW);
    digitalWrite(motorPin4, LOW);
  }
}
void setup_mpu_6050_registers()
{
  //Activate the MPU-6050
  Wire.beginTransmission(0x68);                                       
  Wire.write(0x6B);                                                   
  Wire.write(0x00);                                                    
  Wire.endTransmission();                                             
  //Configure the accelerometer (+/-8g)
  Wire.beginTransmission(0x68);                                       
  Wire.write(0x1C);                                                    
  Wire.write(0x10);                                                    
  Wire.endTransmission();                                             
  //Configure the gyro (500dps full scale)
  Wire.beginTransmission(0x68);                                        
  Wire.write(0x1B);                                              
  Wire.write(0x08);                                                    
  Wire.endTransmission();                                             
}
void read_mpu_6050_data()
{                                             
  Wire.beginTransmission(0x68);                                        
  Wire.write(0x3B);                                                    
  Wire.endTransmission();                                             
  Wire.requestFrom(0x68,14);                                         
  while(Wire.available() < 14);                                        
  acc_x = Wire.read()<<8|Wire.read();                                  
  acc_y = Wire.read()<<8|Wire.read();                                  
  acc_z = Wire.read()<<8|Wire.read();                                  
  temp = Wire.read()<<8|Wire.read();                                   
  gyro_x = Wire.read()<<8|Wire.read();                                 
  gyro_y = Wire.read()<<8|Wire.read();                                 
  gyro_z = Wire.read()<<8|Wire.read();  
//  Serial.println("gyro_x: ");Serial.print(gyro_x);Serial.print(" | ");Serial.print("gyro_y: ");Serial.print(gyro_y);Serial.print(" | ");Serial.print("gyro_z: ");Serial.print(gyro_z);Serial.print(" | ");                            
}

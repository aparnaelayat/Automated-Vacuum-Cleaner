//Source: https://lastminuteengineers.com/l293d-dc-motor-arduino-tutorial/

// Motor A connections-- Left from back
int enA = 12;
int in1 = 8;
int in2 = 7;

// Motor B connections-- Right from back
int enB = 3;
int in3 = 5;
int in4 = 4;

int relay= 2;

void setup() {
  // Set all the motor control pins to outputs
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(relay, OUTPUT);

  
  // Turn off motors - Initial state
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);

}

void loop() {
  digitalWrite(relay, HIGH); 
  delay(100); // Waits for 1 second
  digitalWrite(relay, LOW); 
  directionControl();
  delay(1000);  
}

// This function lets you control spinning direction of motors
void directionControl() 
{
  // Set motors to maximum speed
  // For PWM maximum possible values are 0 to 255
  analogWrite(enA, 143);
  analogWrite(enB, 133);

  // Turn on motor A & B
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  delay(1000);

}

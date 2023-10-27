/*
   Line follower Arduino
   Sensor - 5 IR sensor
   Black = 1
   White = 0 
   Controller - PID
*/


const int input1A = 1;                             // Input pin 1 for Motor A
const int input2A = 2;                             // Input pin 2 for Motor A
const int enableA = 3;                             //  Enable pin for Motor A

const int input1B = 5;                             // Input pin 1 for Motor B
const int input2B = 6;                             // Input pin 2 for Motor B
const int enableB = 7;                             // Enable pin for Motor B

int const ir1 = A0;                                // IR (can be analog or digitral)
int const ir2 = A1;
int const ir3 = A2;
int const ir4 = A3;
int const ir5 = A4;

const float kp = 0.5;                              // Potential constant
const float ki = 0.005;                            // Integral constant
const float kd = 0.2;                              // Differential constant
float prevError = 0;
float kiError = 0;

int baseSpeed = 100;    

int Error = 0;


void setup() {
  //Motor A
  pinMode(input1A, OUTPUT);
  pinMode(input2A, OUTPUT);
  pinMode(enableA, OUTPUT);
  //Motor B
  pinMode(input1B, OUTPUT);
  pinMode(input2B, OUTPUT);
  pinMode(enableB, OUTPUT);
  // ir sensor
  pinMode(ir1, INPUT);
  pinMode(ir2, INPUT);
  pinMode(ir3, INPUT);
  pinMode(ir4, INPUT);
  pinMode(ir5, INPUT);
}


int PID(int Error){              
  kiError = kiError + Error; 
  float P = kp * Error;                      
  float i = ki * kiError;                   // Integral out    
  float d = kd * (Error*prevError);     // Differential out

  // Final Error
  float Total_out =  P+i+d;// total error calculation

  prevError = Error;
  // Returning Error   

  return int(Total_out); 
}


void cntrlMotor(int pidOutput) {
  int turnVal = baseSpeed + pidOutput;
  if (turnVal > 255) {
    turnVal = 255;
  }
   
  // Stopping motors when error is too large
  if (Error=5){
    analogWrite(enableA,0);
    analogWrite(enableB,0);
  }
   
  // Update from old code. Chagning values here and sending signals at end 
  int leftSpeed = 0;
  int rightSpeed = 0;
  if (pidOutput > 0) {
    leftSpeed = abs(turnVal);   // If robot is turning right
    rightSpeed = baseSpeed;
  } else if (pidOutput < 0) {
    leftSpeed = baseSpeed;
    rightSpeed = abs(turnVal);  // If robot is turning left
  } else {
    leftSpeed = baseSpeed;
    rightSpeed = baseSpeed;  // Go straight
  }

  // Controlling the motors
  analogWrite(enableA, leftSpeed);
  analogWrite(enableB, rightSpeed);
}

int calculateError() {
  // Gathering data from sensors
  int s1 = digitalRead(ir1);  // Left Most Sensor
  int s2 = digitalRead(ir2);  // Left Sensor
  int s3 = digitalRead(ir3);  // Middle Sensor
  int s4 = digitalRead(ir4);  // Right Sensor
  int s5 = digitalRead(ir5);  // Right Most Sensor

  // Variables for the error calculation
  int error = 0;

  //Assuming Black = 1 && White  = 0

  /*
    0 0 0 0 1 ==> Error = 4
    0 0 0 1 1 ==> Error = 3
    0 0 0 1 0 ==> Error = 2
    0 0 1 1 0 ==> Error = 1
    0 0 1 0 0 ==> Error = 0
    0 1 1 0 0 ==> Error = -1
    0 1 0 0 0 ==> Error = -2
    1 1 0 0 0 ==> Error = -3
    1 0 0 0 0 ==> Error = -4 
    1 1 1 1 1 ==> Error = 5
  */

  // Calculate the error based on sensor readings
   
  //if only middle sensor detects black line
  if((s1 == 0) && (s2 == 0) && (s3 == 1) && (s4 == 0) && (s5 == 0))
  {error = 0; }
  //if only left sensor detects black line
  if((s1 == 0) && (s2 == 1) && (s3 == 0) && (s4 == 0) && (s5 == 0))
  {error = -2;}
  //if only left most sensor detects black line
  if((s1 == 1) && (s2 == 0) && (s3 == 0) && (s4 == 0) && (s5 == 0))
  {error = -4;}
  //if only right sensor detects black line
  if((s1 == 0) && (s2 == 0) && (s3 == 0) && (s4 == 1) && (s5 == 0))
  {error = 2;}
  //if only right most sensor detects black line
  if((s1 == 1) && (s2 == 1) && (s3 == 1) && (s4 == 1) && (s5 == 0))
  {error = 4;}
  //if middle and right sensor detects black line
  if((s1 == 0) && (s2 == 0) && (s3 == 1) && (s4 == 1) && (s5 == 0))
  {error = 1;}
  //if middle and left sensor detects black line
  if((s1 == 0) && (s2 == 1) && (s3 == 1) && (s4 == 0) && (s5 == 0))
  {error = -1;}
  //if middle, left and left most sensor detects black line
  if((s1 == 1) && (s2 == 1) && (s3 == 1) && (s4 == 0) && (s5 == 0))
  {error = -2;}
  //if middle, right and right most sensor detects black line
  if((s1 == 0) && (s2 == 0) && (s3 == 1) && (s4 == 1) && (s5 == 1))
  {error = 2;}
  //if all sensors are on a black line
  if((s1 == 1) && (s2 == 1) && (s3 == 1) && (s4 == 1) && (s5 == 1))
  {error = 5;}

  return error;
}

void loop() {
  Error = calculateError();
  // Serial.print("Error: ");
  // Serial.println(Error);

  int p_val = PID(Error);

  // Serial.print("PID Output: ");
  // Serial.println(p_val);
  
  cntrlMotor(p_val);
  delay(10);

}

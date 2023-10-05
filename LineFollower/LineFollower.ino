/*  
Arduino DC Motor Control - PWM | H-Bridge | L298N 
Control Logic - PID
Arduino IR sensor - |unknown model|
by- Shagnik Guha
*/

const int input1A = 7;                             // Input pin 1 for Motor A
const int input2A = 8;                             // Input pin 2 for Motor A
const int enableA = 9;                             //  Enable pin for Motor A

const int input1B = 1;                             // Input pin 1 for Motor B
const int input2B = 2;                             // Input pin 2 for Motor B
const int enableB = 3;                             // Enable pin for Motor B

const float kp = 0.5;                              // Potential constant
const float ki = 0.005;                            // Integral constant
const float kd = 0.2;                              // Differential constant
float prevError = 0;
float kiError = 0;

int baseSpeed = 100;


void setup() {
  //Motor A
  pinMode(input1A, OUTPUT);
  pinMode(input2A, OUTPUT);
  pinMode(enableA, OUTPUT);
  //Motor B
  pinMode(input1B, OUTPUT);
  pinMode(input2B, OUTPUT);
  pinMode(enableB, OUTPUT);
}


int PID(int Error){
  float Proportional = kp * Error;                 // Proportional out 
  kiError = kiError + Error;                       
  float Integral = ki * kiError;                   // Integral out    
  float Differential = kd * (Error*prevError);     // Differential out
  prevError = Error;
  // Final Error
  float Total_out =  Proportional+Integral+Differential;// total error calculation
  // Returning Error      
  return int(Total_out); 
}


void cntrlMotor(int pidOutput){
  int turnVal = baseSpeed + abs(pidOutput);
  if(turnVal > 255 ){
    turnVal = 255;
  }
  if (pidOutput > 0) {
    analogWrite(enableA, abs(turnVal));     // If robot is towards right
    analogWrite(enableB, abs(baseSpeed));
  } else{
    analogWrite(enableA, (baseSpeed));
    analogWrite(enableB, abs(turnVal));     // If robot is towards left
  }
}


void loop() {
  /*
    Error from sensor calculated here
  */
  int error = 0;          // assume we have some error data here
  int cntrl = PID(error);
  cntrlMotor(cntrl);
  
  delay(20);
}

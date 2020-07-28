/* 
 *  CEID Food Assembly Line code
 *  by Wesley Wong
 */

// Servo Setup
#include <Servo.h>
Servo myservo;  // create servo object to control a servo
int servingsFirst = 2;  // number of servings for first dispenser
Servo myservo2; // second servo
int servingsSecond = 2; // number of servings for second dispenser

// conveyor dc motor1 setup
int speedPin1 = 13;
int dir1a = 12;
int dir1b = 11;

// conveyor dc motor2 setup
int speedPin2 = 10;
int dir2a = 9;
int dir2b = 8;

int mSpeed = 255; // max speed
int start = 0; // startup counter variable
int conveyorSpeed = 125; // speed at which the tray will move

void setup() {
  // put your setup code here, to run once:
  pinMode(3,INPUT);   // photosensor module initialization port
  pinMode(4,INPUT);  // second photosensor initialization
  myservo.attach(6);   //  attaches the servo on pin 9 to the servo object
  myservo2.attach(7); // second servo to pin 10
  //pinMode(speedPin1, OUTPUT); // set pin 11 to output to conveyor motor 1 enable
  pinMode(speedPin2, OUTPUT); // set pin 12 to output to conveyor motor 2 enable
  Serial.begin(9600);  // initialization of serial port
  Serial.println("Ready to connect/nDefault password is 1234 or 000");
  delay(1000);
}

void loop() {
  // initialize and reset sensor variables
  int valOne = 0; 
  int valTwo = 0;

  // for loop to create sum variables for the sensor values to smooth out the readings and prevent false triggers
  for (int i = 0; i < 10; i++) {
    if(digitalRead(5) == HIGH)// assign light sensor one to port 52
      valOne++; // increment val one if sensor covered
    if(digitalRead(4) == HIGH)      // light sensor two to port 50
      valTwo++; // increment val two if second sensor covered
    delay(10);
  }
  
  // test code
  Serial.println("valOne=");
  Serial.println(valOne);
  Serial.println("valTwo=");
  Serial.println(valTwo);

  // declare boolean variables to track status of sensors
  bool tempOne = false;
  bool tempTwo = false;

  // update variables if we are sure the sensor is triggered
  if (valOne >= 7) { // this one was 7 due to sensor adjustments
    tempOne = true;
  }
  if (valTwo == 10) {
    tempTwo = true;
  }
  
  Serial.println("Intensity1=");    //print on serial monitor using ""
  Serial.println(tempOne);          //display output on serial monitor
  Serial.println("Intensity2=");    //print on serial monitor using ""
  Serial.println(tempTwo);          //display output on serial monitor
  delay(10);

  /*// Every 100 miliseconds, do a measurement using the sensor and print the distance in centimeters.
  double distance = distanceSensor.measureDistanceCm(); //measures distance in cm
  Serial.println(distance);
  delay(100);*/
  
  if(tempOne == true and tempTwo == true)               //both blocked
  {
    // dispenses first module
    Serial.println("both");
    myservo.write(100); // reverses directon for a bit to unclog
    myservo2.write(100); // reverses directon for a bit to unclog
    delay(20);
    myservo.write(90 - (45 * servingsFirst)); // dispense out
    myservo2.write(90 - (45 * servingsFirst)); // dispense out second servo
    delay(100);
  }
  else if(tempOne == true and tempTwo == false)               //first blocked, second unblocked
  {
    // dispenses first module
    Serial.println("one");
    myservo.write(100); // reverses directon for a bit to unclog
    delay(20);
    myservo.write(90 - (45 * servingsFirst)); // dispense out
    myservo2.write(88.9); // stops second servo
    //myservo.write(100); // reverses directon for a bit to unclog
  }
  else if(tempOne == false and tempTwo == true)               //first unblocked, second blocked
  {
    // dispenses first module
    Serial.println("two");
    myservo2.write(100); // reverses directon for a bit to unclog
    delay(20);
    myservo.write(88.9); // stops first servo
    myservo2.write(90 - (45 * servingsFirst)); // dispense out second
    //myservo.write(100); // reverses directon for a bit to unclog
  }
  else               // for when neither is high
  {
    // dispenses first module
    Serial.println("none");
    myservo.write(88.9); // stops second servo
    myservo2.write(88.9); // stops first servo
    //myservo.write(100); // reverses directon for a bit to unclog
  }
  // direction of motors for conveyor
  digitalWrite(dir1a, HIGH);
  digitalWrite(dir1b, LOW);
  
  digitalWrite(dir2a, HIGH);
  digitalWrite(dir2b, LOW);

  // this function kickstarts the dc motors
  if (start == 0)
  {
    analogWrite(speedPin2, mSpeed); // start motor 2
    analogWrite(speedPin1, mSpeed); // start motor 1
    delay(100);
    start++;
  }

  // conveyor on
  analogWrite(speedPin1, mSpeed);
  analogWrite(speedPin2, mSpeed);
  Serial.println("motor");
  delay(80);
  analogWrite(speedPin1, 0);
  analogWrite(speedPin2, 0);
  delay(80);
  Serial.println("test");
}

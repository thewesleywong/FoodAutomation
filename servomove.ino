/* 
 *  CEID Food Assembly Line code
 *  by Wesley Wong
 */

//HC-05 bluetooth module SoftwareSerial library
#include <SoftwareSerial.h> 
SoftwareSerial MyBlue(2, 3); // Software definition for serial pins; RX2 & TX3
/* Legend (first number is serial entry from bluetooth, second number is number of servings sent by each module)
 * 0-00 
 * 1-01
 * 2-10
 * 3-02
 * 4-20
 */
int val = 0; // declare variable for input from device

//ultrasonic sensor
#include <HCSR04.h>
// Initialize sensor that uses digital pins 12 and 11.
int triggerPin = 12;
int echoPin = 11;
UltraSonicDistanceSensor distanceSensor(triggerPin, echoPin);
//Test distance = (high level time × velocity of sound (340m/s)/2

// Servo Setup
#include <Servo.h>
Servo myservo;  // create servo object to control a servo
const int servingsFirst = 0;  // number of servings for first dispenser
Servo myservo2; // second servo
const int servingsSecond = 0; // number of servings for second dispenser



void setup() {
  // put your setup code here, to run once:
  pinMode(52,INPUT);   // photosensor module initialization port
  pinMode(50,INPUT);  // second photosensor initialization
  myservo.attach(9);   //  attaches the servo on pin 9 to the servo object
  myservo2.attach(10); // second servo to pin 10
  Serial.begin(9600);  // initialization of serial port
  MyBlue.begin(9600);  // Configuring software serial baud rate at 9600
  Serial.println("Ready to connect/nDefault password is 1234 or 000");
  delay(1000);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (MyBlue.available()) //checks if connected and assigns read value to val
   val = MyBlue.read(); 
  int tempOne=digitalRead(52);      // assign light sensor one to port 52
  int tempTwo=digitalRead(50);      // light sensor two to port 50
  Serial.println("Intensity1=");    //print on serial monitor using ""
  Serial.println(tempOne);          //display output on serial monitor
  Serial.println("Intensity2=");    //print on serial monitor using ""
  Serial.println(tempTwo);          //display output on serial monitor

  // Every 100 miliseconds, do a measurement using the sensor and print the distance in centimeters.
  double distance = distanceSensor.measureDistanceCm(); //measures distance in cm
  Serial.println(distance);
  delay(100);
  
  if(tempOne == HIGH)               //HIGH means,light got blocked
  {
    // dispenses first module
    Serial.println("dispense");
    myservo.write(90 - (45 * servingsFirst)); // dispense out
    delay(100);
    // myservo.write(100); // reverses directon for a bit to unclog
  }
  else if(tempTwo == HIGH)
  {
    // dispenses second module
    Serial.println("second dispense");
    myservo2.write(90 - (45 * servingsSecond)); // dispense out
    delay(100);
    // myservo2.write(100); // reverses direction for a bit
  }
  else {
    myservo.write(89); // stops servos
    myservo2.write(0); // stops servos
  }
}

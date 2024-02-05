#include <Servo.h>

const int POT_PIN = A0 ; // Potentiometer output pin
const int SRV_PIN = 9 ; // Servo input pin
const int INP_DEL = 15 ; // wait time between updating servo postion

Servo myServo ;

int servoAngle ;
int potenValue ;

void log(int pVal, int aVal) {
  Serial.print("pVal: ");
  Serial.print(pVal);
  Serial.print(", aVal: ");
  Serial.println(aVal);
}

void setup() {

  /*
    Sets digital pin as INPUT for Servo
  */
  myServo.attach(SRV_PIN) ;
  
  /*
    Turns on serial port for reading Potentiometer data
  */
  Serial.begin(9600);

}

void loop() {

  /*
    Read in the potentiometers position (0-1023)
  */
  potenValue = analogRead(POT_PIN);

  /*
    Convert potentiometer value to an angle (0-179)
  */
  servoAngle = map(potenValue, 0, 1023, 0, 179);

  /*
    Set servo angle
  */
  myServo.write(servoAngle);

  log(potenValue, servoAngle);

  delay(INP_DEL);

}

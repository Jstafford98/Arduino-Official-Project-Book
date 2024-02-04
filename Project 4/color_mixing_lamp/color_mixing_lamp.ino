/*
  Brief introduction to reading RGB values and writing them to RGB LEDS. Not superrrr interesting but still useful.
*/


/*
  Define Analog inputs for measured RGB colors
  from Photoreistors (PHR):
    A0 = Red
    A1 = Green
    A2 = Blue
*/
const int PHR_RED = A0 ;
const int PHR_GRE = A1 ;
const int PHR_BLU = A2 ;

/*
  Define output pins to the RGB LED
  Pin 9  = Green
  Pin 10 = Blue
  Pin 11 = Red
*/
const int LED_PIN_G = 9 ;
const int LED_PIN_B = 10 ;
const int LED_PIN_R = 11 ;

const int READ_DELAY = 10 ;

// Values to write to RGB LED
int redValue ;
int blueValue ;
int greenValue ;

// Measured color channel values from Photoresistors
int redSensorValue ;
int blueSensorValue ;
int greenSensorValue ;

void setup() {
  
  Serial.begin(9600);

  pinMode(LED_PIN_G, OUTPUT);
  pinMode(LED_PIN_B, OUTPUT);
  pinMode(LED_PIN_R, OUTPUT);

}

void loop() {

  redSensorValue = analogRead(PHR_RED);
  delay(READ_DELAY); // We need a delay to allow analogRead to work

  greenSensorValue = analogRead(PHR_GRE);
  delay(READ_DELAY);

  blueSensorValue = analogRead(PHR_BLU);
  delay(READ_DELAY);

  redValue = redSensorValue / 4 ;
  blueValue = blueSensorValue / 4 ;
  greenValue = greenSensorValue / 4 ;

  analogWrite(LED_PIN_R, redValue);
  analogWrite(LED_PIN_G, greenValue);
  analogWrite(LED_PIN_B, blueValue);

  Serial.print("Raw Sensor Values\t(Red: ");
  Serial.print(redSensorValue);

  Serial.print(", Green: ");
  Serial.print(greenSensorValue);

  Serial.print(", Blue: ");
  Serial.println(blueSensorValue);

  Serial.print("Mapped Sensor Values\t(Red: ");
  Serial.print(redValue);

  Serial.print(", Green: ");
  Serial.print(greenValue);

  Serial.print(", Blue: ");
  Serial.println(blueValue);

}

/*
  Project 6 - Light Theremin

  This code enables our Light Theremin Arduino project, which changes the pitch played by a 
  Piezo based on the ambient light readings from a Photoresistor.
  
*/

const int LED_PIN = 13 ; // built in Arduino LED to signal calibration
const int PHR_PIN = A0 ; // Analog pin to read Photoresistor values from
const int PIEZO_PIN = 8 ; // Digital pin to write our Piezo tone to

/*
  Our pitch is in Hz. These values will be used when mapping our read Photoresistor
  values to a tone on the Piezo. Our PITCH_MIN will be the Hertz value to map a 
  Photoresistor value of 0 to, and PITCH_MAX to a value of 1023. These are effectively
  the lowest and highest pitches the Piezo will play.
*/
const int PITCH_MIN = 50 ;
const int PITCH_MAX = 4000 ;

// Time the Piezo will play the tone
const int TONE_DURATION = 20 ;

// Time between updating the tone on our Piezo
const int UPDATE_DELAY = 10 ;

// Time to take ambient light samples from our Photoresistor to calibrate it
const int CALIBRATION_TIME = 5000 ;

int pitch ;
int sensorValue ;

// these values will be calibrated to control our Piezo's tone based on ambient light
int sensorLow ;
int sensorHigh ;

void calibratePiezo(){
  /*
    This function calibrates our mapping range for converting ambient light readings from
    a photoresistor to a tone on the piezo. We take samples for CALIBRATION_TIME milliseconds
    and record the highest and lowest value recieved from the photoresistor.
  */

  sensorHigh = 0 ;
  sensorLow = 1023 ;

  digitalWrite(LED_PIN, HIGH); //Turn on the Arduino board LED while calibration is going

  while (millis() < CALIBRATION_TIME) {

    // Sample ambient light and record it if it's a record low or high
    sensorValue = analogRead(PHR_PIN);

    if (sensorValue > sensorHigh)
      sensorHigh = sensorValue;

    if (sensorValue < sensorLow)
      sensorLow = sensorValue;

  }

  digitalWrite(LED_PIN, LOW); // Turn off board LED

}

void setup() {

  pinMode(LED_PIN, OUTPUT);
  calibratePiezo();

}

void loop() {

  // Read value from Photoresistor (0-1023)
  sensorValue = analogRead(PHR_PIN);

  /*
    Convert the value to a "pitch" in hertz (hz), where 
    a sensorValue of 0 == PITCH_MIN hz and 1023 == PITCH_MAX hz
  */
  pitch = map(sensorValue, sensorLow, sensorHigh, PITCH_MIN, PITCH_MAX);

  // Write our pitch to the Pizeo
  tone(PIEZO_PIN, pitch, TONE_DURATION);

  delay(UPDATE_DELAY);

}

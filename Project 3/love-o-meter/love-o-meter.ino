/*
  The Love-O-Meter :)

  This is the driver program for the Love-O-Meter, an Arduino project which measures a users body temperature
  and displays an LED sequence that correlates to how much hotter the user is than the calibrated ambient room 
  temperature. 
  
  The user's body temperature is read using a TMP36 Temperature sensor, which returns a value of
  0-1023, which correlates to the actual voltage of the sensor. We can convert this returned value to it's
  actual voltage, and then to a temperature in degrees Celcius.

  Baesd on the calculated temperature, 1 to 3 LEDs are turned on show the user as a low, medium, or high 
  "hotness" level. 

*/
const int LED_PIN_2 = 2;
const int LED_PIN_3 = 3;
const int LED_PIN_4 = 4;
const int SWITCH_PIN = 5 ;
const int TMP_PIN_A0 = A0;

// Deltas above baselineTemp used for triggering LEDs
const int LOW_TEMP_DELTA = 2;
const int MEDI_TEMP_DELTA = 4;
const int HIGH_TEMP_DELTA = 6;

/*
  This smoothing value is a time delay, in milliseconds, we should wait before
  recalculating body temperature and updating our LED display. When we read the 
  temperature without a delay, we may find that the most recently lit LED tends
  to flash off and on quickly as the temperature reading from our sensor can fluctuate.
  The simplest solution is to slow down how quickly we read temperature to allow
  it to stabilize a bit.
*/
const int SMOOTHING = 500;

/*
  Configurable params for calibrating the TMP36 Sensor.
  nCalibrationSamples is the total temperature samples to take
  calibrationSampleRate is the time, in milliseconds, to wait between samples
*/
const int nCalibrationSamples = 10;
const int calibrationSampleRate = 100;

int switchState;
int sensorValue;
float sensorVoltage;
float temperatureCelcius;

/* Our calibrated sensor temperature will be stored here */
float baselineTemp;

float calculateVoltage(int sensorValue) {
  /*
    Our TMP36 Sensor returns a value between 0 and 1023, which represents a Voltage from
    0 to 5 Volts.
  */
  return (sensorValue / 1024.0) * 5.0;
}

float calculateTemperatureCelcius(float sensorVoltage) {
  /*
    We can convert the output voltage from our TMP36 Sensor into degrees celcius like so
  */
  return (sensorVoltage - .5) * 100;
}

void displayToSerial(int sensorValue, float sensorVoltage, float temperatureCelcius) {
  /*
    Some basic logging to the Serial monitor 
  */

  Serial.print("Sensor Value: ");
  Serial.print(sensorValue);
  Serial.print(", Volts: ");
  Serial.print(sensorVoltage);
  Serial.print(", degrees C: ");
  Serial.print(temperatureCelcius);
  Serial.print("\n");
}

void setLedSequence(int pin2, int pin3, int pin4) {
  /*
    Code used to set our 3-LED Sequence. Each value should either be 
    LOW or HIGH.
  */
  digitalWrite(LED_PIN_2, pin2);
  digitalWrite(LED_PIN_3, pin3);
  digitalWrite(LED_PIN_4, pin4);
}

void calibrateSensor(int nSamples, int sampleDelay) {
  /*
    Calibrate our TMP36 Sensor to the ambient room temperature by taking the mean of
    our samples and setting that as our baseline. 
  */

  setLedSequence(HIGH, HIGH, HIGH);  // Turn on all LEDs to show calibration in process

  int sampleMean;
  float sampleTemp;
  float sampleVoltage;
  int samples[nSamples];

  Serial.print("\n-------------------- Calibration Values --------------------\n");
  Serial.print("Calibration Samples: [");

  for (int i = 0; i < nSamples; i++) {
    // Get nSamples of ambient temperature
    samples[i] = analogRead(TMP_PIN_A0);

    Serial.print(samples[i]);

    if (i != (nSamples - 1))
      Serial.print(", ");

    delay(sampleDelay);
  }
  Serial.print("]\n");

  // Calculate the mean ambient temperature (sensor value)
  sampleMean = 0;
  for (int i = 0; i < nSamples; i++)
    sampleMean += samples[i];
  sampleMean = sampleMean / nSamples;

  // Convert our mean sensor value to an actual temperature
  sampleVoltage = calculateVoltage(sampleMean);
  sampleTemp = calculateTemperatureCelcius(sampleVoltage);
  baselineTemp = sampleTemp;

  // Log the calibration data
  Serial.print("Mean Sensor Value: ");
  Serial.print(sampleMean);
  Serial.print(", Volts: ");
  Serial.print(sampleVoltage);
  Serial.print("\n");
  Serial.print("Calibrated Baseline Temperature in Degrees C: ");
  Serial.print(sampleTemp);
  Serial.print("\n");
  Serial.print("-------------------- End of Calibration --------------------\n");

  setLedSequence(LOW, LOW, LOW);  // turn off LEDs once calibration is complete
}

void setup() {

  Serial.begin(9600);  // Opens our serial port

  // Configure our output LEDs
  pinMode(LED_PIN_2, OUTPUT);
  pinMode(LED_PIN_3, OUTPUT);
  pinMode(LED_PIN_4, OUTPUT);

  /* 
    Configure a switch as an input, this switch is used to 
    recalibrate a baseline temperature on command.
  */
  pinMode(SWITCH_PIN, INPUT);

  //Calibrate sensor to get a baseline
  calibrateSensor(nCalibrationSamples, calibrationSampleRate);
}

void loop() {

  delay(SMOOTHING);  // Delay helps us ignore some ugly fluctuation in LEDs

  /* Recalibrate our temperature if the user presses the calibration button */
  switchState = digitalRead(SWITCH_PIN);
  if (switchState == HIGH){
    setLedSequence(LOW, LOW, LOW);
    calibrateSensor(nCalibrationSamples, calibrationSampleRate);
  }

  sensorValue = analogRead(TMP_PIN_A0);  // Gets a value of 0-1023 from our TMP36 Sensor

  sensorVoltage = calculateVoltage(sensorValue);
  temperatureCelcius = calculateTemperatureCelcius(sensorVoltage);

  displayToSerial(sensorValue, sensorVoltage, temperatureCelcius);

  /* 
    Here we need to determine what LEDs should be on in our display. 
    If the read temp is >= baselineTemp + HIGH_TEMP_DELTA we should
    light all three, if MEDI_TEMP_DELTA, first two, if LOW_TEMP_DELTA,
    only one. Otherwise, turn them all off.
  */
  if (temperatureCelcius >= baselineTemp + HIGH_TEMP_DELTA)
    return setLedSequence(HIGH, HIGH, HIGH);

  if (temperatureCelcius >= baselineTemp + MEDI_TEMP_DELTA)
    return setLedSequence(HIGH, HIGH, LOW);

  if (temperatureCelcius >= baselineTemp + LOW_TEMP_DELTA)
    return setLedSequence(HIGH, LOW, LOW);

  return setLedSequence(LOW, LOW, LOW);
}

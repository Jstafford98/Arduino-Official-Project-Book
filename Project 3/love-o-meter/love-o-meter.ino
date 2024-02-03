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
const int TMP_PIN_A0 = A0;

const float BASELINE_TEMP = 20.0;  // Ambient room temperature

// Deltas above BASELINE_TEMP used for triggering LEDs
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

int sensorValue;
float sensorVoltage;
float temperatureCelcius;

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

void setup() {

  Serial.begin(9600);  // Opens our serial port

  // Configure our output LEDs
  pinMode(LED_PIN_2, OUTPUT);
  pinMode(LED_PIN_3, OUTPUT);
  pinMode(LED_PIN_4, OUTPUT);
}

void loop() {

  delay(SMOOTHING);  // Delay helps us ignore some ugly fluctuation in LEDs

  sensorValue = analogRead(TMP_PIN_A0);  // Gets a value of 0-1023 from our TMP36 Sensor

  sensorVoltage = calculateVoltage(sensorValue);
  temperatureCelcius = calculateTemperatureCelcius(sensorVoltage);

  displayToSerial(sensorValue, sensorVoltage, temperatureCelcius);

  /* 
    Here we need to determine what LEDs should be on in our display. 
    If the read temp is >= BASELINE_TEMP + HIGH_TEMP_DELTA we should
    light all three, if MEDI_TEMP_DELTA, first two, if LOW_TEMP_DELTA,
    only one. Otherwise, turn them all off.
  */
  if (temperatureCelcius >= BASELINE_TEMP + HIGH_TEMP_DELTA)
    return setLedSequence(HIGH, HIGH, HIGH);

  if (temperatureCelcius >= BASELINE_TEMP + MEDI_TEMP_DELTA)
    return setLedSequence(HIGH, HIGH, LOW);

  if (temperatureCelcius >= BASELINE_TEMP + LOW_TEMP_DELTA)
    return setLedSequence(HIGH, LOW, LOW);

  return setLedSequence(LOW, LOW, LOW);
}

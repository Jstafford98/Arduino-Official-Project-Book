/*
  Spaceship Interface Code (Arduino Project 2)

  This code configures a 3-LED Arduino spaceship interface, which consists of 1 green LED and 2 red LEDs that
  are controlled by a Push-Button switch. 

  When the state of the switch, represented by switchState, is LOW (0 or False), then only the green LED is 
  highlighted. If the state becomes HIGH, or if the state remains HIGH after a previous sequence, then an 
  LED display sequence is displayed, defined in displayInitSequence. After the switch button is unpressed,
  then the LOW switch state resumes and is controlled by resetShipDisplay.
  
  Sequence Diagram:

  - Default State
    [ ] - Red   LED (LOW)
    [ ] - Red   LED (LOW)
    [*] - Green LED (HIGH)

  - When the switch is pressed, our initialization sequence has two stages
    - Stage One
        [*] - Red   LED (HIGH)
        [ ] - Red   LED (LOW)
        [ ] - Green LED (LOW)

    - There is now a delay of n milliseconds , where n == DELAY_SEQUENCE, before stage two.

    - Stage Two
        [ ] - Red   LED (LOW)
        [*] - Red   LED (HIGH)
        [ ] - Green LED (LOW)

    - There is now a delay of n milliseconds , where n == DELAY_SEQUENCE, before the next step.

    Stages one and two will repeat n times, where n == REPEAT_SEQUENCE.

    As long as the switch is now released (LOW state), then we return to the default state. If the switch is still
    HIGH, then repeat.

  Arduino Pin Plan:
    Pin 5  : Output, controls top LED (Red)
    Pin 4  : Output, controls middle LED (Red)
    Pin 3  : Output, controls bottom LED (Green)
    Pin 2  : Input, tracks the state of the Push-Button Switch
    Power  : 5V Arduino Connector
    Ground : Arduino GRND Connector
*/

const int DELAY_SEQUENCE = 250;  // Delay between LED changes in displayInitSequence
const int REPEAT_SEQUENCE = 1;   // Total times to repeat the displayInitSequence code for each button press.

const int SWITCH_PIN = 2; // Our input from the switch will come to pin 2
const int TOP_RED_LED = 5;
const int MIDDLE_RED_LED = 4;
const int BOTTOM_GREEN_LED = 3;

int switchState;

void displayInitSequence(int delaySequence, int repeatSequence) {
  /*
    This function controls our initialization sequence of the spaceship delay when
    the switch is pressed. 

    @param delaySequence  : total delay between stages (in milliseconds)
    @param repeatSequence : total times to repeat our initialization visual
  */
  for (int i = 0; i < repeatSequence; i++) {

    // Stage One (Turn on top led, turn off bottom)
    digitalWrite(BOTTOM_GREEN_LED, LOW);
    digitalWrite(MIDDLE_RED_LED, LOW);
    digitalWrite(TOP_RED_LED, HIGH);
    delay(delaySequence); // delay so you can visually see it happen

    // Stage Two (Turn on middle led, turn off top)
    digitalWrite(MIDDLE_RED_LED, HIGH);
    digitalWrite(TOP_RED_LED, LOW);
    delay(delaySequence);

  };
};

void resetShipDisplay() {
  /*
    Resets our ship display to it's default following an initialiation sequence
    OR maintains the default display while idle
  */
  digitalWrite(BOTTOM_GREEN_LED, HIGH);
  digitalWrite(MIDDLE_RED_LED, LOW);
  digitalWrite(TOP_RED_LED, LOW);
};

void setup() {
  // Define pins 3-5 as output pins and pin 2 as the input pin
  pinMode(SWITCH_PIN, INPUT);
  pinMode(BOTTOM_GREEN_LED, OUTPUT);
  pinMode(MIDDLE_RED_LED, OUTPUT);
  pinMode(TOP_RED_LED, OUTPUT);
};

void loop() {
  /*
    Main program loop, runs indefinitely and monitors the state of our switch. 
    If the switch state is set to HIGH, we run the initialization visual sequence
    and then return to default
  */

  // Get switch state from pin 2
  switchState = digitalRead(2);

  // If pin 2 state is LOW, ensure display is set to default
  if (switchState == LOW)
    return resetShipDisplay();

  // If our switch was pressed, run the initialization sequence display
  // our display will be reset on the next cycle
  displayInitSequence(DELAY_SEQUENCE, REPEAT_SEQUENCE);
};

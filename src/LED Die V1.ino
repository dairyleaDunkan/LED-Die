/*
This is the code for a simple (child-solderable) LED die

Below is the pysical layout of the LEDs for reference:

LED1  LED2  LED3


      LED4


LED5  LED6  LED7

For ease, the 7 LEDs are connected to the first 7 pins in PORTA - in order
This way you can bit-set all the outputs in one line

*/

// The pysical port that the LEDs are connected to and the switch pin
#DEFINE LED_PORT PORTA
#DEFINE SW1 8

// The output patterns that represent dice rolls 1 to 6 (or all off) and a flashing animation
byte patterns[7] = { b00000000, b00010000, b00101000, b10010010, b10101010, b10111010, b11101110 };
byte animation[2] = { b10101010, b00010000 };

// The status of the switch input and the randomly chosen dice roll
boolean sw_status;
int randNumber;

// Placeholders for the captured program time (milliseconds)
unsigned long elapsed_time;
unsigned long now_time;

// setup stuff here
void setup() {
  // Seed the random value from the unconnected analog input (noisy)
  randomSeed(analogRead(7));
  // Iterate 6 times
  for (int i = 0; i < 7; i++) {
    // Set pin 'i' as an output
    pinMode(i, OUTPUT);
  }
  // Set the switch pin to an input
  pinMode(SW1, INPUT);
  // Set LEDs to all OFF
  setLeds(0);
}

void loop() {
  // Read the switch (ON or OFF)
  sw_status = digitalRead(SW1);
  // If the switch is pressed (ON)
  if (sw_status) {
    // Choose a random number from 1 to 6
    randNumber = random(1, 7);
    // Show the flashing animation
    animation();
    // Show the selected rolled number on the LEDs
    setLeds(randNumber);
    // Save the time that we started displaying
    now_time = millis();
  }
  // Check the elapsed time (since we last updated the LEDs)
  elapsed_time = millis() - now_time;
  // If the elapsed time is 10 seconds or over
  if(elapsed_time >= 10000){
    // Reset the LEDs to OFF
    setNumber(0);
  }
}

void setNumber(byte number) {
  // As long as the input number is from 0 to 6
  if (0 <= number <= 7) {
    // Set bits 0 to 6 of LED port to the required pattern
    LED_PORT = LED_PORT | patterns[number];
  }
}

void animation() {
  // Starting delay time
  int init_delay = 20;
  // Iterate, increasing i each time to slow down the animation
  for (int i = init_delay; i >= 1000; i = i + 50) {
    // Set bits 0 to 6 of LED port to the required pattern
    LED_PORT = LED_PORT | animations[0];
    // Wait ...
    delay(i);
    // Set bits 0 to 6 of LED port to the required pattern
    LED_PORT = LED_PORT & animations[1];
    // Wait ...
    delay(i);
  }
}
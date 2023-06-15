#include "FastLED.h"
#define NUM_LEDS 60
CRGB leds[NUM_LEDS];
#define PIN 8

unsigned long startTime = 0;
//unsigned long currentMillis;

// Set timer for LED lights
const unsigned long LED_Lights_Max_Time_On = 125000;

const int trigPinUpstairs = 4; // Trigger pin of the upstairs ultrasonic sensor
const int echoPinUpstairs = 5; // Echo pin of the upstairs ultrasonic sensor
const int trigPinDownstairs = 6; // Trigger pin of the downstairs ultrasonic sensor
const int echoPinDownstairs = 7; // Echo pin of the downstairs ultrasonic sensor
//long durationUpstairs, distanceUpstairs, durationDownstairs, distanceDownstairs;

// LED status
bool led_on = false;

void upStairs(){

  long duration1, distance1;
  digitalWrite(trigPinUpstairs, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPinUpstairs, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinUpstairs, LOW);
  duration1 = pulseIn(echoPinUpstairs, HIGH);
  distance1 = duration1 * 0.034 / 2;

  // Read distance from downstairs sensor
  long duration2, distance2;
  digitalWrite(trigPinDownstairs, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPinDownstairs, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinDownstairs, LOW);
  duration2 = pulseIn(echoPinDownstairs, HIGH);
  distance2 = duration2 * 0.034 / 2;

  // Check if either sensor is triggered
  if (distance1 <= 50 && led_on == false) {
    startTime = millis();  //initial start time
    colorWipe(0xff,0x00,0x00, 50);
    colorWipe(0x00,0x00,0xff, 50);
    led_on = true;
  } else {
    if(led_on == true && distance1 <= 50){
        led_on = false;
        colorWipe(0x00,0x00,0x00, 50);
    }
  }

  if(led_on == true && distance2 <= 50){
    colorWipe(0x00,0x00,0x00, 50);
     led_on = false;
  }
}

void downstairs(){
  /*********** THIS IS FOR THE DOWN STAIRS SENSOR **********/

  
  long duration1, distance1;
  digitalWrite(trigPinUpstairs, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPinUpstairs, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinUpstairs, LOW);
  duration1 = pulseIn(echoPinUpstairs, HIGH);
  distance1 = duration1 * 0.034 / 2;

  // Read distance from downstairs sensor
  long duration2, distance2;
  digitalWrite(trigPinDownstairs, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPinDownstairs, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinDownstairs, LOW);
  duration2 = pulseIn(echoPinDownstairs, HIGH);
  distance2 = duration2 * 0.034 / 2;

  if(distance1 <= 50 && led_on == true){
      led_on = false;
      colorWipe(0x00,0x00,0x00, 50);
  }

  if(distance2 <= 50 && led_on == false){
      startTime = millis();  //initial start time
      led_on = true;
      reverseColorWipe(0xff,0x00,0x00, 50);
      reverseColorWipe(0x00,0x00,0xff, 50);
  } else {
    if(led_on == true && distance2 <= 50){
        led_on = false;
        reverseColorWipe(0x00,0x00,0x00, 50);
    }
  }
}

void setup() {
  // Initialize serial communication
  Serial.begin(9600);
  
  // Initialize FastLED library
  FastLED.addLeds<WS2811, PIN, GRB>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );

  // Initialize pins
  pinMode(trigPinUpstairs, OUTPUT);
  pinMode(echoPinUpstairs, INPUT);
  pinMode(trigPinDownstairs, OUTPUT);
  pinMode(echoPinDownstairs, INPUT);
}

void loop() {
  
  upStairs();
  delay(100);
  downstairs();
  delay(100);
  checkLEDLightStatus();
  delay(100);

}

void colorWipe(byte red, byte green, byte blue, int SpeedDelay) {
  for(uint16_t i=0; i<NUM_LEDS; i++) {
      leds[i] = CRGB(red, green, blue);
      FastLED.show();
      delay(SpeedDelay);
  }
}

void reverseColorWipe(byte red, byte green, byte blue, int SpeedDelay)
{
  for (int i = NUM_LEDS - 1; i >= 0; i--){
      leds[i] = CRGB(red, green, blue);
      FastLED.show();
      delay(SpeedDelay);
  }
}

void checkLEDLightStatus()
{
  // Check to see if the LED lights are on for more than 2 minutes
  if(led_on == true && millis() - startTime > LED_Lights_Max_Time_On){
    reverseColorWipe(0x00,0x00,0x00, 50);
    led_on = false;
  }  
}

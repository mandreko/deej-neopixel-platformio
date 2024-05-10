#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>  // Required for 16 MHz Adafruit Trinket
#endif

const int NUM_SLIDERS = 5;
const int analogInputs[NUM_SLIDERS] = {A6, A0, A1, A2, A3}; 
const int BRIGHTNESS = 25; // out of 100%

int analogSliderValues[NUM_SLIDERS];
Adafruit_NeoPixel NeoPixel(NUM_SLIDERS, A7, NEO_GRB + NEO_KHZ800);

void updateSliderValues() {
  for (int i = 0; i < NUM_SLIDERS; i++) {
     analogSliderValues[i] = analogRead(analogInputs[i]); 
  }
}

void sendSliderValues() {
  String builtString = String("");

  for (int i = 0; i < NUM_SLIDERS; i++) {
    builtString += String((int)analogSliderValues[i]);

    if (i < NUM_SLIDERS - 1) {
      builtString += String("|");
    }
  }
  
  Serial.println(builtString);
}

void updateMuteLEDColors() {
  NeoPixel.clear();
  for (int i = 0; i < NUM_SLIDERS; i++) {
    int sliderValue = analogSliderValues[i];
    int sliderNumber = 4-i; // inverse since LEDs are "backwards"
    if (sliderValue <= 10) { 
      // Change LED to red
      NeoPixel.setPixelColor(sliderNumber, NeoPixel.Color(255, 0, 0));
    }
    else{
      NeoPixel.setPixelColor(sliderNumber, NeoPixel.Color((1023-sliderValue)/4, sliderValue/4, 0));
    }
    
    
    //Serial.println("changing pixel#" + String(i) + " to " + String(sliderValue));
    NeoPixel.show();
  }
}

void setup() { 
  for (int i = 0; i < NUM_SLIDERS; i++) {
    pinMode(analogInputs[i], INPUT);
  }
  NeoPixel.begin();
  NeoPixel.setBrightness(BRIGHTNESS);

  Serial.begin(9600);
}

void loop() {
  updateSliderValues();
  sendSliderValues(); // Actually send data (all the time)
  updateMuteLEDColors();
  // printSliderValues(); // For debug
  delay(10);
}
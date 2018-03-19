#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN 12

const int ledPin =  13;                                    // the number of the LED pin
const int strandLength = 76;                               // the length of the LED strand
const int numPanels = 8;                                   // number of panels per LED strand
const int pixelOffset = 2;                                 // number of LEDs at the beginning of the strand to skip
const int pixelsPerPanel = floor(strandLength/numPanels);  // length of a panel in LEDs
const int buttonPins[numPanels] = {1,2,3,5,7,9,10,11};     // the number of the pushbutton pin
const int maxBrightness = 128;

// variables will change:
int buttonStates[numPanels];                               // variable for reading the pushbutton status
byte colorCounters[numPanels];                              // variable for incrementing the LED display type when button is pressed.
uint32_t colors[numPanels];                                // variable for per-panel color 
int anyButtonsPushed;


// Initialize the NeoPixel strip
// =========================================================================
// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(90, PIN, NEO_GRB + NEO_KHZ800);
// ===========================================================================

void setup() {
  pinMode(ledPin, OUTPUT);
  // initialize the pushbutton pin as an input:
  for(int i=0; i< numPanels; i++){
    pinMode(buttonPins[i], INPUT);
  }
  strip.begin();
  strip.setBrightness(maxBrightness);
  strip.show();
}

void loop() {
//  strip.setPixelColor(10, 255, 0, 0);
//  colorWipe((255,255,0), 100);
  anyButtonsPushed = 0;
//  a = !a;
//  
  for (uint16_t i=0; i<numPanels; i++){
    // read the state of the pushbutton value:
    buttonStates[i] = digitalRead(buttonPins[i]);

    if (buttonStates[i] == 1) {
      colorCounters[i] += 1;
    }

    colors[i] = Wheel(colorCounters[i]);
  }


  for (uint16_t i=0; i<numPanels; i++){
    for (uint16_t j=0; j<pixelsPerPanel; j++){
      strip.setPixelColor(pixelsPerPanel*i + j + pixelOffset, colors[i]);
    }
  }

  strip.show();

  for (uint16_t i=0; i<numPanels; i++){
    anyButtonsPushed |= buttonStates[i];
  }
 
  digitalWrite(ledPin, anyButtonsPushed);

  delay(10);
}


// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define LED_PIN 12
#define LETTER_I 0
#define LETTER_N1 7
#define LETTER_D 16
#define LETTER_N2 38
#define LETTER_E1 50
#define LETTER_G 61
#define LETTER_E2 77
#define LETTER_V 87

Adafruit_NeoPixel strip = Adafruit_NeoPixel(100, LED_PIN, NEO_GRB + NEO_KHZ400);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

void setup() {
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
  #if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif
  // End of trinket special code


  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
  whiteAll();
  rainbowWipe();
  rainbowAll();
  lettersBlink();
  showAll();
  theaterChase(strip.Color(127, 127, 127), 100);
  theaterChase(strip.Color(127, 127, 127), 100);
  theaterChase(strip.Color(127, 127, 127), 100);
  theaterChase(strip.Color(127, 127, 127), 100);

}

// all sign rainbow wipe
void rainbowWipe() {
  uint32_t i, c;
  c = randomColor();
  for(i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(70);
  }
}

void letter(uint8_t from, uint8_t to, uint32_t c) {
  uint8_t a;
  for(a=from; a<to; a++)  {
      strip.setPixelColor(a, c);    
  }
  strip.show();
}

void lettersBlink() {
uint16_t t, d;
d=1000;
  for(t=0; t<4; t++) {
    whiteAll();
    letter(LETTER_I,LETTER_N1, randomColor());
    delay(d);
    whiteAll();
    letter(LETTER_N1,LETTER_D, randomColor());
    delay(d);
    whiteAll();
    letter(LETTER_D,LETTER_N2, randomColor());
    delay(d);
    whiteAll();
    letter(LETTER_N2,LETTER_E1, randomColor());
    delay(d);
    whiteAll();
    letter(LETTER_E1,LETTER_G, randomColor());
    delay(d);
    whiteAll();
    letter(LETTER_G,LETTER_E2, randomColor());
    delay(d);
    whiteAll();
    letter(LETTER_E2,LETTER_V, randomColor());    
    delay(d);
    whiteAll();
    letter(LETTER_V,101, randomColor());
    delay(d);
  }
}

void rainbowAll() {
  uint16_t i, j, t;  
  for(t=0; t<4; t++) {
    for(j=0; j<256; j++) {
      for(i=0; i<strip.numPixels(); i++) {
        strip.setPixelColor(i, Wheel((i+j) & 255));
      }
      strip.show();
      delay(30);
    }
  }
}

void showAll() {
  uint32_t i, color;
  color = randomColor();
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, color);
    }
    strip.show();
    delay(5000);
}

uint32_t randomColor() {
  uint32_t i, color;
  i = random(11);
  if(i==0){
    color = strip.Color(255, 0, 0);
   }
  else if (i==1){
    color = strip.Color(0, 255, 0);
   }
  else if (i==2){
    color = strip.Color(0, 0, 255);
   }
  else if (i==3){
    color = strip.Color(150, 255, 0);
   }
  else if (i==4){
    color = strip.Color(230, 20, 150);
   }
  else if (i==5){
    color = strip.Color(230, 100, 20);
   }
  else if (i==6){
    color = strip.Color(200, 7, 170);
   }
  else if (i==7){
    color = strip.Color(255, 255, 0);
   }
  else if (i==8){
    color = strip.Color(0, 255, 255);
   }
  else if (i==9){
    color = strip.Color(255, 0, 255);
   }
  else if (i==10){
    color = strip.Color(120, 10, 140);
   }      
  return color;
}

void whiteAll() {
  uint16_t i;
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, strip.Color(255, 255, 255));
    }
    strip.show();
}

void offAll() {
  uint16_t i;
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, strip.Color(0, 0, 0));
    }
    strip.show();
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
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

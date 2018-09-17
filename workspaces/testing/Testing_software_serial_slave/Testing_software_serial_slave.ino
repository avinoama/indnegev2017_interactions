#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif
#include <SoftwareSerial.h>
  SoftwareSerial gtSerial(7, 8); // Arduino RX, Arduino TX

#define LED1_PIN 2
#define LED2_PIN 3

// debug flag
#define DEBUG_SERIAL=0;

int num_of_leds = 50;
int led_pins[] = {2, 3}; // digital
int num_strips = 2;

Adafruit_NeoPixel strips[2] = {
  Adafruit_NeoPixel(num_of_leds, LED1_PIN, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(num_of_leds, LED2_PIN, NEO_GRB + NEO_KHZ800)
};
int strip_curent_positions[] = {0, 0};
int strip_goto_positions[] = {0, 0};
int y, sensorValue; // tmp parameter to help map the data
String str="r";

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  gtSerial.begin(38400);
  for (int i = 0; i < num_strips; i++) {
    strips[i].begin();
    strips[i].show();
  }
  sensorValue = y = 0;
}

// the loop routine runs over and over again forever:
void loop() {
  if (gtSerial.available()>2) {
    str = gtSerial.readStringUntil('\n');
    Serial.println(str);
  }
  if (str.substring(0, 6).toInt() >= 0 and str.substring(0, 6).toInt() <= 999999) // if get 000000 to 200200 read how many LEDS
  { 
    ledsOn(str.substring(0, 3).toInt(), str.substring(3, 6).toInt()); // gets 2 values for "how many leds" per each strip
//    Serial.println("Led");
  }
  if (str.startsWith("f")) {
    offAll();
    theaterChase(20);
    offAll();
    str="r";
  }
  if (str.startsWith("r")) {
    rainbowAll();
  }
  if (str.startsWith("o")) {
    offAll();
  }
  if (str.startsWith("s")) {
    offAll();
    ledsStart();
    str="o";
  }
}

void ledsOn(uint32_t score1, uint32_t score2) {
  uint32_t i;
  strip_goto_positions[0] = score1;
  strip_goto_positions[1] = score2;
  for (i = 0; i < num_strips; i++) {
    strip_curent_positions[i];
    strip_goto_positions[i];
    if (strip_curent_positions[i] < strip_goto_positions[i]) {
      strips[i].setPixelColor(strip_curent_positions[i], strips[i].Color(255, 0, 0));
      strips[i].show();
      delay(2);
      strip_curent_positions[i]++;
    }
    else if (strip_curent_positions[i] > strip_goto_positions[i]) {
      strips[i].setPixelColor(strip_curent_positions[i], strips[i].Color(0, 0, 0));
      strips[i].show();
      delay(2);
      strip_curent_positions[i]--;
    }
  }
}

void ledsStart() {
  uint32_t i, x;
  for(i=0; i<num_of_leds; i++) {
     for (x = 0; x < num_strips; x++){
      strips[x].setPixelColor(i, strips[x].Color(0, 0, 255));
      strips[x].show();
      delay(20);
     }
  }

  for(i=num_of_leds; i>0; i--) {
     for (x = 0; x < num_strips; x++){
      strips[x].setPixelColor(i, strips[x].Color(0, 0, 0));
      strips[x].show();
      delay(20);
     }
  }

}

void theaterChase(uint8_t wait) {
  for (int j = 0; j < 3; j++) { //do 10 cycles of chasing
    for (int q = 0; q < 3; q++) {
      for (uint16_t i = 0; i < num_of_leds; i = i + 3) {
        for (int x = 0; x < num_strips; x++) {
          for (int x = 0; x < num_strips; x++) {
            strips[x].setPixelColor(i + q, strips[x].Color(127, 127, 127));  //turn every third pixel on
          }
          strips[x].show();
          delay(wait);
          for (uint16_t i = 0; i < strips[x].numPixels(); i = i + 3) {
            strips[x].setPixelColor(i + q, 0);      //turn every third pixel off
          }
        }
      }
    }
  }
}

void offAll() {
  uint16_t i;
    for(i=0; i < num_of_leds; i++) {
      for (int x = 0; x < num_strips; x++) {
        strips[x].setPixelColor(i, strips[x].Color(0, 0, 0));
        strips[x].show();
      }
    }
}

void rainbowAll() {
  uint16_t i, j, t;  
  for(t=0; t<1; t++) {
    for(j=0; j<256; j++) {
      for(i=0; i<num_of_leds; i++) {
        strips[0].setPixelColor(i, Wheel((i+j) & 255));
        strips[1].setPixelColor(i, Wheel((i+j) & 255));
      }
      strips[0].show();
      strips[1].show();
      delay(15);
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strips[0].Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strips[0].Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strips[0].Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

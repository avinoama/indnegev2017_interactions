#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define LED1_PIN 2
#define LED2_PIN 3

// debug flag
#define DEBUG_SERIAL=0;

int num_of_leds = 14;
int led_pins[] = {2, 3}; // digital
int num_strips = 2;

Adafruit_NeoPixel strips[2] = {
  Adafruit_NeoPixel(num_of_leds, LED1_PIN, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(num_of_leds, LED2_PIN, NEO_GRB + NEO_KHZ800)
};
int strip_curent_positions[] = {0, 0};
int strip_goto_positions[] = {0, 0};
int y, sensorValue; // tmp parameter to help map the data
int led_s1;

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  for (int i = 0; i < num_strips; i++) {
    strips[i].begin();
    strips[i].show();
  }
  sensorValue = y = 0;
}

// the loop routine runs over and over again forever:
void loop() {
  if ( Serial.available()>=4) {
     led_s1 = Serial.parseInt(); 
    //led_s1 = Serial.read();
  }
  Serial.println(led_s1);
  ledsOn(led_s1, 10); // gets 2 values for "how many leds" per each strip
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
      delay(20);
      strip_curent_positions[i]++;
    }
    else if (strip_curent_positions[i] > strip_goto_positions[i]) {
      strips[i].setPixelColor(strip_curent_positions[i], strips[i].Color(0, 0, 0));
      strips[i].show();
      delay(20);
      strip_curent_positions[i]--;
    }
  }
}


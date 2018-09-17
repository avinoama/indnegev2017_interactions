#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define DEBUG 1
#define LED1_PIN 2
#define LED2_PIN 3

int num_of_leds = 100;
int led_pins[] = {2, 3}; // digital
int num_strips = 2;

Adafruit_NeoPixel strips[2] = {
  Adafruit_NeoPixel(num_of_leds, LED1_PIN, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(num_of_leds, LED2_PIN, NEO_GRB + NEO_KHZ800)
};
uint32_t color_map[] = {
  strips[0].Color(255, 0, 0),      // Red
  strips[0].Color(128, 0, 128),   // yellow
  strips[0].Color(255, 255, 0),   // Purple
  strips[0].Color(0, 0, 255),     // Blue
  strips[0].Color(0, 255, 0)     // green
};
int color_amount = 5;
int current_flickers = 0;
uint32_t current_color = strips[0].Color(0, 0, 0);

String states [3] = {"white", "rainbow", "flicker"};
int switch_state_limit[3] = {5000, 5000, 25};
int current_switch_state = 0;
int current_switch_state_cont = 0;

int strip_curent_positions[] = {0, 0};
int strip_goto_positions[] = {0, 0};




// the setup routine runs once when you press reset:
void setup() {

  for (int i = 0; i < num_strips; i++) {
    strips[i].begin();
    strips[i].show();
  }
  Serial.begin(9600);

}

// the loop routine runs over and over again forever:
void loop() {

  if (states[current_switch_state] == "rainbow") {
    ledsOnColor(num_of_leds, num_of_leds); // gets 2 values for "how many leds" per each strip

  } else if (states[current_switch_state] == "flicker") {
    flikerColors();

  } else if (states[current_switch_state] == "white") {
    ledsOn(num_of_leds, num_of_leds);
  }



  if (current_switch_state_cont >= switch_state_limit[current_switch_state]) {
      
      Serial.println(states[current_switch_state]);
      Serial.println(current_switch_state_cont);
  
    if (states[current_switch_state] == "rainbow") {
      current_switch_state = 2;
      delay(10000);
    } else if (states[current_switch_state] == "flicker") {
      current_switch_state = 0;
      strip_curent_positions[0] = strip_curent_positions[1] = 0;
    } else if (states[current_switch_state] == "white") {
      delay(10000);
      current_switch_state = 1;
      strip_curent_positions[0] = strip_curent_positions[1] = 0;
    }
    current_switch_state_cont = 0;
  }

  current_switch_state_cont++;



}
void  flikerColors() {
  //current_flickers
  
//   Serial.print("current_flickers " ); Serial.print(current_flickers);Serial.println();

  
  // turn to white
  for (int i = 0; i < num_of_leds; i++) {
    strips[current_flickers].setPixelColor(i,  strips[0].Color(255, 255, 255));
  }
  strips[current_flickers].show();

  delay(100);
  

  for (int i = 0; i < num_of_leds; i++) {
    if ( i  <= (num_of_leds / color_amount) * 1 ) {
      current_color = color_map[0];
      // <40
    } else if (i  > (color_amount / num_of_leds)
               && i <= (num_of_leds / color_amount) * 2) {
      current_color = color_map[1];
      // <60
    } else if (i  > (num_of_leds / color_amount) * 2
               && i <= (num_of_leds / color_amount) * 3) {

      current_color = color_map[2];

      // <80
    } else if (i  > (num_of_leds / color_amount) * 3
               && i <= (num_of_leds / color_amount) * 4) {
      current_color = color_map[3];

      // <100
    } else if (i  > (num_of_leds / color_amount) * 4
               && i <= (num_of_leds / color_amount) * 5) {
      current_color = color_map[4];
    }

    strips[current_flickers].setPixelColor(i,  current_color);
  }
  
  strips[current_flickers].show();
  delay(100);
  
  
  current_flickers++;
  if (current_flickers >= num_strips) {
    current_flickers = 0;
  }


}
void ledsOnColor(uint32_t score1, uint32_t score2) {
  uint32_t i;
  strip_goto_positions[0] = score1;
  strip_goto_positions[1] = score2;
  for (i = 0; i < num_strips; i++) {

    strip_curent_positions[i];
    strip_goto_positions[i];
    if (strip_goto_positions[i] > strip_curent_positions[i]) {
      // go forward by one iteration

      // if < from first step take color in postiion 0
      // @todo rewrite it as a loop
      // <20
      if ( strip_curent_positions[i]  <= (num_of_leds / color_amount) * 1 ) {
        current_color = color_map[0];
        // <40
      } else if (strip_curent_positions[i]  > (color_amount / num_of_leds)
                 && strip_curent_positions[i] <= (num_of_leds / color_amount) * 2) {
        current_color = color_map[1];
        // <60
      } else if (strip_curent_positions[i]  > (num_of_leds / color_amount) * 2
                 && strip_curent_positions[i] <= (num_of_leds / color_amount) * 3) {

        current_color = color_map[2];

        // <80
      } else if (strip_curent_positions[i]  > (num_of_leds / color_amount) * 3
                 && strip_curent_positions[i] <= (num_of_leds / color_amount) * 4) {
        current_color = color_map[3];

        // <100
      } else if (strip_curent_positions[i]  > (num_of_leds / color_amount) * 4
                 && strip_goto_positions[i] <= (num_of_leds / color_amount) * 5) {
        current_color = color_map[4];
      }



      strips[i].setPixelColor(strip_curent_positions[i], current_color);
      strips[i].show();
      delay(20);
      strip_curent_positions[i]++;
    }
    else if (strip_curent_positions[i] > strip_goto_positions[i]) {
      strips[i].setPixelColor(strip_curent_positions[i], strips[i].Color(0, 0, 0));
      strips[i].show();
      delay(30);
      strip_curent_positions[i]--;
    }
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
      strips[i].setPixelColor(strip_curent_positions[i], strips[i].Color(255, 255, 255));
      strips[i].show();
      delay(20);
      strip_curent_positions[i]++;
    }
    else if (strip_curent_positions[i] > strip_goto_positions[i]) {
      strips[i].setPixelColor(strip_curent_positions[i], strips[i].Color(0, 0, 0));
      strips[i].show();
      delay(30);
      strip_curent_positions[i]--;
    }
  }
}






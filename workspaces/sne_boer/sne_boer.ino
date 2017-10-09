#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

/*
  Sne Boar
  is an interaction designed for talking to god that his image is an endless burning bush.

  The interaction is build ut of 2 parts.
  1. the image of god - the endless burning bush.
  2. a cone shape device that you talk to god through.

  The setup is a mountain you have to climb to talk into the cone shape device, and bebeath the mountain is a tree that is covered in led strips.
  each strip will be a single flame and all the flames togther will react to the person talking to god.


*/



// debug flaf
#define DEBUG_SERIAL 0
#define NUM_STRIPS 2
#define NUM_COLORS 4


int num_of_leds[] = {14, 14};
int sensor_pin = A2; // analog
//Adafruit_NeoPixel strips[2];// = new Adafruit_NeoPixel[2];
Adafruit_NeoPixel strips[2] = {
  Adafruit_NeoPixel(num_of_leds[0], 2, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(num_of_leds[1], 3, NEO_GRB + NEO_KHZ800)
};


int current[2];
int color_pos[2];

int y, sensorValue; // tmp parameter to help map the data



uint32_t color_map[] = {
  strips[0].Color(255, 0, 0),      // Red
  strips[0].Color(128, 0, 128),   // yellow
  strips[0].Color(255, 255, 0),   // Purple
  strips[0].Color(0, 0, 255)     // Blue
};

// the color should be
// red on top 10%     example 60*0.1 = 6
// yellow  after 40%  example 60*0.4 = 24
// purple after 25%   example 60*0.25 = 15
// blue  after  25%   example 60*0.25 = 15
float strip_color_range[NUM_STRIPS][NUM_COLORS] = {
  {0.1, 0.4, 0.25, 0.25},
  {0.1, 0.4, 0.25, 0.25}
};



// [num_strips][num_of_leds]
//uint32_t strip_color_maps[][];



// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);

  for (int i = 0; i < NUM_STRIPS; i++) {
//    strips[i] = Adafruit_NeoPixel(num_of_leds, num_of_leds[i], NEO_GRB + NEO_KHZ800);

    strips[i].begin();
    strips[i].show();

    strip_color_range[i];

    color_pos[i] = current[i] = 0;

  }
  sensorValue = y =  0;
}

// the loop routine runs over and over again forever:
void loop() {
  // read the input from god:
  sensorValue = analogRead(sensor_pin);// this will change to communication from another arduino
  if (DEBUG_SERIAL) {
    // Serial.println(sensorValue);
  }

  for (int i = 0; i < NUM_STRIPS; i++) {
    // print out the value you read:
    y = map(sensorValue, 1, 1000, 0, num_of_leds[i]);
    // map color to
    
    if (DEBUG_SERIAL) {
      // check pixels are ok
       Serial.println(num_of_leds[i]);
       Serial.print(sensorValue);Serial.print(" - ");Serial.println(y);Serial.print(" > ");Serial.println(current[i]);
    }
    


    // psado code
    // if up
    // check what color need to add at the buttom and push it
    //   That mean light up another led from the buttom and from the top
    // if down
    // overwrith the color with the top color and remove top

    //current[i];
    //color_pos[i] = map(y, 0, num_of_leds[i], 0, 2);


    if (y > current[i] && current[i] < num_of_leds) {
      
      //map_strip(i,y);
      
      current[i]++;
      strips[i].setPixelColor(current[i], color_map[0]);
      
    } else if (y < current[i] && current[i] > 0) {
      
      strips[i].setPixelColor(current[i], 0);
      current[i]--;
      
    }
    strips[i].show();
  }
  
  if (DEBUG_SERIAL) {
    delay(1000);
  }
  delay(1);        // delay in between reads for stability
}

void map_strip(int strip_num,int pos) {
  // foreach color untill pos map colors
  // color_pos
  // strip_color_range
  int range = 0;
  int range_pos = 1;
  for(int i=0;i<pos;i++) {
    if((strip_color_range[strip_num][range]*pos)>i) {
      range_pos++;
      range++;
    }
    strips[i].setPixelColor(current[i], color_map[range_pos]);
  }
}

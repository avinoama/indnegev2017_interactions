#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

// debug flaf
#define DEBUG_SERIAL 0
#define NUM_STRIPS 2
#define NUM_COLORS 4


int num_of_leds[] = {60,60};
int sensor_pins[] = {1,2}; // analog
Adafruit_NeoPixel strips[2] = new Adafruit_NeoPixel[2];


int current[2];
int color_pos[2];

int y,sensorValue;// tmp parameter to help map the data


 
uint32_t color_map[] = {
  strips[0].Color(128, 0, 128),   // yellow
  strips[0].Color(0, 0, 255),     // green
  strips[0].Color(255, 255, 0),   // Purple
  strips[0].Color(255, 0, 0)      // Red
};

// the color should be
// red on top 10%     example 60*0.1 = 6
// yellow  after 40%  example 60*0.4 = 24
// purple after 25%   example 60*0.25 = 15
// blue  after  25%   example 60*0.25 = 15
float strip_color_range[NUM_STRIPS][NUM_COLORS] = {
  {0.1,0.4,0.25,0.25},
  {0.1,0.4,0.25,0.25}
  };


// [num_strips][num_of_leds]
//uint32_t strip_color_maps[][];



// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);

  for(int i=0;i<num_strips;i++) {
    strips[i] = Adafruit_NeoPixel(num_of_leds, num_of_leds[i], NEO_GRB + NEO_KHZ800);  
    
    strips[i].begin();
    strips[i].show();
    
    strip_color_map[i];
    
    color_pos[i] = current[i] = 0;
  }
  sensorValue=y=0;
}

// the loop routine runs over and over again forever:
void loop() {
    // read the input from god:
    sensorValue = analogRead(A0);// this will change to communication from another arduino

        
   for(int i=0;i<num_strips;i++) {
    // print out the value you read:
    y = map(sensorValue, 1, 600, 0, num_of_leds[i]);




    // psado code
    // if up
    // check what color need to add at the buttom and push it
    //   That mean light up another led from the buttom and from the top
    // if down
    // overwrith the color with the top color and remove top

    current[i]
    //color_pos[i] = map(y, 0, num_of_leds[i], 0, 2);

        
    if(y>current[i] && current[i]<num_of_leds) {
      current[i]++;
      strips[i].setPixelColor(current[i], color_map[color_pos]);
    } else if (y<current[i] && current[i]>=0) {
      strips[i].setPixelColor(current[i], 0);
      current[i]--;
    }
    strips[i].show();
    
    if(DEBUG_SERIAL) {
      delay(1000);
    }
  }
  delay(1);        // delay in between reads for stability
}

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif
#define PIN_BUTTON1 2
#define PIN_BUTTON1 3

// debug flaf
#define DEBUG_SERIAL=0;

int num_of_leds = 60;
int sensor_pins[] = {1,2}; // analog
int button_pins[] = {2,3}; // digital
int num_buttons = 2;
Adafruit_NeoPixel strips[2] = new Adafruit_NeoPixel[2];
int current[] = {0,0};
int color_pos[]=  {0,0};

// int myPins[] = {2, 4, 8, 3, 6};
//  int mySensVals[6] = {2, 4, -8, 3, 2};

int y,sensorValue;// tmp parameter to help map the data


uint32_t color_map[] = {
  strips[0].Color(128, 0, 128),   // yellow
  strips[0].Color(0, 0, 255),     // green
  strips[0].Color(255, 255, 0),   // Purple
  strips[0].Color(255, 0, 0)      // Red
};

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);

  for(int i=0;i<num_buttons;i++) {
    strips[i] = Adafruit_NeoPixel(num_of_leds, button_pins[i], NEO_GRB + NEO_KHZ800);  
    
    strips[i].begin();
    strips[i].show();
    color_pos[i] = current[i] = 0;
  }
  sensorValue=y=0;
}

// the loop routine runs over and over again forever:
void loop() {
   for(int i=0;i<num_buttons;i++) {
    // read the input on analog pin 0:
    sensorValue = analogRead(sensor_pins[i]);
    // print out the value you read:
    y = map(sensorValue, 1, 600, 0, num_of_leds);
    color_pos[i] = map(y, 0, num_of_leds, 0, 2);
  
    if(DEBUG_SERIAL) {
      Serial.print(sensorValue);
      Serial.print(" - ");
      Serial.print(y);
      Serial.print(" - ");
      Serial.print(color_pos);
      Serial.println();
    }
    
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



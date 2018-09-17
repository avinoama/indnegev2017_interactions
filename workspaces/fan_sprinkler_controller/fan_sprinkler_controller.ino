

/*


מחפש תנועה
מזהה תנועה, מדליק ריננבואו בסטריפ, סופר 10 שניות
מדליק ריליי מאורר, 0.5 שניה אחכ מדליק ריליי סולונויד, לד מהבהב בלבן (?), סופר 20 שניות, סוגר הכל
30 שניות לא מקבל תנועה



*/
#include <Adafruit_NeoPixel.h>

unsigned long SLEEP_TIME = 120000; // Sleep time between reports (in milliseconds)
#define DIGITAL_INPUT_SENSOR 12   // The digital input you attached your motion sensor.  (Only 2 and 3 generates interrupt!)

#define LED_PIN 11

#define SPRINKLER_PIN 8
#define FAN_PIN 9

#define DEBUG 0


Adafruit_NeoPixel strip = Adafruit_NeoPixel(50, LED_PIN, NEO_GRB + NEO_KHZ800);


void setup() {
  if(DEBUG) {
    Serial.begin(9600);
  }
  
  pinMode(DIGITAL_INPUT_SENSOR, INPUT);      // sets the motion sensor digital pin as input
  
  pinMode(LED_PIN, OUTPUT);
  pinMode(FAN_PIN, OUTPUT);
  pinMode(SPRINKLER_PIN, OUTPUT);

  digitalWrite(SPRINKLER_PIN, HIGH);
  digitalWrite(FAN_PIN, HIGH);  
  
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  delay(3000);
}


void loop() {
  // Read digital motion value
  bool tripped = digitalRead(DIGITAL_INPUT_SENSOR) == HIGH;

  if(DEBUG) {
    Serial.println(tripped);
  }
  if(tripped) {
    if(DEBUG) {
      Serial.println("Fan");
    }   
    delay(5000);
   
    digitalWrite(FAN_PIN, LOW);  
    
    delay(1000);
    
    colorWipe(strip.Color(255, 0, 0), 10); // Red
    
    delay(1000);

    if(DEBUG) {
      Serial.println("Sprinklers");
    }   
    digitalWrite(SPRINKLER_PIN, LOW);  
    
    
    for(int i=0;i<3;i++) {
      colorWipe(strip.Color(0, 255, 0), 10); // Green
      colorWipe(strip.Color(0, 0, 255), 10); // Blue
      colorWipe(strip.Color(204, 51, 255), 10); // purple
  
      colorWipe(strip.Color(255, 255, 0), 10); // yellow
      colorWipe(strip.Color(255, 153, 0), 10); // orenge
    
    }
    
    digitalWrite(SPRINKLER_PIN, HIGH);

    for(int i=0;i<10;i++) {
      colorWipe(strip.Color(0, 255, 0), 10); // Green
      colorWipe(strip.Color(0, 0, 255), 10); // Blue
      colorWipe(strip.Color(204, 51, 255), 10); // purple
  
      colorWipe(strip.Color(255, 255, 0), 10); // yellow
      colorWipe(strip.Color(255, 153, 0), 10); // orenge
    
    }
    
    digitalWrite(FAN_PIN, HIGH);  
    // wait 30 sec
    //delay(3000);
    

  }
  

  
  
  delay(100);
}
// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}


void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
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

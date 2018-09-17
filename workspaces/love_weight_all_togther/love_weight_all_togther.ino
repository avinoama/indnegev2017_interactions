#include <SoftwareSerial.h>
#include <Adafruit_NeoPixel.h>

#ifdef __AVR__
#include <avr/power.h>
#endif


// debug flag
#define DEBUG_SERIAL 1

#define CMD_SEL_DEV 0X09
#define DEV_TF 0X02
#define CMD_PLAY_W_VOL 0X22
#define CMD_PLAY 0X0D
#define CMD_PAUSE 0X0E
#define CMD_PREVIOUS 0X02
#define CMD_NEXT 0X01


#define LED1_PIN 2
#define LED2_PIN 3

int num_of_leds = 200;
int led_pins[] = {2, 3}; // digital
int num_strips = 2;

Adafruit_NeoPixel strips[2] = {
  Adafruit_NeoPixel(num_of_leds, LED1_PIN, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(num_of_leds, LED2_PIN, NEO_GRB + NEO_KHZ800)
};
int strip_curent_positions[] = {0, 0};
int strip_goto_positions[] = {0, 0};



static int8_t Send_buf[8] = {0} ;

SoftwareSerial soundSerial(5, 6);


int sensor_pins[] = {2, 3}; // analog
int sensor_values[] = {0, 0};
int last_values[] = {0, 0};

int sensor_num = 2;
int thresholds[] = {400, 600, 800, 1000};

boolean is_game_started = false;

int y, sensorValue; // tmp parameter to help map the data

char buf[3];
String sensor_value_string = "";


int hey_hey_in_x_rounds = 20000;
int hey_hey_current_round = 0;

int game_over_rounds = 5000;
int game_over_amount = 0;

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  if (DEBUG_SERIAL) {
    Serial.begin(9600);
  }
  soundSerial.begin(9600);
  delay(1000);//Wait chip initialization is complete
  sendCommand(CMD_SEL_DEV, DEV_TF);//select the TF card
  delay(200);//wait for 200ms
  sendCommand(CMD_PLAY_W_VOL, 0X1E01);//play the first song with volume 30 class
  delay(6000);

  //  heartSerial.begin(9600);
  sensorValue = y = 0;
  delay(1000);

  for (int i = 0; i < num_strips; i++) {
    strips[i].begin();
    strips[i].show();
  }
  sensorValue = y = 0;


  rainbowAll();
}


// the loop routine runs over and over again forever:
void loop() {
  
  // get both sensor values
  for (int i = 0; i < sensor_num; i++ ) {
    sensor_values[i] = analogRead(sensor_pins[i]);

    if (is_game_started == false) {

      // check if game hasent started
      if (sensor_values[i] > thresholds[0] && last_values[i] < thresholds[0]) {
        // start jumping
        //        heartSerial.write('o');
        Serial.println("Highest Jump");

        sendCommand(CMD_PLAY_W_VOL, 0X1E02); // borchim habaim Highest Jump
        ledsStart();
        delay(5000);
        sendCommand(CMD_PLAY_W_VOL, 0X1E04); // Gong

        is_game_started = true;
      }
    }

    last_values[i] = sensor_values[i];
  }

  ledsOn(map(sensor_values[0], 0, 1024, 0, num_of_leds), map(sensor_values[1], 0, 1024, 0, num_of_leds));



  if (DEBUG_SERIAL) {
       Serial.print(sensor_values[0]);Serial.print(" - ");Serial.print(sensor_values[1]);Serial.println();
  }


  if (is_game_started == true) {
    if (sensor_values[0] > thresholds[3] && sensor_values[1] > thresholds[3] ) {
      // great success
      sendCommand(CMD_PLAY_W_VOL, 0X1E05); // Victory
      theaterChase(2);
      Serial.println("Victory");
      delay(1000);

      is_game_started = false;
      hey_hey_current_round = 0;
    } else  {
      //      heartSerial.write(s);

      //      sendCommand(CMD_PLAY_W_VOL, 0X1E04); // Ha ha try again
      //      delay(10000);
      //      sendCommand(CMD_PLAY_W_VOL, 0X1E03); // Gong
      if (game_over_amount >= game_over_rounds) {

        rainbowAll();
        is_game_started = false;
        hey_hey_current_round = 0;
        game_over_amount = 0;
      }

      game_over_amount++;


    }

  } else {
    if (hey_hey_current_round >= hey_hey_in_x_rounds) {
      Serial.println("Hey Hey");
      hey_hey_current_round = 0;
      sendCommand(CMD_PLAY_W_VOL, 0X1E01); // Hey Hey
      rainbowAll();
    }
    hey_hey_current_round++;
  }


  delay(2);        // delay in between reads for stability
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
      //delay(2);
      strip_curent_positions[i]++;
    }
    else if (strip_curent_positions[i] > strip_goto_positions[i]) {
      strips[i].setPixelColor(strip_curent_positions[i], strips[i].Color(0, 0, 0));
      strips[i].show();
      //delay(2);
      strip_curent_positions[i]--;
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




void ledsStart() {
  uint32_t i, x;
  for (i = 0; i < num_of_leds; i++) {
    for (x = 0; x < num_strips; x++) {
      strips[x].setPixelColor(i, strips[x].Color(0, 0, 255));
      strips[x].show();
      delay(20);
    }
  }

  for (i = num_of_leds; i > 0; i--) {
    for (x = 0; x < num_strips; x++) {
      strips[x].setPixelColor(i, strips[x].Color(0, 0, 0));
      strips[x].show();
      delay(20);
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

void sendCommand(int8_t command, int16_t dat)
{
  delay(20);
  Send_buf[0] = 0x7e; //starting byte
  Send_buf[1] = 0xff; //version
  Send_buf[2] = 0x06; //the number of bytes of the command without starting byte and ending byte
  Send_buf[3] = command; //
  Send_buf[4] = 0x00;//0x00 = no feedback, 0x01 = feedback
  Send_buf[5] = (int8_t)(dat >> 8);//datah
  Send_buf[6] = (int8_t)(dat); //datal
  Send_buf[7] = 0xef; //ending byte
  for (uint8_t i = 0; i < 8; i++) //
  {
    soundSerial.write(Send_buf[i]) ;
  }
}

#include <SoftwareSerial.h>

#ifdef __AVR__
  #include <avr/power.h>
#endif

SoftwareSerial sondSerial(5, 6);


// debug flaf
#define DEBUG_SERIAL 0

int num_of_leds = 60;
int sensor_pins[] = {4,7}; // analog
int sensor_values[] = {0,0};
int last_values[] = {0,0};

int sensor_num = 2;
int thresholds[] = {200,400,800,1000};
boolean is_game_started=false;
int y,sensorValue;// tmp parameter to help map the data

//SoftwareSerial mp3(ARDUINO_RX, ARDUINO_TX);
//#define mp3 Serial3    // Connect the MP3 Serial Player to the Arduino MEGA Serial3 (14 TX3 -> RX, 15 RX3 -> TX)

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  if(DEBUG_SERIAL) {
    Serial.begin(9600);
  }
  sensorValue=y=0;
}


// the loop routine runs over and over again forever:
void loop() {
    for(int i=0;i<sensor_num;i++ ) {
      sensor_values[i] = analogRead(sensor_pins[1]);
      if(sensor_values[i] > thresholds[0] && last_values[i]<thresholds[0]) {
        // start jumping
        // send message reset all the zero
        is_game_started=true;
      }
      last_values[i] = sensor_values[i]; 
    }

    if(is_game_started==true) {
       if(sensor_values[0]> 1000 && sensor_values[2]>1000 ) {
          // great success  
          
          // send great success to heart module
          is_game_started=false;
        }
     

      
    }
    
    
    if(DEBUG_SERIAL) {
      Serial.println(sensorValue);
      delay(1000);
    }
  
  delay(2);        // delay in between reads for stability
}

#include <SoftwareSerial.h>

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

static int8_t Send_buf[8] = {0} ;

SoftwareSerial soundSerial(5, 6);
SoftwareSerial heartSerial(8, 7);


int sensor_pins[] = {2,3}; // analog
int sensor_values[] = {0,0};
int last_values[] = {0,0};

int sensor_num = 2;
int thresholds[] = {200,400,800,1000};

boolean is_game_started=false;

int y,sensorValue;// tmp parameter to help map the data

char buf[3];
String sensor_value_string="";


int hey_hey_in_x_rounds = 20000;
int hey_hey_current_round = 0;

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  if(DEBUG_SERIAL) {
    Serial.begin(9600);
  } 
  
  heartSerial.begin(9600);
  heartSerial.write("s\n");
 Serial.write("s\n");
  sensorValue=y=0;
}


// the loop routine runs over and over again forever:
void loop() {

 

  
  delay(2);        // delay in between reads for stability
}


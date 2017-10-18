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
//   soundSerial.begin(9600);
//   sendCommand(CMD_SEL_DEV, DEV_TF);//select the TF card  
  
  heartSerial.begin(9600);
  delay(20);
  heartSerial.write("ooo\n");
  sensorValue=y=0;
  delay(1000);
}


// the loop routine runs over and over again forever:
void loop() {

    // get both sensor values  
    for(int i=0;i<sensor_num;i++ ) {
      sensor_values[i] = analogRead(sensor_pins[i]);
      
      if(is_game_started==false) {
        // check if game hasent started
          if(sensor_values[i] > thresholds[0] && last_values[i]<thresholds[0]) {
            // start jumping
            heartSerial.write('o');
            sendCommand(CMD_PLAY_W_VOL, 0X1E02); // Highest Jump
            heartSerial.write('o');
            is_game_started=true;
          }    
        }
    
      last_values[i] = sensor_values[i]; 
    }
    
    if(DEBUG_SERIAL) {
            Serial.print(sensor_values[0]);Serial.print(" - ");Serial.print(sensor_values[1]);Serial.println();
    }

    
      sensor_value_string  = getSensorValues();
      char s[7];
      sensor_value_string.toCharArray(s, 7);
      heartSerial.write(s);
          
          if(DEBUG_SERIAL) {
           Serial.println(sensor_value_string);
          }
    
    if(DEBUG_SERIAL) {
      delay(100);
    }
//


  
  delay(2);        // delay in between reads for stability
}

String getSensorValues() {
      sensor_value_string="";
      for(int i=0;i<sensor_num;i++ ) {
          int j = map( sensor_values[i],0,1000,0,200);
          if(j<10) {
            sprintf(buf, "00%d",j);  
          } else if( j<100 && j>9) {
            sprintf(buf, "0%d", j);    
          } else {
            sprintf(buf, "0%d", j);    
          }
          sensor_value_string+= buf;
      }
      sensor_value_string+= "\n";
return   sensor_value_string;
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
  for(uint8_t i=0; i<8; i++)//
  {
    soundSerial.write(Send_buf[i]) ;
  }
}

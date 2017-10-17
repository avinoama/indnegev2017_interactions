/*
Hug tree
its a heart shape spunge that is painted like a heart nd will be hang on trees
Using Attiny 85
 */

#define PULSE_PIN 3 // 0 //3 
#define HEART_PIN 2 // 2
#define DEBUG 0

int delay_time=1000;
int sensorValue=0;

void setup()  { 
  // declare pin 9 to be an output:
  pinMode(PULSE_PIN, OUTPUT);
  if(DEBUG) {
    //Serial.begin(9600);
  }  
  
    
} 

void loop()  {
  // read presure on heart
  sensorValue = analogRead(HEART_PIN);
  if(DEBUG) {
      Serial.println(sensorValue);
  }
  delay_time = map(sensorValue, 1, 1000, 1000, 0);

  // map presure from high to low
  // the hight the preuser the lower the delay
  
  beep(50);
  beep(50);
  delay(delay_time);
}

void beep(unsigned char delayms){
  analogWrite(PULSE_PIN, 20);      // Almost any value can be used except 0 and 255
                           // experiment to get the best tone
  delay(delayms);          // wait for a delayms ms
  analogWrite(PULSE_PIN, 0);       // 0 turns it off
  delay(delayms);          // wait for a delayms ms   
}  


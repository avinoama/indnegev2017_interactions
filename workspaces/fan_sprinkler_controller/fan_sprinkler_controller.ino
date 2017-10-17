

unsigned long SLEEP_TIME = 120000; // Sleep time between reports (in milliseconds)
#define DIGITAL_INPUT_SENSOR 12   // The digital input you attached your motion sensor.  (Only 2 and 3 generates interrupt!)
#define LED_PIN 13   // The digital input you attached your motion sensor.  (Only 2 and 3 generates interrupt!)

void setup() {
  Serial.begin(9600);
  pinMode(DIGITAL_INPUT_SENSOR, INPUT);      // sets the motion sensor digital pin as input
  pinMode(LED_PIN, OUTPUT);
}


void loop() {
  // Read digital motion value
  bool tripped = digitalRead(DIGITAL_INPUT_SENSOR) == HIGH;
  digitalWrite(LED_PIN, tripped);
  
  Serial.println(tripped);

  
  
  delay(100);
}



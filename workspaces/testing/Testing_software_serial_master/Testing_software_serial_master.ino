#include <SoftwareSerial.h>

SoftwareSerial gtSerial(8, 7); // Arduino RX, Arduino TX

void setup() {
  Serial.begin(9600);    // serial / USB port
  gtSerial.begin(9600);  // software serial port
}

byte rx_byte = 0;        // stores received byte

void loop() {
  // check if byte available from USB port
  if (gtSerial.available()) {
    rx_byte = gtSerial.read();
    // send a byte to the software serial port
    Serial.write(rx_byte);
  }

  // check if byte available on the software serial port
  if (gtSerial.available()) {
    // get the byte from the software serial port
    rx_byte = gtSerial.read();
    Serial.write(rx_byte);
  }
}

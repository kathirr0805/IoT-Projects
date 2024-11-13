#include <SoftwareSerial.h>

const int ledPin = 13;  // LED connected to pin 8 on Arduino
SoftwareSerial espSerial(0, 1); // RX, TX (Use pins 10 and 11 for communication with NodeMCU)

void setup() {
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);  // Start with the LED off

  Serial.begin(9600);         // Initialize the main serial communication (for debugging)
  espSerial.begin(9600);      // Initialize SoftwareSerial for ESP8266 communication
}

void loop() {
  if (espSerial.available()) {
    char command = espSerial.read();  // Read the incoming command from NodeMCU

    // Check if the command is '1' or '0'
    if (command == '1') {
      digitalWrite(ledPin, HIGH);  // Turn ON the LED
      Serial.println("LED ON");
    } else if (command == '0') {
      digitalWrite(ledPin, LOW);   // Turn OFF the LED
      Serial.println("LED OFF");
    }
  }
}

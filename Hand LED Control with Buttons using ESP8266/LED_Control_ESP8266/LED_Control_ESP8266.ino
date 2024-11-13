int led1 = D1;  // GPIO pin for LED 1
int led2 = D2;  // GPIO pin for LED 2

void setup() {
  Serial.begin(9600);  // Start serial communication
  pinMode(led1, OUTPUT);  // Set LED 1 pin as output
  pinMode(led2, OUTPUT);  // Set LED 2 pin as output
}

void loop() {
  if (Serial.available() > 0) {
    char command = Serial.read();  // Read the command from Serial

    if (command == '1') {
      digitalWrite(led1, HIGH);  // Turn on LED 1
      delay(1000);               // Keep it on for 1 second
      digitalWrite(led1, LOW);   // Turn off LED 1
    } 
    else if (command == '2') {
      digitalWrite(led2, HIGH);  // Turn on LED 2
      delay(1000);               // Keep it on for 1 second
      digitalWrite(led2, LOW);   // Turn off LED 2
    }
  }
}

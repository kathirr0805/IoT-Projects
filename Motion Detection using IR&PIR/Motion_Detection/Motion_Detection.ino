#define BLYNK_TEMPLATE_ID "TMPL3UXE3xXkL" // Change this to your template ID if necessary
#define BLYNK_TEMPLATE_NAME "Detection"
#define BLYNK_AUTH_TOKEN "6RSjd5kH0E-KLS8ZtENwu6boEFng293T"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// Your WiFi credentials
char ssid[] = "Kathir WIFI";     // Replace with your network SSID
char pass[] = "kathir2005";      // Replace with your network password

// Define pins
#define PIR_PIN D2  // PIR sensor output pin
#define LED_PIN D5  // LED pin for indication (optional)

// Timer
BlynkTimer timer;

// Function to send a notification and control the LED
void sendNotification() {
  Serial.println("Motion detected!"); // Print to Serial Monitor
  digitalWrite(LED_PIN, HIGH); // Turn on the LED
  Blynk.logEvent("led_on", "MOTION IS DETECTED"); // Log event for LED ON
  timer.setTimeout(1000L, []() { // Turn off the LED after 1 second
    digitalWrite(LED_PIN, LOW); 
    Blynk.logEvent("led_off", "MOTION IS NOT DETECTED"); // Log event for LED OFF
  });
}

void setup() {
  Serial.begin(115200);
  pinMode(PIR_PIN, INPUT);    // Set PIR_PIN as INPUT
  pinMode(LED_PIN, OUTPUT);    // Set LED_PIN as OUTPUT
  digitalWrite(LED_PIN, LOW);  // Ensure LED is off initially

  // Connect to WiFi and Blynk
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

void loop() {
  // Check if motion is detected
  if (digitalRead(PIR_PIN) == HIGH) {
    sendNotification(); // Send notification if motion is detected
    delay(5000); // Debounce time to prevent multiple notifications
  }

  Blynk.run(); // Run Blynk
  timer.run(); // Run the timer
}

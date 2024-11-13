#define BLYNK_TEMPLATE_ID "TMPL3UXE3xXkL"
#define BLYNK_TEMPLATE_NAME "Detection"
#define BLYNK_AUTH_TOKEN "qwuoINnsaggyXBeQ_j4Xn28RhqLf_pI5"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// Your WiFi credentials
char ssid[] = "Kathir WIFI";     // Replace with your network SSID
char pass[] = "kathir2005";      // Replace with your network password

// Define LED pin
#define LED_PIN D2

// Blynk Timer
BlynkTimer timer;

// Function to turn the LED on
BLYNK_WRITE(V0) { // Assuming you are using virtual pin V0 for the push button
  int buttonState = param.asInt(); // Get the state of the button (0 or 1)
  
  if (buttonState == 1) { // If button is pressed
    digitalWrite(LED_PIN, HIGH);  // Turn on the LED
    Serial.println("LED state: ON");  // Print LED state to Serial Monitor
    Blynk.logEvent("led_on", "LED is ON");  // Log event to Blynk (will trigger notification)
  } else {
    digitalWrite(LED_PIN, LOW);  // Turn off the LED
    Serial.println("LED state: OFF");  // Print LED state to Serial Monitor
    Blynk.logEvent("led_off", "LED is OFF");  // Log event to Blynk (will trigger notification)
  }
}

void setup() {
  // Debug console
  Serial.begin(9600);
  
  // Set LED pin as output
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);  // Ensure LED is off initially

  // Connect to WiFi and Blynk
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi not connected!");
  }
  
  if (Blynk.connected()) {
    Blynk.run();  // Run Blynk when connected
  } else {
    Serial.println("Blynk not connected, retrying...");
    Blynk.connect();  // Attempt to reconnect if disconnected
  }
}

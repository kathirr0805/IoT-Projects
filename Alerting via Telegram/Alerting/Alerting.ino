#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>

// Replace the following!
char ssid[] = "Kathir WIFI";         // your network SSID (name)
char password[] = "kathir2005";      // your network password
#define TELEGRAM_BOT_TOKEN "7012288515:AAFyuuL7Kx2150PdluNDWKUaRWrZdvzmrzo" // Your Telegram Bot Token
#define CHAT_ID "2114649840"          // Your Telegram Chat ID

// Initialize the Wifi client and the Telegram bot
WiFiClientSecure client;
UniversalTelegramBot bot(TELEGRAM_BOT_TOKEN, client);

#define LED_PIN D1 // GPIO pin connected to the LED

unsigned long lastNotificationSent = 0;

void setup() {
  Serial.begin(115200);

  // Set WiFi to station mode and disconnect from an AP if it was previously connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH); // Turn on the LED initially

  // Attempt to connect to Wifi network
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Only required on 2.5 Beta
  client.setInsecure();
}

void sendLEDStatusNotification() {
  if (millis() - lastNotificationSent > 5000) { // Check if 5 seconds have passed
    bot.sendMessage(CHAT_ID, "LED is turned ON", "");
    Serial.println("LED is turned ON");
    lastNotificationSent = millis();
  }
}

void loop() {
  sendLEDStatusNotification();
}
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>

// Replace the following!
char ssid[] = "Kathir WIFI";         // your network SSID (name)
char password[] = "kathir2005";      // your network password
#define TELEGRAM_BOT_TOKEN "7012288515:AAFyuuL7Kx2150PdluNDWKUaRWrZdvzmrzo" // Your Telegram Bot Token

// Initialize the Wifi client and the Telegram bot
WiFiClientSecure client;
UniversalTelegramBot bot(TELEGRAM_BOT_TOKEN, client);

#define LED_PIN D1 // GPIO pin connected to the LED

int delayBetweenChecks = 1000;
unsigned long lastTimeChecked;   // Last time messages' scan has been done

void setup() {
  Serial.begin(115200);

  // Set WiFi to station mode and disconnect from an AP if it was previously connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

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

  // Set longPoll to improve response time of the bot
  bot.longPoll = 60;
}

void handleNewMessages(int numNewMessages) {
  for (int i = 0; i < numNewMessages; i++) {
    // If the type is a "callback_query", an inline keyboard button was pressed
    if (bot.messages[i].type == F("callback_query")) {
      String text = bot.messages[i].text;
      Serial.print("Callback button pressed with text: ");
      Serial.println(text);

      if (text == F("ON")) {
        digitalWrite(LED_PIN, HIGH);
        Serial.println("LED ON");
      } 
      else if (text == F("OFF")) {
        digitalWrite(LED_PIN, LOW);
        Serial.println("LED OFF");
      }
    } else {
      String chat_id = String(bot.messages[i].chat_id);
      String text = bot.messages[i].text;

      if (text == F("/options")) {
        // Keyboard JSON is an array of arrays
        String keyboardJson = F("[[{ \"text\" : \"ON\", \"callback_data\" : \"ON\" },{ \"text\" : \"OFF\", \"callback_data\" : \"OFF\" }]]");
        bot.sendMessageWithInlineKeyboard(chat_id, "Control the LED", "", keyboardJson);
      }

      // When a user first uses a bot they will send a "/start" command
      if (text == F("/start")) {
        bot.sendMessage(chat_id, "Use /options to control the LED\n", "Markdown");
      }
    }
  }
}

void loop() {
  if (millis() > lastTimeChecked + delayBetweenChecks) {
    // getUpdates returns 1 if there is a new message from Telegram
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    if (numNewMessages) {
      Serial.println("Got response");
      handleNewMessages(numNewMessages);
    }

    lastTimeChecked = millis();
  }
}
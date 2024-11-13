#include <SinricPro.h>
#include <SinricProSwitch.h>
#include <ESP8266WiFi.h>

#define WIFI_SSID         "Kathir WIFI"      // Your Wi-Fi SSID
#define WIFI_PASS         "kathir2005"      // Your Wi-Fi Password

#define APP_KEY           "e3643330-d760-4aea-8cae-78cc52bdebbe"        // Sinric Pro App Key
#define APP_SECRET        "fbe49ba0-e40b-48b7-9aa0-4f69eef30290-66f85dc9-c859-4899-b196-6fc4b73fb7d2"  // Sinric Pro App Secret

#define SWITCH_ID_BUZZER  "670a3fd3deddece34bac3f8c"    // Switch ID for Buzzer
#define SWITCH_ID_LED     "670a3f1d54041e4ff64d55e3"    // Switch ID for LED

#define BUZZER_PIN        D2    // Pin for Buzzer
#define LED_PIN           D1    // Pin for LED

// Function to handle the buzzer switch
bool onBuzzerPowerState(const String &deviceId, bool &state) {
  if (deviceId == SWITCH_ID_BUZZER) {
    digitalWrite(BUZZER_PIN, state ? HIGH : LOW);  // Turn buzzer on/off
    Serial.printf("Buzzer %s\n", state ? "ON" : "OFF");
    return true;
  }
  return false;
}

// Function to handle the LED switch
bool onLEDPowerState(const String &deviceId, bool &state) {
  if (deviceId == SWITCH_ID_LED) {
    digitalWrite(LED_PIN, state ? HIGH : LOW);  // Turn LED on/off
    Serial.printf("LED %s\n", state ? "ON" : "OFF");
    return true;
  }
  return false;
}

void setupWiFi() {
  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void setupSinricPro() {
  SinricProSwitch &buzzerSwitch = SinricPro[SWITCH_ID_BUZZER];
  buzzerSwitch.onPowerState(onBuzzerPowerState);

  SinricProSwitch &ledSwitch = SinricPro[SWITCH_ID_LED];
  ledSwitch.onPowerState(onLEDPowerState);

  SinricPro.begin(APP_KEY, APP_SECRET);
}

void setup() {
  Serial.begin(115200);

  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  digitalWrite(BUZZER_PIN, LOW);  // Buzzer off initially
  digitalWrite(LED_PIN, LOW);     // LED off initially

  setupWiFi();
  setupSinricPro();
}

void loop() {
  SinricPro.handle();  // Handle requests from Alexa
}

#include <ESP8266WiFi.h>

// Wi-Fi credentials
const char* ssid = "Kathir WIFI";  // Your Wi-Fi SSID
const char* password = "kathir2005";  // Your Wi-Fi password

WiFiServer server(80);  // Start a web server on port 80

void setup() {
  Serial.begin(9600);  // Initialize serial communication with Arduino
  WiFi.begin(ssid, password);

  // Wait for the NodeMCU to connect to Wi-Fi
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to Wi-Fi...");
  }
  
  Serial.println("Connected to Wi-Fi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.begin();  // Start the server
}

void loop() {
  WiFiClient client = server.available();  // Listen for incoming clients

  if (client) {
    Serial.println("New client connected");
    String request = client.readStringUntil('\r');  // Read the request
    Serial.println(request);
    client.flush();

    // Check if the client requests to turn ON or OFF the LED
    if (request.indexOf("GET /on") != -1) {
      Serial.write('1');  // Send '1' to the Arduino (turn LED ON)
    } 
    if (request.indexOf("GET /off") != -1) {
      Serial.write('0');  // Send '0' to the Arduino (turn LED OFF)
    }

    // Respond to the client with enhanced HTML/CSS (Dark theme, responsive)
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println();
    client.println("<!DOCTYPE HTML>");
    client.println("<html lang=\"en\">");
    client.println("<head>");
    client.println("<meta charset=\"UTF-8\">");
    client.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">");
    client.println("<title>LED Control</title>");
    client.println("<style>");
    client.println("body { font-family: Arial, sans-serif; background-color: #121212; color: #ffffff; margin: 0; display: flex; justify-content: center; align-items: center; height: 100vh; }");
    client.println("h1 { text-align: center; font-size: 2.5em; }");
    client.println(".container { text-align: center; }");
    client.println("a { display: inline-block; margin: 20px; padding: 15px 30px; background-color: #1e90ff; color: #ffffff; text-decoration: none; font-size: 1.5em; border-radius: 10px; transition: background-color 0.3s; }");
    client.println("a:hover { background-color: #0a64c2; }");
    client.println(".dark { background-color: #333; color: white; border: 2px solid #555; }");
    client.println(".dark:hover { background-color: #555; }");
    client.println("</style>");
    client.println("</head>");
    client.println("<body>");
    client.println("<div class=\"container\">");
    client.println("<h1>LED Control Panel</h1>");
    client.println("<a href=\"/on\">Turn LED ON</a>");
    client.println("<a href=\"/off\" class=\"dark\">Turn LED OFF</a>");
    client.println("</div>");
    client.println("</body>");
    client.println("</html>");
    client.println();
    
    client.stop();  // Close the connection
    Serial.println("Client disconnected");
  }
}

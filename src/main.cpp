#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <EEPROM.h>

const char* ssid = "ESP8266_Hotspot";
const char* password = "password";

ESP8266WebServer server(80);
DNSServer dnsServer;

String inputValue = "";

void handleRoot() {
  Serial.println("Handling root request");
  String html = "<h1>Hello, Christine!</h1>";
  html += "<form action=\"/submit\" method=\"POST\">";
  html += "Input: <input type=\"text\" name=\"input\" value=\"" + inputValue + "\"><br>";
  html += "<input type=\"submit\" value=\"Submit\">";
  html += "</form>";
  server.send(200, "text/html", html);
}

void handleSubmit() {
  if (server.hasArg("input")) {
    inputValue = server.arg("input");
    Serial.println("Received input: " + inputValue);
    EEPROM.begin(512);
    for (int i = 0; i < inputValue.length(); ++i) {
      EEPROM.write(i, inputValue[i]);
    }
    EEPROM.write(inputValue.length(), '\0'); // Add termination character
    EEPROM.commit();
  }
  server.sendHeader("Location", "/");
  server.send(303);
}

void setup() {
  Serial.begin(9600);
  Serial.println("Serial started");
  
  WiFi.softAP(ssid, password);
  Serial.println("Hotspot started");
  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP());

  dnsServer.start(53, "*", WiFi.softAPIP());

  EEPROM.begin(512);
  char savedInput[512];
  for (int i = 0; i < 512; ++i) {
    savedInput[i] = EEPROM.read(i);
  }
  inputValue = String(savedInput);

  server.on("/", handleRoot);
  server.on("/submit", HTTP_POST, handleSubmit);
  server.begin();
  Serial.println("HTTP server started");
  Serial.println("To view the web page, connect to the WiFi network and open a web browser. Then, navigate to the IP address: " + WiFi.softAPIP().toString());
}

void loop() {
  dnsServer.processNextRequest();
  server.handleClient();
  Serial.println("Handling client");
  Serial.print("Free heap memory: ");
  Serial.println(ESP.getFreeHeap());
  delay(1000); // Add a delay to avoid flooding the serial output
}
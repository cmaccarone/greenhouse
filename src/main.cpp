//YWROBOT
//Compatible with the Arduino IDE 1.0
//Library version:1.1
#include <OneWire.h>
#include <DallasTemperature.h>
#include "SSD1306.h"

// Data wire is connected to the Arduino digital pin 4
#define ONE_WIRE_BUS D3

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);

// Address set here 0x3C that I found in the scanner, and pins defined as D2 (SDA/Serial Data), and D5 (SCK/Serial Clock).
SSD1306 lcd(0x3D, D2, D1);
SSD1306 lcd1(0x3C, D2, D1); //bottom set the LCD address to 0x27 for a 16 chars and 2 line display

int actLimit;
int incrementPin = D6;
int decrementPin = D5;
int relayPin = D7;

void setup() {
  pinMode(relayPin, OUTPUT);
  pinMode(incrementPin, INPUT);
  pinMode(decrementPin, INPUT);
  Serial.begin(115200);
  sensors.begin();
  lcd.init();  // initialize the lcd 
  lcd.setColor(WHITE);
  lcd1.init();
  // Print a message to the LCD.
  lcd.flipScreenVertically();
  lcd1.flipScreenVertically();
  
  digitalWrite(D8, HIGH);
}

void loop() {
  //Call sensors.requestTemperatures() to issue a global temperature and Requests to all devices on the bus
  sensors.requestTemperatures(); 
  if (digitalRead(incrementPin) == HIGH) {    
      if (actLimit < 50) {
    actLimit += 1;
    delay(250);
  }
  }
  
  if (digitalRead(decrementPin) == HIGH) {
      if (actLimit > 0) {
    actLimit -= 1;
    delay(250);
  }
  }

  if (sensors.getTempFByIndex(1) > sensors.getTempFByIndex(0) + actLimit) {
    digitalWrite(relayPin, HIGH);
  } else {
    digitalWrite(relayPin, LOW);
  }
  lcd1.clear();
  lcd.clear();
  lcd1.drawString(0, 0, "Stove: " + String(sensors.getTempFByIndex(1)) + "  "); 
  lcd1.drawString(0, 16, "OutPut: " + String(sensors.getTempFByIndex(0)) + "     ");
  lcd.drawString(0, 0, "Temp Diff: " + String(actLimit) + "  "); 
  lcd.drawString(0, 32, "Relay Status: " + String(digitalRead(relayPin) == HIGH ? "ON" : "OFF"));
  lcd.drawString(0, 16, "By: Caleb & Mario");
  lcd1.display();
  lcd.display();

  // Add logs to show temperature readings and relay status
  Serial.print("Temperature Sensor 0: ");
  Serial.println(sensors.getTempFByIndex(0));
  Serial.print("Temperature Sensor 1: ");
  Serial.println(sensors.getTempFByIndex(1));
  Serial.print("Temperature Difference Limit: ");
  Serial.println(actLimit);
  Serial.print("Relay Status: ");
  Serial.println(digitalRead(relayPin) == HIGH ? "ON" : "OFF");
}

void increment() {
  if (actLimit < 50) {
    actLimit += 1;
    delay(250);
  }
}

void decrement() {
  if (actLimit > 0) {
    actLimit -= 1;
    delay(250);
  }
}

// Documentation Section
// Pin Connections:
// - ONE_WIRE_BUS: D3 (Data wire for temperature sensor)
// - LCD 0x3D: D2 (SDA/Serial Data), D1 (SCK/Serial Clock)
// - LCD 0x3C: D2 (SDA/Serial Data), D1 (SCK/Serial Clock)
// - incrementPin: D6 (Button to increment temperature limit)
// - decrementPin: D5 (Button to decrement temperature limit)
// - relayPin: D7 (Relay control pin)
// - Serial communication: 115200 baud rate
// - Additional pin: D8 (Set to HIGH in setup)
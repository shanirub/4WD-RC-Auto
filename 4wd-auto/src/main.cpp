/*
 * Auto Module
 * Code for the ESP12-E microcontroller.
 * Connected to motor shield.
 * 
 * 1. connect to Olafchen V
 * 2. start wifiserver
 */

#include <Arduino.h>
#include <ESP8266WiFi.h>

// motors pins
#define LEFT_MOTORS_POWER D2 // (Motor Shield B) - Left motors
#define LEFT_MOTORS_DIRECTION D4
#define RIGHT_MOTORS_POWER D1 // (Motor Shield A) - Right motors
#define RIGHT_MOTORS_DIRECTION D3

int port = 2345;

WiFiServer server(port);

const char* ssid     = "Olafchen";
const char* password = "lalalalala";

int leftPower = 1023;         // left motors speed
uint8_t leftDirection = HIGH; // left motors direction
int rightPower = 1023;        // right motors speed
uint8_t rightDirection = LOW;// right motors direction


void drive_forward() {
  Serial.println(" Driving forward...");
  digitalWrite(LEFT_MOTORS_DIRECTION, leftDirection);
  digitalWrite(RIGHT_MOTORS_DIRECTION, rightDirection);
  analogWrite(RIGHT_MOTORS_POWER, rightPower);
  analogWrite(LEFT_MOTORS_POWER, leftPower);
}

void stop_driving() {
  Serial.println(" Stopping!");
  digitalWrite(RIGHT_MOTORS_DIRECTION, rightDirection);
  digitalWrite(LEFT_MOTORS_DIRECTION, leftDirection);
  analogWrite(RIGHT_MOTORS_POWER, 0);
  analogWrite(LEFT_MOTORS_POWER, 0);
}

void drive_backwards() {
  Serial.println(" Driving backwards...");
  digitalWrite(LEFT_MOTORS_DIRECTION, !leftDirection);
  digitalWrite(RIGHT_MOTORS_DIRECTION, !rightDirection);
  analogWrite(RIGHT_MOTORS_POWER, rightPower);
  analogWrite(LEFT_MOTORS_POWER, leftPower);
}

void turn_left() {
  Serial.println(" Turning left...");
  digitalWrite(LEFT_MOTORS_DIRECTION, !leftDirection);
  digitalWrite(RIGHT_MOTORS_DIRECTION, rightDirection);
  analogWrite(RIGHT_MOTORS_POWER, rightPower);
  analogWrite(LEFT_MOTORS_POWER, leftPower);
}

void turn_right() {
  Serial.println(" Turning right...");
  digitalWrite(LEFT_MOTORS_DIRECTION, leftDirection);
  digitalWrite(RIGHT_MOTORS_DIRECTION, !rightDirection);
  analogWrite(RIGHT_MOTORS_POWER, rightPower);
  analogWrite(LEFT_MOTORS_POWER, leftPower);
}

void setup() {
  Serial.begin(9600);
  Serial.println("");
  pinMode(LEFT_MOTORS_DIRECTION, OUTPUT);
  pinMode(LEFT_MOTORS_POWER, OUTPUT);
  pinMode(RIGHT_MOTORS_DIRECTION, OUTPUT);
  pinMode(RIGHT_MOTORS_POWER, OUTPUT);

  WiFi.begin(ssid, password);

  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());

  Serial.println("Starting WiFiServer in port " + port);
  server.begin();
}

void loop()
{
  WiFiClient client = server.available();
  // wait for a client (web browser) to connect
  if (client)
  {
    Serial.println("\n[Client connected]");
    while (client.connected())
    {
      // read line by line what the client (web browser) is requesting
      if (client.available())
      {
        String line = client.readStringUntil('\r');
        Serial.print(line);
        // wait for end of client's request, that is marked with an empty line
        if (line.length() == 1 && line[0] == '\n')
        {
          client.println("Hi client it's me, server");
          break;
        }
      }
    }
    delay(1); // give the web browser time to receive the data

    // close the connection:
    client.stop();
    Serial.println("[Client disconnected]");
  }
}

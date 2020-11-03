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
#include <ESP8266mDNS.h>

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

  if (!MDNS.begin("auto"))
  {
    Serial.println("Error setting up MDNS responder!");
    while (1)
    {
      delay(1000);
    }
  }
}

void loop()
{
  MDNS.update();
  WiFiClient client = server.available();
  // wait for a client (web browser) to connect
  if (client)
  {
    Serial.println("\n[Client connected]");
    while (client.connected())
    {
      // reading requested direction from rc module
      if (client.available())
      {
        int requested_direction = client.read();
        Serial.print(requested_direction);
        switch (requested_direction)
        {
        case 'l':
          turn_left();
          break;

        case 'r':
          turn_right();
          break;
        
        case 'f':
          drive_forward();
          break;

        case 'b':
          drive_backwards();
          break;

        case 's':
          stop_driving();
          break;

        default:
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

/*
 * Auto Module
 * Code for the ESP12-E microcontroller.
 * Connected to motor shield.
 * 
 * 1. connects to AP (runs on rc-module)
 * 2. starts a WebSocketServer (https://github.com/Links2004/arduinoWebSockets)
 * 
 * 
 */

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WebSocketsServer.h>

// motors pins
#define LEFT_MOTORS_POWER D2 // (Motor Shield B) - Left motors
#define LEFT_MOTORS_DIRECTION D4
#define RIGHT_MOTORS_POWER D1 // (Motor Shield A) - Right motors
#define RIGHT_MOTORS_DIRECTION D3

int port = 2345;

WebSocketsServer ws = WebSocketsServer(port);

const char *ssid = "Olafchen";
const char *password = "lalalalala";

int leftPower = 1023;         // left motors speed
uint8_t leftDirection = HIGH; // left motors direction
int rightPower = 1023;        // right motors speed
uint8_t rightDirection = LOW; // right motors direction



void drive_forward()
{
  Serial.println(" Driving forward...");
  digitalWrite(LEFT_MOTORS_DIRECTION, leftDirection);
  digitalWrite(RIGHT_MOTORS_DIRECTION, rightDirection);
  analogWrite(RIGHT_MOTORS_POWER, rightPower);
  analogWrite(LEFT_MOTORS_POWER, leftPower);
}

void stop_driving()
{
  Serial.println(" Stopping!");
  digitalWrite(RIGHT_MOTORS_DIRECTION, rightDirection);
  digitalWrite(LEFT_MOTORS_DIRECTION, leftDirection);
  analogWrite(RIGHT_MOTORS_POWER, 0);
  analogWrite(LEFT_MOTORS_POWER, 0);
}

void drive_backwards()
{
  Serial.println(" Driving backwards...");
  digitalWrite(LEFT_MOTORS_DIRECTION, !leftDirection);
  digitalWrite(RIGHT_MOTORS_DIRECTION, !rightDirection);
  analogWrite(RIGHT_MOTORS_POWER, rightPower);
  analogWrite(LEFT_MOTORS_POWER, leftPower);
}

void turn_left()
{
  Serial.println(" Turning left...");
  digitalWrite(LEFT_MOTORS_DIRECTION, !leftDirection);
  digitalWrite(RIGHT_MOTORS_DIRECTION, rightDirection);
  analogWrite(RIGHT_MOTORS_POWER, rightPower);
  analogWrite(LEFT_MOTORS_POWER, leftPower);
}

void turn_right()
{
  Serial.println(" Turning right...");
  digitalWrite(LEFT_MOTORS_DIRECTION, leftDirection);
  digitalWrite(RIGHT_MOTORS_DIRECTION, !rightDirection);
  analogWrite(RIGHT_MOTORS_POWER, rightPower);
  analogWrite(LEFT_MOTORS_POWER, leftPower);
}




/**
 * Method addapted from:
 * https://github.com/Links2004/arduinoWebSockets/edit/master/examples/esp8266/WebSocketServer/WebSocketServer.ino
 * 
*/
void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length)
{
  IPAddress ip = ws.remoteIP(num);


  switch (type)
  {

    case WStype_DISCONNECTED:
      Serial.printf("[%u] Disconnected!\n", num);
      break;

    case WStype_CONNECTED:
      Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
      Serial.printf("Requested direction: %s", payload);
      switch (*payload)
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
          Serial.println(" Invalid direction, " + *payload);
      }

      // send message to client
      ws.sendTXT(num, "Connected");
      break;

    case WStype_TEXT:
      Serial.printf("[%u] get Text: %s\n", num, payload);

      // send message to client
      // webSocket.sendTXT(num, "message here");

      // send data to all connected clients
      // webSocket.broadcastTXT("message here");
      break;
    case WStype_BIN:
      Serial.printf("[%u] get binary length: %u\n", num, length);
      hexdump(payload, length);

      // send message to client
      // webSocket.sendBIN(num, payload, length);
      break;
  }
}

  void setup()
  {
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

    Serial.println("Starting WebSocketsServer on port " + port);
    ws.begin();
    ws.onEvent(webSocketEvent);
  }

  void loop()
  {
    ws.loop();
  }
/*
 * Code for the ESP12-E microcontroller.
 * Connected to motor shield.
 */

#include <Arduino.h>

// motors pins
#define LEFT_MOTORS_POWER D2 // (Motor Shield B) - Left motors
#define LEFT_MOTORS_DIRECTION D4
#define RIGHT_MOTORS_POWER D1 // (Motor Shield A) - Right motors
#define RIGHT_MOTORS_DIRECTION D3

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
}

void loop() {
  drive_backwards();
  delay(500);
  stop_driving();
  delay(500);
  turn_left();
  delay(500);
  turn_right();
  delay(500);
}
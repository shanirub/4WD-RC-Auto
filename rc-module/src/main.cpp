/*
*   ESP32 remote control module.
*   
*   Wifi Client
*   Access point
*   
Note: ADC2 pins cannot be used when Wi-Fi is used. So, if you’re using Wi-Fi and you’re having trouble getting the value from an ADC2 GPIO, you may consider using an ADC1 GPIO instead, that should solve your problem. 
*/

#include <Arduino.h>
#include <driver/adc.h>
#include <WiFi.h>
#include "esp_wifi.h"
#include <ESPmDNS.h>

#define RIGHT_X 35
#define RIGHT_Y 32
#define RIGHT_SW 33
#define LEFT_X 39
#define LEFT_Y 36
#define LEFT_SW 34
#define LED 22

const char *ssid = "Olafchen";
const char *password = "lalalalala";

const int port = 2345;

char *wifiserver_ip;

char direction = 's';

int right_x_value; // right control: 0 up, 4095 down
int right_y_value;
int right_sw_value;
int left_x_value;
int left_y_value; // left control: 0 up, 4095 down
int left_sw_value;

void setup()
{

  Serial.begin(9600);

  // piModes
  pinMode(LED, OUTPUT);
  pinMode(RIGHT_SW, INPUT_PULLUP);
  pinMode(RIGHT_X, ANALOG);
  pinMode(RIGHT_Y, ANALOG);
  pinMode(LEFT_SW, INPUT_PULLUP);
  pinMode(LEFT_X, ANALOG);
  pinMode(LEFT_Y, ANALOG);

  // establish access point
  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  // wait for auto (esp12) to connect
  while (WiFi.softAPgetStationNum() == 0)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.print("Stations connected: ");
  Serial.println(WiFi.softAPgetStationNum());

  // here we need to find the station's ip connected to this ap
  wifi_sta_list_t wifi_sta_list;
  tcpip_adapter_sta_list_t adapter_sta_list;

  memset(&wifi_sta_list, 0, sizeof(wifi_sta_list));
  memset(&adapter_sta_list, 0, sizeof(adapter_sta_list));

  esp_wifi_ap_get_sta_list(&wifi_sta_list);
  tcpip_adapter_get_sta_list(&wifi_sta_list, &adapter_sta_list);

  for (int i = 0; i < adapter_sta_list.num; i++)
  {

    tcpip_adapter_sta_info_t station = adapter_sta_list.sta[i];

    Serial.print("station nr ");
    Serial.println(i);

    Serial.print("MAC: ");

    for (int i = 0; i < 6; i++)
    {

      Serial.printf("%02X", station.mac[i]);
      if (i < 5)
        Serial.print(":");
    }

    Serial.print("\nIP: ");

    char *station_ip = ip4addr_ntoa(&(station.ip));
    Serial.println(station_ip);

    wifiserver_ip = station_ip;
  }

  if (!MDNS.begin("rc-module"))
  {
    Serial.println("Error setting up MDNS responder!");
    while (1)
    {
      delay(1000);
    }
  }
  Serial.println("mDNS responder started");

  Serial.println("-----------");
  delay(5000);
}

void print_values()
{
  Serial.print("Right X ");
  Serial.print(right_x_value);
  Serial.print("| Y ");
  Serial.print(right_y_value);
  Serial.print("| SW ");
  Serial.print(right_sw_value);
  Serial.println("");

  Serial.print("Left X ");
  Serial.print(left_x_value);
  Serial.print("| Y ");
  Serial.print(left_y_value);
  Serial.print("| SW ");
  Serial.print(left_sw_value);
  Serial.println("");
}

void loop()
{
  
  right_x_value = analogRead(RIGHT_X); // right control: 0 up, 4095 down
  right_y_value = analogRead(RIGHT_Y);
  right_sw_value = digitalRead(RIGHT_SW);
  left_x_value = analogRead(LEFT_X);
  left_y_value = analogRead(LEFT_Y); // left control: 0 up, 4095 down
  left_sw_value = digitalRead(LEFT_X);

  print_values();

  if ((right_x_value < 500) && (left_y_value < 500))
  {
    direction = 'f';
  }
  else if ((right_x_value > 3500) && (left_y_value > 3500))
  {
    direction = 'b';
  }
  else if ((left_y_value > 3500) && (right_x_value < 500))
  {
    direction = 'l';
  }
  else if ((left_y_value < 500) && (right_x_value > 3500))
  {
    direction = 'r';
  }
  else
  {
    direction = 's';
  }

  WiFiClient client;
  if (!client.connect(wifiserver_ip, port))
  {
    Serial.println("connection failed");
    return;
  }

  // This will send the request to the server
  Serial.print(" Sending auto the following direction: ");
  Serial.println(direction);
  client.print(direction);
  delay(10);

  // Read all the lines of the reply from server and print them to Serial
  while (client.available())
  {
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }

  Serial.println();
  Serial.println("closing connection");

  delay(500);
}
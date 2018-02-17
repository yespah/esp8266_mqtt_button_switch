#include <ESP8266WiFi.h>
#include <MQTT.h>
#include "credentials.h"

#define BUTTON_PIN 2

void myDataCb(String& topic, String& data);
void myPublishedCb();
void myDisconnectedCb();
void myConnectedCb();

#define CLIENT_ID "kontakt1"

// create MQTT object
MQTT myMqtt(CLIENT_ID, "192.168.72.200", 1883);

//
void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);

  WiFi.begin(WIFI_SSID, WIFI_PASS);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  Serial.println("Connecting to MQTT server");

  // setup callbacks
  myMqtt.onConnected(myConnectedCb);
  myMqtt.onDisconnected(myDisconnectedCb);
  myMqtt.onPublished(myPublishedCb);
  myMqtt.onData(myDataCb);

  Serial.println("connect mqtt...");
  myMqtt.connect();

  delay(10);
}

//
void loop() {

  int value = analogRead(A0);

  String topic("/");
  topic += CLIENT_ID;
  topic += "/value";

  String valueStr(value);

  // publish value to topic
  boolean result = myMqtt.publish(topic, valueStr);

  delay(1000);
}


/*
 *
 */
void myConnectedCb()
{
  Serial.println("connected to MQTT server");
}

void myDisconnectedCb()
{
  Serial.println("disconnected. try to reconnect...");
  delay(500);
  myMqtt.connect();
}

void myPublishedCb()
{
  //Serial.println("published.");
}

void myDataCb(String& topic, String& data)
{

  Serial.print(topic);
  Serial.print(": ");
  Serial.println(data);
}

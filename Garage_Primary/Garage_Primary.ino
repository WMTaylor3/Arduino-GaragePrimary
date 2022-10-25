#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFi.h>
#include "credentials.h"

#define GARAGE_DOOR_CONTROL_PIN 13

WiFiClient wifiClient;
PubSubClient mqtt;

// Variables
unsigned long timeOfLastReboot = 0;
unsigned long rebootInterval = 86400000; // 24 Hours.

bool timeHasElapsed(unsigned long timeOfLastExe, unsigned long delayLength) {
  return (millis() - timeOfLastExe > delayLength);
}

void mqttCallback(char* topic, byte* payload, unsigned int length) {
  Serial.print(F("Message arrived ["));
  Serial.print(topic);
  Serial.print(F("] "));
  for (int i = 0; i < length; i++) {
    char receivedChar = (char)payload[i];
    Serial.print(receivedChar);
  }

  Serial.println();

  if((char)payload[0] == 't' && (char)payload[1] == 'g' && (char)payload[2] == 'l' && (char)payload[3] == 'e'){
    Serial.println("Toggling");
    mqtt.publish(MQTT_TOPIC, "Toggling Garage Door");
    digitalWrite(GARAGE_DOOR_CONTROL_PIN, HIGH);
    delay(500);
    digitalWrite(GARAGE_DOOR_CONTROL_PIN, LOW);
  }
}

void setup() {
  pinMode(GARAGE_DOOR_CONTROL_PIN, OUTPUT);
  
  Serial.begin(9600);

  setupWifi();

  setupMqtt();
}

void setupWifi() {
  Serial.print(F("Connecting to WiFi: "));
  Serial.println(WIFI_NAME);

  WiFi.begin(WIFI_NAME, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(F("."));
  }

  Serial.println(F(""));
  Serial.println(F("WiFi connected"));
  Serial.println(F("IP address: "));
  Serial.println(WiFi.localIP());
}

void setupMqtt() {
  mqtt.setClient(wifiClient);
  mqtt.setServer(MQTT_SERVER, MQTT_PORT);
  mqtt.setCallback(mqttCallback);
}

void reconnectMqtt() {
  while (!mqtt.connected()) {
    Serial.print(F("Attempting MQTT connection..."));
    if (mqtt.connect(MQTT_ID, MQTT_USER, MQTT_PASSWORD)) {
      Serial.println(F("Connected"));
      Serial.print("Subscribing to topic ");
      Serial.print(MQTT_TOPIC);
      Serial.print(" result: ");
      Serial.println(mqtt.subscribe(MQTT_TOPIC));
      mqtt.publish(MQTT_TOPIC, "Subscribed to MQTTT");
    } else {
      Serial.print(F("Failed, rc="));
      Serial.print(mqtt.state());
      Serial.println(F(" try again in 5 seconds"));
      delay(5000);
    }
  }
}

void loop() {
  if(timeHasElapsed(timeOfLastReboot, rebootInterval)) {
    timeOfLastReboot = millis();
    ESP.restart();
  }
  
  if(WiFi.status() != WL_CONNECTED) {
    setupWifi();
  }
  
  if (!mqtt.connected()) {
    reconnectMqtt();
  }

  mqtt.loop();
}

#pragma once

#include <WiFi.h>
extern "C" {
	#include "freertos/FreeRTOS.h"
	#include "freertos/timers.h"
}
#include <AsyncMqttClient.h>
#include <ArduinoJson.h>

#include "settings.h"


AsyncMqttClient mqttClient;
TimerHandle_t mqttReconnectTimer;
TimerHandle_t wifiReconnectTimer;
boolean eth_connected = false;


void connectToMqtt() {
  SER.println("Connecting to MQTT...");
  mqttClient.connect();
}

void connectWifi() {
  SER.println("Connecting to Wi-Fi...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}

void WiFiEvent(WiFiEvent_t event) {
    SER.printf("[WiFi-event] event: %d\n", event);
    switch(event) {
    case SYSTEM_EVENT_STA_GOT_IP:
        SER.println("WiFi connected");
        SER.println("IP address: ");
        SER.println(WiFi.localIP());
        connectToMqtt();
        break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
        SER.println("WiFi lost connection");
        xTimerStop(mqttReconnectTimer, 0); // ensure we don't reconnect to MQTT while reconnecting to Wi-Fi
		    xTimerStart(wifiReconnectTimer, 0);
        break;
    }
}

void onMqttConnect(bool sessionPresent) {
  SER.println("Connected to MQTT.");
  SER.print("Session present: ");
  SER.println(sessionPresent);
}

void publishMessage(String topic, String message) {
  mqttClient.publish(topic.c_str(), 0, true, message.c_str());
}

void onMqttDisconnect(AsyncMqttClientDisconnectReason reason) {
  SER.println("Disconnected from MQTT.");

  if (WiFi.isConnected()) {
    xTimerStart(mqttReconnectTimer, 0);
  }
}

void onMqttSubscribe(uint16_t packetId, uint8_t qos) {
  SER.println("Subscribe acknowledged.");
  SER.print("  packetId: ");
  SER.println(packetId);
  SER.print("  qos: ");
  SER.println(qos);
}

void onMqttUnsubscribe(uint16_t packetId) {
  SER.println("Unsubscribe acknowledged.");
  SER.print("  packetId: ");
  SER.println(packetId);
}

void onMqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total) {
  SER.println("Publish received.");
  SER.print("  topic: ");
  SER.println(topic);
  SER.print("  qos: ");
  SER.println(properties.qos);
  SER.print("  dup: ");
  SER.println(properties.dup);
  SER.print("  retain: ");
  SER.println(properties.retain);
  SER.print("  len: ");
  SER.println(len);
  SER.print("  index: ");
  SER.println(index);
  SER.print("  total: ");
  SER.println(total);
}

void onMqttPublish(uint16_t packetId) {
  SER.println("Publish acknowledged.");
  SER.print("  packetId: ");
  SER.println(packetId);
}

void sendMqttMessage(String jsonMessage) {
  if (mqttClient.connected()) {

  } else {
    SER.println("MQTT is not connected. Discarding message");
  }
}

void mqttInit() {

  mqttReconnectTimer = xTimerCreate("mqttTimer", pdMS_TO_TICKS(2000), pdFALSE, (void*)0, reinterpret_cast<TimerCallbackFunction_t>(connectToMqtt));
  wifiReconnectTimer = xTimerCreate("wifiTimer", pdMS_TO_TICKS(2000), pdFALSE, (void*)0, reinterpret_cast<TimerCallbackFunction_t>(connectWifi));

  WiFi.onEvent(WiFiEvent);

  mqttClient.onConnect(onMqttConnect);
  mqttClient.onDisconnect(onMqttDisconnect);
  mqttClient.onSubscribe(onMqttSubscribe);
  mqttClient.onUnsubscribe(onMqttUnsubscribe);
  mqttClient.onMessage(onMqttMessage);
  mqttClient.onPublish(onMqttPublish);
  mqttClient.setServer(MQTT_HOST, MQTT_PORT);

  connectWifi();

}



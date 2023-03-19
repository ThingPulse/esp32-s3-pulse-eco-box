#include <Arduino.h>
#include <Wire.h>
#include "mqtt.h"
#include "pm-sensor.h"
#include "led.h"
#include "bme280.h"
#include "accelerometer.h"
#include "pdm-mic.h"

int ledIndex = 0;

TwoWire i2cBus = TwoWire(1); //I2C1 bus

void setup() {
    delay(3000);
    SER.begin(115200);
    SER.printf("Total heap: %d\n", ESP.getHeapSize());
    SER.printf("Free heap: %d\n", ESP.getFreeHeap());
    SER.printf("Total PSRAM: %d\n", ESP.getPsramSize());
    SER.printf("Free PSRAM: %d\n", ESP.getFreePsram());
    SER.println("Started");
    i2cBus.setPins(I2C_SDA, I2C_SCL);
    initBme280(&i2cBus);
    initQma7981(&i2cBus);
    mqttInit();
    //initPdmMic();
    //initPdmMicServer();
    pmInit();
    
}

void loop() {

    String pmMessage = getPmAsJson();
    publishMessage(SPS_MQTT_TOPIC, pmMessage);
    Serial.println(pmMessage);
    String bmeMessage = getBme280AsJson();
    publishMessage(BME_MQTT_TOPIC, bmeMessage);
    Serial.println(bmeMessage);
    String accelMessage = getQma7981AsJson();
    Serial.println(accelMessage);
    publishMessage(QMA_MQTT_TOPIC, accelMessage);
    delay(1000);
    //runMicServer();

}
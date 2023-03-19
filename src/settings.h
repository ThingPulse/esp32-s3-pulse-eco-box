#define SER Serial

// For Sensirion
#define SPS_SCL 6
#define SPS_SDA 7
#define SPS_MQTT_TOPIC "pulse.eco/sps"

// For BME280 and QMA7981
#define I2C_SCL 5
#define I2C_SDA 4
#define BME_I2C_ADDR 0x76
#define BME_MQTT_TOPIC "pulse.eco/bme"
#define QMA_MQTT_TOPIC "pulse.eco/qma"

#define WIFI_SSID "yourssid"
#define WIFI_PASSWORD "yourpassw0rd"

#define MQTT_HOST IPAddress(192, 168, 0, 100)
#define MQTT_PORT 1883
#define MQTT_TOPIC_SEND "pulse.eco/pm"

// LED Settings
#define ESP_LED_PIN 8
#define LED_COUNT   4
#define CHANNEL     1

// PDM Microphone GMA4030H11-F26
#define PIN_PDM_CLK 41
#define PIN_PDM_DATA 2

#define SAMPLE_RATE 22050
#define NUM_CHANNEL 1
#if NUM_CHANNEL == 1
   #define CHANNEL_FORMAT I2S_CHANNEL_FMT_ONLY_LEFT
   #define CHANNEL_MODE I2S_CHANNEL_MONO
#else
   #define CHANNEL_FORMAT I2S_CHANNEL_FMT_RIGHT_LEFT
   #define CHANNEL_MODE I2S_CHANNEL_STEREO
#endif

#define BYTES_PER_CHANNEL 2
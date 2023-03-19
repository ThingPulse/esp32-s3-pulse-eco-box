
#include <Arduino.h>
#include "WiFi.h"
#include <driver/i2s.h>
//#include "driver/i2s_pdm.h"
#include <soc/i2s_reg.h>
#include "format_wav.h"

#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include "settings.h"



const i2s_port_t I2S_PORT = I2S_NUM_0;


WebServer server(80);

 // Buffer
volatile uint16_t rpt = 0; // Pointer


void varyGain(uint8_t * buf, int32_t bufSize, int16_t gain)
{
    int16_t tempIS, tempUS;
    float value;

    for (int i = 0; i<(bufSize/2); i++) {
        tempIS = buf[i*2+1] << 8 | buf[i*2];
        value = tempIS/32768.00000;
        value = value*gain; 
        if (value>1) value = 1;                   //clipping
        if (value<-1) value = -1;
        tempUS = value*32768;
        
        buf[i*2] = 0x00FF&tempUS;
        buf[i*2+1] = tempUS>>8;
    }
}

void handleRoot() {
  SER.println("Handle root...");
  //server.send(200, "audio/x-wav", "hello from esp32!");
  uint32_t recTime = 2;
  uint32_t wavSize = recTime * SAMPLE_RATE * NUM_CHANNEL * BYTES_PER_CHANNEL;
  const wav_header_t wav_header =
        WAV_HEADER_PCM_DEFAULT(wavSize, BYTES_PER_CHANNEL * 8, SAMPLE_RATE / 2, NUM_CHANNEL);


  SER.println("Init Buffer done");
  const int16_t headerSize = sizeof(wav_header);
  size_t fileSize = headerSize + wavSize;
  SER.printf("RecTime: %d, headerSize: %d, wavSize: %d, fileSize: %d\n", recTime, headerSize, wavSize, fileSize);
  //uint8_t * buffer = (uint8_t*) malloc(fileSize);
  uint8_t * buffer = (uint8_t*) ps_malloc(fileSize);
  memcpy(buffer, &wav_header, headerSize);
  SER.println("Copying header to buffer: done");
  uint32_t num_bytes_read = 0;
  i2s_read(I2S_PORT, (char *)buffer + headerSize,  wavSize, &num_bytes_read, portMAX_DELAY); // no timeout
  SER.println("Recording audio: done");
  varyGain(buffer + headerSize, wavSize, 5);
  SER.printf("Bytes read: %d\n", num_bytes_read);
  server.sendContent((char*)buffer, fileSize);
  free(buffer);

}

void initPdmMic() {

    esp_err_t err;

    i2s_config_t i2s_config = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX | I2S_MODE_PDM),
        .sample_rate = SAMPLE_RATE,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
        .channel_format = CHANNEL_FORMAT,
        .communication_format = I2S_COMM_FORMAT_STAND_PCM_LONG,
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL3,
        .dma_buf_count = 8,
        .dma_buf_len = 1024,
        .use_apll = true,
        .fixed_mclk = 0
    };

    /*
        MIC Pins
        PDM_DATA 2
        PDM_CLK 41
    */
    // ESP32 GPIO PINS > I2S MIC PINS:
    const i2s_pin_config_t pin_config = {
        .bck_io_num = I2S_PIN_NO_CHANGE,   // > SCK
        .ws_io_num = PIN_PDM_CLK,    // > WS
        .data_out_num = I2S_PIN_NO_CHANGE, // MYSERIAL Data Out is no connected
        .data_in_num = PIN_PDM_DATA,  // > SD
    };

    i2s_pcm_cfg_t pcm_config = {
        .pcm_type = I2S_PCM_DISABLE
    };

    // CONFIGURE I2S DRIVER AND PINS.
    err = i2s_driver_install(I2S_PORT, &i2s_config, 0, NULL);
    if (err != ESP_OK) {
        SER.printf("Failed installing driver: %d\n", err);
        while (true);
    }

    err = i2s_set_pin(I2S_PORT, &pin_config);
    if (err != ESP_OK) {
        SER.printf("Failed setting pin: %d\n", err);
        while (true);
    }
    err = i2s_set_clk(I2S_PORT, SAMPLE_RATE, I2S_BITS_PER_SAMPLE_16BIT, CHANNEL_MODE);
    if (err != ESP_OK) {
        SER.printf("Failed setting clk: %d\n", err);
        while (true);
    }
    err = i2s_set_pdm_rx_down_sample(I2S_PORT, I2S_PDM_DSR_16S);
    if (err != ESP_OK) {
        SER.printf("Failed setting pdm rx downsampling: %d\n", err);
        while (true);
    }
    err = i2s_pcm_config(I2S_PORT, &pcm_config);
    if (err != ESP_OK) {
        SER.printf("Failed setting pcm config: %d\n", err);
        while (true);
    }

}

void initPdmMicServer() {
    SER.println("I2S driver OK");
    server.on("/sound.wav", handleRoot);
    server.on("/wav", handleRoot);
    server.begin();
}

void runMicServer() {
    server.handleClient();
}
#pragma once

#include <FastLED.h>
#include "settings.h"

CRGB leds[LED_COUNT];

// Initialize 
void initStatusLeds() {
  FastLED.addLeds<WS2812B, ESP_LED_PIN, RGB>(leds, LED_COUNT);
  FastLED.setBrightness(255);
}

void clearLeds() {
  for (int i = 0; i < LED_COUNT; i++) {
    leds[i] = CRGB::Black;
  }
}

// Set the color of LED at position <ledIndex>
void updateLed(uint8_t ledIndex, CRGB led) {
    leds[ledIndex] = led;
    FastLED.show();
    FastLED.delay(1000 / 10);
}

void cycleLeds(CRGB color) {
    for (int i = 0; i < LED_COUNT; i++) {
        updateLed((i + LED_COUNT - 1) % LED_COUNT, CRGB::Black);
        updateLed(i, color);
        delay(250);
    }
    updateLed(LED_COUNT - 1, CRGB::Black);
}
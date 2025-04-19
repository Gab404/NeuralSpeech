#include <Arduino.h>
#include "header.h"
#include "globals.h"

// Globals variables
volatile float window[WIN_SIZE] = {0.0f};
volatile bool windowReady = false;
volatile float previousGain = 1.0f;

float mfcc[N_MFCC];

float winBuffer[WIN_SIZE];
float vImag[WIN_SIZE] = {0.0f};

uint8_t countWarmUpFrames = 0;

Adafruit_SSD1306 display(128, 64);
ArduinoFFT<float> FFT = ArduinoFFT<float>(winBuffer, vImag, WIN_SIZE, 8000);

void setup() {
  Serial.begin(460800);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();

  setupADC();
}

void loop() {

  if (windowReady) {
    // Copy window to buffer and shift window
    for (uint16_t i = 0; i < WIN_SIZE / 2; i++) {
      winBuffer[i] = window[i];
      winBuffer[i + (WIN_SIZE / 2)] = window[i + (WIN_SIZE / 2)];
      window[i] = window[i + (WIN_SIZE / 2)];
      vImag[i] = 0.0f;
      vImag[i + (WIN_SIZE / 2)] = 0.0f;
    }

    applyAGC(winBuffer);

    if (countWarmUpFrames >= WARM_UP_FRAMES) {
      getFFT(winBuffer, FFT); // time = 22 380 us

      for (uint16_t i = 0; i < WIN_SIZE; i++)
        winBuffer[i] /= 13;
  
      uint8_t bar_height;
  
      display.clearDisplay();
      for (uint8_t i = 0; i < WIN_SIZE / 2; i++) {
          bar_height = (uint8_t)(winBuffer[i] * 64);
          display.drawLine(i, 63, i, 63 - bar_height, WHITE);
      }
    
      display.display();
  
      getMFCC(winBuffer, mfcc); // time = 5 240 us
    } else
      countWarmUpFrames++;


    windowReady = false;
  }
}
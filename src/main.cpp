#include <Arduino.h>
#include "header.h"

uint16_t buffer[TAPS] = {0};
uint8_t lenBuffer = 0;

float mfcc[N_MFCC];

float window[WIN_SIZE] = {0.0f};
float vImag[WIN_SIZE] = {0.0f};

uint8_t indexBuffer = 0;
uint8_t indexDownSampling = 0;
uint8_t indexWindow = 0;

Adafruit_SSD1306 display(128, 64);
ArduinoFFT<float> FFT = ArduinoFFT<float>(window, vImag, WIN_SIZE, 8000);

void setup() {
  Serial.begin(460800);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();

  setupADC();
}

void loop() {
  uint16_t sample;

  while((ADC->ADC_ISR & 0x80)==0);

  sample = ADC->ADC_CDR[7];

  buffer[indexBuffer] = sample;
  if (lenBuffer == TAPS - 1) {

    if (indexDownSampling == 3) {

      window[indexWindow] = applyRIF(buffer, indexBuffer); // time = 1ms
      vImag[indexWindow] = 0.0f;

      if (indexWindow == WIN_SIZE - 1) {
        
        normalizeByEnergy(window); // time = 2ms
        getFFT(window, FFT); // time = 22ms

        uint8_t bar_height;
  
        display.clearDisplay();
        for (uint8_t i = 0; i < WIN_SIZE / 2; i++) {
            bar_height = (uint8_t)(window[i] * 32);
            display.drawLine(i, 63, i, 63 - bar_height, WHITE);
        }
      
        display.display();

        getMFCC(window, mfcc); // time = 5ms


        // Serial.print("MFCC : ");
        // for (uint8_t i = 0; i < N_MFCC; i++) {
        //   Serial.print(mfcc[i]);
        //   Serial.print(" ");
        // }
        // Serial.println("");

        indexWindow = 0;
      } else {
        indexWindow ++;
      }
    }
    indexBuffer = (indexBuffer + 1) % TAPS;
    indexDownSampling = (indexDownSampling + 1) & 3;
  } else {
    indexBuffer ++;
    lenBuffer ++;
  }
}